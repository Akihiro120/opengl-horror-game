#pragma once
#include "../data/updateData.h"
#include "../entities/entity.h"
#include "../renderer/postprocessing/pbr/pbr.h"
#include "../renderer/postprocessing/shadows/shadow.h"
#include "../renderer/postprocessing/postProcessing.h"
#include "../renderer/postprocessing/bloom/bloom.h"
#include "../renderer/postprocessing/deferred/deferred.h"
#include "../renderer/postprocessing/SSAO/ssao.h"
#include "../renderer/postprocessing/reflections/reflections.h"

class Scene
{
public:
	Scene(UpdateData& data);

	void update(UpdateData& data);
	void draw(UpdateData& data);

	void preLighting(UpdateData& data);

private:
	std::vector<Entity*> m_Entities;

	// rendering states
	PBRRenderer m_PBRRenderer{};
	ShadowRenderer m_ShadowRenderer{};
	PostProcessRenderer m_PostProcessRenderer;
	BloomRenderer m_BloomRenderer;
	DeferredRenderer m_DeferredRenderer;
	SSAORenderer m_SSAORenderer;
	ReflectionRenderer m_ReflectionRenderer;
};