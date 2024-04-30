#include "item.h"
#include "../camera/camera.h"

ItemActor::ItemActor(Transform transform) : Entity(transform)
{

}

void ItemActor::onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID)
{
	Camera* cameraReference = dynamic_cast<Camera*>(entities[0]);
	if (cameraReference)
	{
		cameraReference->addItemToInventory(currentItem);

		destroyActor();
	}
}