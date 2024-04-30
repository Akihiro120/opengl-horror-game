#pragma once
#include "../../entities/entity.h"

class SceneLoader
{
public:
	static void SCENE_visual(UpdateData& data, std::vector<Entity*>& entities);
	static void SCENE_visual2(UpdateData& data, std::vector<Entity*>& entities);
};