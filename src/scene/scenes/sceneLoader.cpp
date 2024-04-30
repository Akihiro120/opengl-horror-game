#include "sceneLoader.h"
#include "../../entities/actors/quad/quad.h"
#include "../../entities/actors/cube/cube.h"
#include "../../entities/actors/camera/camera.h"
#include "../../entities/actors/lightCube/lightCube.h"
#include "../../entities/actors/safetyDoor/door/door.h"
#include "../../entities/actors/safetyDoor/switch/doorSwitch.h"
#include "../../entities/actors/item/test/itemCube.h"
#include "../../entities/actors/woodDoor/woodDoor.h"
#include "../../entities/actors/woodDoor/key/doorKey.h"
#include "../../renderer/postprocessing/pbr/equirectangular/equirectangular.h"
#include "../../entities/actors/pbrDemo/pbrDemo.h"
#include "../../entities/actors/building/building.h"
#include "../../entities/actors/building/2/building2.h"
#include "../../entities/actors/water/water.h"
#include "../../entities/actors/sceneDoor/sceneDoor.h"

void SceneLoader::SCENE_visual(UpdateData& data, std::vector<Entity*>& entities)
{
	// clear
	//entities.clear();
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->destroyActor();
	}

	// add
	entities.push_back(new Camera(Transform{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, data));

	entities.push_back(new LightCube(Transform{ glm::vec3(-1.8f, 4.4f, 4.98917f), glm::vec3(0.0f, 180.0f, 135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-0.8f, 4.4f, 41.8), glm::vec3(0.0f, -90.0f, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(9.9f, 6.49108f, 29.9406f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-19.892f, 6.49108f, 29.9406), glm::vec3(0.0f, 180.0f, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-14.8384f, 4.46241f, 41.747f), glm::vec3(0.0f, -90, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-14.8384, 6.46241f, 18.2961f), glm::vec3(0.0f, 90, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(0.00f, 6.46241f, 18.2961f), glm::vec3(0.0f, 90, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));

	entities.push_back(new Building(Transform{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f) }));

	entities.push_back(new SafetyDoor(Transform{ glm::vec3(0.0f, 0.0f, 18.0f), glm::vec3(0.0f, 90.0f, 0.0f) }, data, 0));
	entities.push_back(new SafetyDoorSwitch(Transform{ glm::vec3(-2.5f, 2.25f, 15.0f), glm::vec3(0.0f) }, data, 0));

	entities.push_back(new SceneDoor(Transform{ glm::vec3(0.0f, 0.0f, 42.0f), glm::vec3(0.0f, 90.0f, 0.0f) }, data, 1, "None"));

	entities.push_back(new Water(Transform{ glm::vec3(0.0f, -2.0f, 29.0f), glm::vec3(0.0f, 0.0f, 0.0f) }));
}

void SceneLoader::SCENE_visual2(UpdateData& data, std::vector<Entity*>& entities)
{
	// clear
	//entities.clear();
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->destroyActor();
	}
	for (int i = 0; i < data.shaders->pointLights.size(); i++)
	{
		data.shaders->pointLights[i].destroy = true;
	}

	// add
	entities.push_back(new Camera(Transform{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, data));

	entities.push_back(new LightCube(Transform{ glm::vec3(2.0f, 4.0f, -12.0f), glm::vec3(0.0f, 180.0f, -135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-32.0f, 4.0f, -12.0f), glm::vec3(0.0f, 0.0f, -135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-32.0f, 4.0f, -30.0f), glm::vec3(0.0f, 0.0f, -135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-9.60796f, 4.0f, -30.0f), glm::vec3(0.0f, 180.0f, -90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-32.0f, 4.0f, -42.0f), glm::vec3(0.0f, 0.0f, -135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-18.0f, 4.0f, -63.0f), glm::vec3(0.0f, -90.0f, -90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(14.42f, 4.0f, -30.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(-0.0919f, 4.0f, -32.1199f), glm::vec3(0.0f, -90.0f, -135.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));
	entities.push_back(new LightCube(Transform{ glm::vec3(14.8391f, 7.57605f, -39.171f), glm::vec3(0.0f, -180.0f, -90.0f), glm::vec3(0.5f) }, glm::vec3(255.0f / 255.0, 191.0f / 255.0, 41.0f / 255.0), data, true));

	entities.push_back(new Building2(Transform{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f) }));

	entities.push_back(new SafetyDoor(Transform{ glm::vec3(0.0f, 0.0f, -21.0f), glm::vec3(0.0f, -90.0f, 0.0f) }, data, 0));
	entities.push_back(new SafetyDoor(Transform{ glm::vec3(-18.0f, 0.0f, -50.0f), glm::vec3(0.0f, -90.0f, 0.0f) }, data, 1));

	data.window->rebuildWindow = true;
}