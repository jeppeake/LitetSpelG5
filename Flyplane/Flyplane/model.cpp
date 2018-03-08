#include "model.h"

#include <iostream>
#include <limits>
#include <assimp/material.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL\glew.h>
#include <glm\gtx\transform.hpp>
#include <stack>
#include "texture.h"


Model::Mesh::Mesh(aiMesh * mesh)
{
	name = mesh->mName.C_Str();


	std::cout << "\t\t[DEBUG] parsing mesh: '" << name;

	is_bb = false;
	if (name.substr(0, 3) == "BB_") {
		std::cout << "', Is BB";
		is_bb = true;
	}
	std::cout << "\n";

	material_index = mesh->mMaterialIndex;

	// Vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D normal;
		if (mesh->HasNormals())
		{
			normal = mesh->mNormals[i];
		}
		aiVector2D tex_coords;
		if (mesh->HasTextureCoords(0))
		{
			tex_coords.x = mesh->mTextureCoords[0][i].x;
			tex_coords.y = mesh->mTextureCoords[0][i].y;
		}
		auto pos = mesh->mVertices[i];
		aiVector3D new_pos(pos.x, pos.z, -pos.y);

		vertices.emplace_back(pos,
			normal,
			tex_coords);
	}

	// Indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < 3; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (!is_bb) 
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);


		unsigned int stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(glm::vec3)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(glm::vec3)));


		glBindVertexArray(0);
	}
}

Model::Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices)
{
	vertices = _vertices;
	indices = _indices;
}

Model::Mesh::~Mesh()
{
	if (!is_bb) {
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
}

void Model::Mesh::bind()
{
	if (!is_bb) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}
}

void Model::Mesh::unbind()
{
	glBindVertexArray(0);
}


void Model::load(const std::string & file)
{


	if (loaded) {
		std::cout << "[WARNING] model '" << file << "', already loaded. Skipping\n";
		return;
	}
	std::cout << "[DEBUG] loading: '" << file << "'\n";

	const aiScene* scene = aiImportFile(file.c_str(),
		//aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_OptimizeMeshes |
		//aiProcess_OptimizeGraph |
		aiProcess_SortByPType |
		//aiProcess_PreTransformVertices |
		aiProcess_ImproveCacheLocality

	);
	if (!scene)
	{
		std::cout << "\t[ERROR] Could not open model '" << file << "': " + std::string(aiGetErrorString()) << "\n";
		return;
	}

	std::cout << "\t[DEBUG] loading meshes in model:\n";
	loaded = true;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.emplace_back(scene->mMeshes[i]);
	}

	std::string model_dir;
	size_t last = file.find_last_of('/');
	if (last >= file.size())
		model_dir = "";
	else
		model_dir = file.substr(0, last + 1);

	for (int i = 0; i < scene->mNumMaterials; i++) {
		auto t = aiTextureType_DIFFUSE;
		auto mat = scene->mMaterials[i];

		aiString ainame;
		mat->Get(AI_MATKEY_NAME, ainame);

		std::string matName(ainame.C_Str());

		std::string tex_file;
		if (mat->GetTextureCount(t) == 0) {
			std::cout << "\t[ERROR] material missing diffuse texture, using korven.png\n";
			//system("pause");
			//exit(EXIT_FAILURE);
			tex_file = "assets/textures/korven.png";
		}
		else 
		{
			aiString filename;
			mat->Get(AI_MATKEY_TEXTURE(t, 0), filename);
			tex_file = model_dir + std::string(filename.C_Str());
		}
		std::cout << "\t[DEBUG] Processing mat: '" << matName << "', using texture: '" << tex_file << "'\n";
		if (!texture.loadTexture(tex_file))
		{
			//std::cout << "\tERROR: Could not load texture: '" << tex_file << "'\n";
		}
	}


	aiNode* airoot = scene->mRootNode;
	Node* root = new Node();

	recursiveBuildTree(airoot, root);

	// flatten tree
	recursiveFlatten(root, glm::scale(glm::vec3(0.005)));

	recursiveDeleteNodes(root);
	
	//std::cout << file << ":\n";
	std::cout << "\t[DEBUG] meshes: " << meshes.size() << "\n";
	std::cout << "\t[DEBUG] in hierarchy: " <<  model_meshes.size() << "\n";
	std::cout << "\t[DEBUG] bounding boxes: " << bounding_boxes.size() << "\n";
	

	aiReleaseImport(scene);
}


void Model::buildBoundingBoxes(Mesh * m, glm::mat4 transform)
{
	BoundingBox bmin;
	BoundingBox bmax;

	bmin.sides[0].x = std::numeric_limits<float>::max();
	bmax.sides[0].x = -std::numeric_limits<float>::max();

	bmin.sides[1].y = std::numeric_limits<float>::max();
	bmax.sides[1].y = -std::numeric_limits<float>::max();

	bmin.sides[2].z = std::numeric_limits<float>::max();
	bmax.sides[2].z = -std::numeric_limits<float>::max();

	float max_radius = 0.f;
	

	for (int i = 0; i < m->vertices.size(); i++) {
		auto pos = transform*glm::vec4(m->vertices[i].position, 1.0);

		float len = glm::length(pos);
		if (len > max_radius)
			max_radius = len;

		if (pos.x < bmin.sides[0].x)
			bmin.sides[0].x = pos.x;
		if (pos.x > bmax.sides[0].x)
			bmax.sides[0].x = pos.x;


		if (pos.y < bmin.sides[1].y)
			bmin.sides[1].y = pos.y;
		if (pos.y > bmax.sides[1].y)
			bmax.sides[1].y = pos.y;


		if (pos.z < bmin.sides[2].z)
			bmin.sides[2].z = pos.z;
		if (pos.z > bmax.sides[2].z)
			bmax.sides[2].z = pos.z;

		// if new box
		if (i % 24 == 23)
		{
			BoundingBox result;
			result.center.x = (bmax.sides[0].x + bmin.sides[0].x)*0.5f;
			result.center.y = (bmax.sides[1].y + bmin.sides[1].y)*0.5f;
			result.center.z = (bmax.sides[2].z + bmin.sides[2].z)*0.5f;

			result.sides[0].x = bmax.sides[0].x - result.center.x;
			result.sides[1].y = bmax.sides[1].y - result.center.y;
			result.sides[2].z = bmax.sides[2].z - result.center.z;


			bounding_boxes.push_back(result);


			bmin.sides[0].x = std::numeric_limits<float>::max();
			bmax.sides[0].x = -std::numeric_limits<float>::max();

			bmin.sides[1].y = std::numeric_limits<float>::max();
			bmax.sides[1].y = -std::numeric_limits<float>::max();

			bmin.sides[2].z = std::numeric_limits<float>::max();
			bmax.sides[2].z = -std::numeric_limits<float>::max();
		}
	}
	bounding_radius = max_radius;
}


std::vector<BoundingBox>* Model::getBoundingBoxes()
{
	return &bounding_boxes;
}

void Model::recursiveBuildTree(aiNode *ainode, Node* node)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			node->transform[i][j] = ainode->mTransformation[j][i];
		}

	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		unsigned int index = ainode->mMeshes[i];
		node->meshes.push_back(&meshes[index]);
	}

	node->children.resize(ainode->mNumChildren);
	for (int i = 0; i < ainode->mNumChildren; i++)
	{
		node->children[i] = new Node();
		recursiveBuildTree(ainode->mChildren[i], node->children[i]);
	}
}

void Model::recursiveFlatten(Node * node, glm::mat4 transform)
{
	glm::mat4 current_transform = transform * node->transform;

	//current_transform = glm::mat4();

	for (int i = 0; i < node->meshes.size(); i++) 
	{
		if (!node->meshes[i]->is_bb) 
		{
			auto pair = std::make_pair(node->meshes[i], current_transform);
			model_meshes.push_back(pair);
		}
		else 
		{
			buildBoundingBoxes(node->meshes[i], current_transform);
		}
	}

	for (int i = 0; i < node->children.size(); i++)
	{
		recursiveFlatten(node->children[i], current_transform);
	}
}

void Model::recursiveDeleteNodes(Node * node)
{
	if (node->children.size() > 0)
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			recursiveDeleteNodes(node->children[i]);
		}
		delete node;
	}
}