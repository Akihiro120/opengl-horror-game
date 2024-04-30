#pragma once
#include "../../data/updateData.h"
#include "../../entities/entity.h"
#include "deferred/deferred.h"

class PostProcessRenderer
{
public:
	PostProcessRenderer();
	void buildPostProcess(UpdateData& data);

	void drawPostProcessing(UpdateData& data, std::vector<Entity*>& entities, DeferredRenderer* renderer, unsigned int SSAO);
	void drawUserInterface();

	unsigned int m_ScreenTexture;
private:
	unsigned int m_PostProcessFBO;
	unsigned int rboDepth;

	// settings
	float exposure = 5.0f;
	float gamma = 2.2f;
	float contrast = 0.05f;
};