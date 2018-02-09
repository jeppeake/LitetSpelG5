#pragma once
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "soundcomponent.h"
#include "transform.h"
#include "previewcomponenet.h"
#include "timer.h"

struct RotatePreviewSystem : public entityx::System<RotatePreviewSystem> {
	entityx::ComponentHandle<PreviewComponent> preview;
	entityx::ComponentHandle<Transform> trans;

	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) {
		for (entityx::Entity entity : entities.entities_with_components(preview, trans)) {
			Transform newTrans = Transform(trans->pos,glm::angleAxis((float)dt, glm::vec3(0,1,0)));
			trans->orientation = trans->orientation * newTrans.orientation;
		}
	}
};