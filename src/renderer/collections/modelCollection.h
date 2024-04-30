#pragma once
#include "../model/model.h"

class ModelCollection
{
public:
	Model cubeModel{ "resources/models/cube/Cube.obj" };
	Model sphereModel{ "resources/models/sphere/sphere.obj" };
	Model safetyDoorModel{ "resources/models/safetyDoor/safetyDoor.obj" };
	Model safetyDoorColliderModel{ "resources/models/safetyDoor/safetyDoorCollider.obj" };
	Model safetyDoorHandleModel{ "resources/models/safetyDoor/safetyDoorHandle.obj" };
	Model woodDoorModel{ "resources/models/woodDoor/woodDoor.obj" };
	Model doorKeyModel{ "resources/models/woodDoor/doorKey.obj" };
	Model cageLightModel{ "resources/models/light/cageLight.obj" };
	Model waterPlaneModel{ "resources/models/water/waterPlane.obj" };

	// maps
	Model buildingModel{ "resources/models/buildings/buildings.obj" };
	Model buildingModel2{ "resources/models/buildings/building2.obj" };
};