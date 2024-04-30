#pragma once
#include "../../../data/updateData.h"
#include "../../../data/bloomMip.h"

class BloomRenderer
{
public:
	BloomRenderer();
	void buildBloom(UpdateData& data);
	void drawBloom(UpdateData& data, unsigned int srcTexture);
	void drawBloomFilter(UpdateData, unsigned int srcTexture);
	void drawToFinal(UpdateData& data, unsigned int srcTexture);

	void drawUserInterface();

private:

	void renderDownsamples(UpdateData& data, unsigned int srcTexture);
	void renderUpsamples(UpdateData& data, float filterRadius);

	void drawMipSamples(UpdateData& data);
	void drawBloomRenderer(UpdateData& data);
	
	// bloom mip generation
	unsigned int m_BloomFilterFBO;
	unsigned int m_BloomFilterTexture;

	unsigned int m_BloomFBO;
	std::vector<BloomMip> m_MipChain;
	int m_MipChainLength = 5;

	// rendering
	glm::ivec2 m_ViewportSize;
	glm::vec2 m_ViewportSizeFloat;
	float m_FilterRadius = 0.002f;

	// settings
	float exposure = 5.0f;
	float gamma = 2.2f;
	float contrast = 0.05f;
	float bloomStrength = 0.035f;
};