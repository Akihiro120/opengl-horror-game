#pragma once
#include "../../../data/updateData.h"
#include "../../../entities/entity.h"

class ShadowRenderer
{
public:
	ShadowRenderer();

	void buildShadows(UpdateData& data);
	void drawShadowMapStatic(UpdateData& data, std::vector<Entity*>& entities);
	void drawShaderMap(UpdateData& data, std::vector<Entity*>& entities);

	glm::mat4 lightSpaceMatrix;
	//unsigned int depthCubemap;

private:
	//unsigned int depthMapFBO;
	//unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	float near_plane = 0.01f, far_plane = 50.0f;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
};