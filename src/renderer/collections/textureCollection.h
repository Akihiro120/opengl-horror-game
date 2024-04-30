#pragma once
#include "../textures/textures.h"

class TextureCollection
{
public:
	unsigned int brick_ALBEDO = Textures::loadTexture2D("resources/textures/plaster/bricks_ALBEDO.jpg", true, false);
	unsigned int brick_NORMALS = Textures::loadTexture2D("resources/textures/plaster/bricks_NORMAL.jpg", false, false);

	unsigned int brickPipe_ALBEDO = Textures::loadTexture2D("resources/textures/bricks/bricks_ALBEDO.jpg", true, false);
	unsigned int brickPipe_NORMALS = Textures::loadTexture2D("resources/textures/bricks/bricks_NORMAL.jpg", false, false);

	unsigned int iron_ALBEDO = Textures::loadTexture2D("resources/textures/iron/iron_ALBEDO.jpg", true, false);
	unsigned int iron_NORMALS = Textures::loadTexture2D("resources/textures/iron/iron_NORMAL.jpg", false, false);

	unsigned int plank_ALBEDO = Textures::loadTexture2D("resources/textures/woodPlanks/planks_ALBEDO.jpg", true, false);
	unsigned int plank_NORMALS = Textures::loadTexture2D("resources/textures/woodPlanks/planks_NORMAL.jpg", false, false);

	unsigned int concrete_ALBEDO = Textures::loadTexture2D("resources/textures/concrete/concrete_ALBEDO.jpg", true, false);
	unsigned int concrete_NORMALS = Textures::loadTexture2D("resources/textures/concrete/concrete_NORMAL.jpg", false, false);

	unsigned int rust_ALBEDO = Textures::loadTexture2D("resources/textures/rust/rust_ALBEDO.jpg", true, false);
	unsigned int rust_NORMALS = Textures::loadTexture2D("resources/textures/rust/rust_NORMAL.jpg", false, false);

	unsigned int steel_ALBEDO = Textures::loadTexture2D("resources/textures/steel/steel_ALBEDO.jpg", true, false);
	unsigned int steel_NORMALS = Textures::loadTexture2D("resources/textures/steel/steel_NORMAL.jpg", false, false);

	unsigned int macro_VARIATION = Textures::loadTexture2D("resources/textures/macroVariation.png", false, false);
	unsigned int water_NORMAL = Textures::loadTexture2D("resources/textures/water/water_NORMAL.jpg", false, false);
	unsigned int DUDV = Textures::loadTexture2D("resources/textures/water/DuDv.png", false, false);

	// sky
	unsigned int hdriMap = Textures::loadHDR("resources/hdri/hdri.hdr");
	//unsigned int hdriMap = Textures::loadTexture2D("resources/hdri/ibl_hdr_radiance.png", false, true);

	// pbr
	unsigned int irradianceMap;
	unsigned int prefilteredMap;
	unsigned int brdfLUTMap;
	unsigned int shadowMap;

	unsigned int reflectTexture;

	void loadBrickPipeMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(brickPipe_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(brickPipe_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setInts("microTexture", 2);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("metallic", 0.01f);
		shader->setFloats("roughness", 0.3f);
	}

	void loadSteelMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(steel_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(steel_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setInts("microTexture", 2);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("roughness", 0.2f);
		shader->setFloats("metallic", 0.8f);
	}

	void loadRustMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(rust_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(rust_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);
		shader->setInts("microTexture", 2);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("roughness", 0.4f);
		shader->setFloats("metallic", 0.7f);
	}

	void loadConcreteMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(concrete_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(concrete_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);
		shader->setInts("microTexture", 2);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("metallic", 0.01f);
		shader->setFloats("roughness", 0.3f);
	}

	void loadBrickMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(brick_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(brick_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);
		shader->setInts("microTexture", 2);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("metallic", 0.01f);
		shader->setFloats("roughness", 0.3f);
	}

	void loadIronMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(iron_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(iron_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setInts("parallaxTexture", 2);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 0.1f);

		shader->setFloats("roughness", 0.1f);
		shader->setFloats("metallic", 1.0f);
	}

	void loadPlankMaterial(Shader* shader, float tiling)
	{
		Textures::bindTexture2D(plank_ALBEDO, GL_TEXTURE0);
		Textures::bindTexture2D(plank_NORMALS, GL_TEXTURE1);
		Textures::bindTexture2D(macro_VARIATION, GL_TEXTURE2);

		shader->setInts("useAlbedoTexture", 1);
		shader->setInts("useNormalTexture", 1);

		shader->setInts("albedoTexture", 0);
		shader->setInts("normalTexture", 1);
		shader->setInts("parallaxTexture", 2);
		shader->setFloats("tiling", tiling);
		shader->setFloats("heightScale", 2.0f);

		shader->setFloats("roughness", 0.5f);
		shader->setFloats("metallic", 0.01f);
	}
};