#pragma once
#include "../../../data/updateData.h"
#include "../bloom/bloom.h"
#include "../postProcessing.h"

class ReflectionRenderer
{
public:
	ReflectionRenderer();
	void buildReflections(UpdateData& data);

	void drawReflections(UpdateData& data, BloomRenderer* bloom, PostProcessRenderer* postProcess);
	void drawUserInterface(UpdateData& data);

	unsigned int m_ReflectionFBO;
	unsigned int m_ReflectionTexture;
	unsigned int rboDepth;
};