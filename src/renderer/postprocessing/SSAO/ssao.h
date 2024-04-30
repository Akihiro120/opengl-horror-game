#pragma once
#include "../../../data/updateData.h"

class SSAORenderer
{
public:
	SSAORenderer();
	void buildSSAO(UpdateData& data);
	void drawSSAO(UpdateData& data, unsigned int gPosition, unsigned int gNormal);
	void drawUserInterface(UpdateData& data);

	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	unsigned int noiseTexture;
	std::vector<glm::vec3> ssaoKernel;
};