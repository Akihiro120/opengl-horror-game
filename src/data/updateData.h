#pragma once
#include "../system/window/window.h"
#include "../renderer/collections/shaderCollection.h"
#include "../renderer/collections/modelCollection.h"
#include "../renderer/collections/textureCollection.h"

struct UpdateData
{
	Window* window;
	ShaderCollection* shaders;
	ModelCollection* models;
	TextureCollection* textures;

	bool drawUsingCurrentMaterial = true;
};