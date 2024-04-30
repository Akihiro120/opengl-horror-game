#pragma once
#include "../../../data/updateData.h"

class PBRRenderer
{
public:
	PBRRenderer();

	void buildPBR(UpdateData& data);
	void drawPBR(UpdateData& data);

private:
	void createEquirectangularMap(UpdateData& data);
	void createIrradianceMap(UpdateData& data);
	void createPrefilterMap(UpdateData& data);
	void createBRDF(UpdateData& data);

	unsigned int captureFBO, captureRBO;
	unsigned int envCubemap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;

	glm::mat4 captureProjection;
	glm::mat4 captureViews[6];
};