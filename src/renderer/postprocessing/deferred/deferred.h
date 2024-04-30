#pragma once
#include "../../../data/updateData.h"
#include "../../../entities/entity.h"

class DeferredRenderer
{
public:
	DeferredRenderer();
	void buildDeferred(UpdateData& data);
	void drawDeferred(UpdateData& data, std::vector<Entity*>& entities);
	void draw(UpdateData& data, std::vector<Entity*>& entities, unsigned int SSAO);
	void drawUserInterface(UpdateData& data);

	unsigned int gBuffer;
	unsigned int gPosition, gNormals, gTangents, gAlbedo, gRoughMetalEmissive, gViewPosition, gTrueNormals;

	unsigned int rboDepth;
	/*
	- Albedo - vec3
	- Normal - vec3
	- Tangents - vec3
	- MetalRoughnessEmmisive - r, g, b, a
	- World Position - vec3
	*/
};