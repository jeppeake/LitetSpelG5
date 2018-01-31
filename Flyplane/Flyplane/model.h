#pragma once

#include <string>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <assimp/vector3.h>
#include <assimp/vector2.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "texture.h"
#include "boundingbox.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;

	Vertex() {}

	Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) : position(pos), normal(norm), tex_coords(tex)
	{
	}

	Vertex(aiVector3D pos, aiVector3D norm, aiVector2D texs)
	{
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;

		normal.x = norm.x;
		normal.y = norm.y;
		normal.z = norm.z;

		tex_coords.x = texs.x;
		tex_coords.y = texs.y;
	}
};



class Model {
	class Mesh
	{
		
		GLuint ebo = 0;
		GLuint vbo = 0;
		GLuint vao = 0;
		unsigned int material_index;
	public:
		std::vector<GLuint> indices;
		std::vector<Vertex> vertices;

		bool is_bb = false;

		std::string name = "";

		Mesh(aiMesh* mesh);
		Mesh(const std::vector<Vertex>& vertices,
			const std::vector<GLuint>& indices);
		~Mesh();

		size_t numIndices()
		{
			return indices.size();
		}
		unsigned int getMaterialIndex()
		{
			return material_index;
		}
		void bind();
		void unbind();
	};

	struct Node
	{
		// meshes taken from the vector<Mesh> in Model
		std::vector<Mesh*> meshes;

		std::vector<Node*> children;

		std::string name;

		// relative to parent
		glm::mat4 transform;
	};

	void recursiveBuildTree(aiNode *ainode, Node* node);
	void recursiveFlatten(Node* node, glm::mat4 transform);
	void recursiveDeleteNodes(Node* node);


	void buildBoundingBoxes(Mesh* mesh, glm::mat4 transform);

	std::vector<Mesh> meshes;

	float bounding_radius;
	
	std::vector<BoundingBox> bounding_boxes;

	bool loaded = false;

public:
	Texture texture;
	//std::vector<Material> materials;
	std::vector<std::pair<Mesh*, glm::mat4>> model_meshes;
	void load(const std::string& file);

	std::vector<BoundingBox>* getBoundingBoxes();
	inline float getBoundingRadius() {
		return bounding_radius;
	}

};