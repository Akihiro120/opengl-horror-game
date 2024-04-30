#pragma once
#include "../shaders/shader.h"
#include "../../data/transform.h"
#include "../../data/light.h"

#include <vector>
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

class ShaderCollection
{
public:
	ShaderCollection()
	{
		// build spotlights, pointlights
		//pointLights.push_back(PointLight{ glm::vec3(-5.0f, 8.0f, 5.0f), glm::vec3(20.0f) });
		//pointLights.push_back(PointLight{ glm::vec3(-5.0f, 8.0f, -5.0f), glm::vec3(20.0f) });
		//pointLights.push_back(PointLight{ glm::vec3(5.0f, 8.0f, 5.0f), glm::vec3(20.0f) });
		//pointLights.push_back(PointLight{ glm::vec3(5.0f, 8.0f, -5.0f), glm::vec3(20.0f) });
	}

	// shaders
	Shader defaultShader{ "resources/shaders/default.vert", "resources/shaders/deferred/gBuffer.frag" };
	Shader lightShader{ "resources/shaders/default.vert", "resources/shaders/light/light.frag" };
	Shader equirectangularShader{ "resources/shaders/pbr/equirectangular/equirectangular.vert", "resources/shaders/pbr/equirectangular/equirectangular.frag" };
	Shader skyboxShader{ "resources/shaders/pbr/skybox/skybox.vert", "resources/shaders/pbr/skybox/skybox.frag" };
	Shader irradianceShader{ "resources/shaders/pbr/equirectangular/equirectangular.vert", "resources/shaders/pbr/irradiance/irradiance.frag" };
	Shader prefilterShader{ "resources/shaders/pbr/equirectangular/equirectangular.vert", "resources/shaders/pbr/prefilter/prefilter.frag" };
	Shader brdfShader{ "resources/shaders/pbr/brdf/brdf.vert", "resources/shaders/pbr/brdf/brdf.frag" };
	Shader quadShader{ "resources/shaders/quad/quad.vert", "resources/shaders/quad/quad.frag" };
	Shader depthMapShader{ "resources/shaders/shadows/depthMap.vert", "resources/shaders/shadows/depthMap.frag" };
	Shader depthMapPointShader{ "resources/shaders/shadows/pointLights/depthMap.vert" ,"resources/shaders/shadows/pointLights/depthMap.frag", "resources/shaders/shadows/pointLights/depthMap.geom"};
	Shader depthVisualShader{ "resources/shaders/quad/quad.vert", "resources/shaders/shadows/visual/depthVisual.frag" };
	Shader postProcessShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postProcessing/postProcess.frag" };

	Shader downSamplingShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postProcessing/bloom/sampling/downSampling.frag" };
	Shader upSamplingShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postProcessing/bloom/sampling/upSampling.frag" };
	Shader bloomShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postProcessing/bloom/bloom.frag" };
	Shader deferredShader{ "resources/shaders/quad/quad.vert", "resources/shaders/default.frag" };

	Shader ssaoShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postprocessing/ssao/ssao.frag" };
	Shader ssaoBlurShader{ "resources/shaders/quad/quad.vert", "resources/shaders/postprocessing/ssao/ssaoBlur.frag" };

	void drawUserInterface()
	{
		ImGui::Begin("Graphics Settings");
		ImGui::Checkbox("Enable PBR Renderer", &enablePBR);
		ImGui::Checkbox("Enable Shadow Renderer", &enableShadows);
		ImGui::Checkbox("Enable Bloom Renderer", &enableBloom);
		ImGui::Checkbox("Enable Post Processing Renderer", &enablePostProcessing);
		ImGui::Checkbox("Enable Fog Renderer", &enableFog);
		ImGui::Checkbox("Enable SSAO Renderer", &enableSSAO);
		ImGui::Checkbox("Enable Micro Variation", &enableParallax);
		ImGui::Checkbox("Enable Reflections", &enableReflections);
		ImGui::End();
	}

	void createCameraMatrix(int width, int height, Transform cameraTransform)
	{
		// create perspective matrix
		float fov = 45.0f;
		float aspect = (float)width / (float)height;
		float far = 100.0f;
		float near = 0.1f; 

		perspective = glm::mat4(1.0f);
		perspective = glm::perspective(glm::radians(fov), aspect, near, far);

		// create view matrix
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		
		view = glm::mat4(1.0f);
		view = glm::lookAt(cameraTransform.location, cameraTransform.location + cameraTransform.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void bindLightingPropertiesToShader(Shader* shader, Transform transform, 
		unsigned int irradianceMap, unsigned int prefilter, unsigned int brdf,
		glm::mat4 lightSpaceMatrix)
	{	

		// destroy lights
		for (int i = 0; i < pointLights.size(); i++)
		{
			if (pointLights[i].destroy)
			{
				// destroy framebuffers and textures
				glDeleteFramebuffers(1, &pointLights[i].shadowFBO);
				glDeleteTextures(1, &pointLights[i].shadowMap);
				pointLights.erase(pointLights.begin() + i);
			}
		}

		shader->bindProgram();
		if (pointLights.size() > 0)
		{
			shader->setInts("numOfPointLights", pointLights.size());

			// spotlight

			// pointlights
			for (int i = 0; i < pointLights.size(); i++)
			{
				if (!pointLights[i].destroy)
				{
					shader->setVec3(std::string("pointLights[" + std::to_string(i) + "].position").c_str(), pointLights[i].position);
					shader->setVec3(std::string("pointLights[" + std::to_string(i) + "].color").c_str(), pointLights[i].color);

					glActiveTexture(GL_TEXTURE10 + i);
					glBindTexture(GL_TEXTURE_CUBE_MAP, pointLights[i].shadowMap);
					shader->setInts(std::string("pointLights[" + std::to_string(i) + "].shadowMap").c_str(), 10 + i);
					shader->setInts(std::string("pointLights[" + std::to_string(i) + "].renderShadows").c_str(), pointLights[i].castShadows);

					shader->setFloats(std::string("pointLights[" + std::to_string(i) + "].radius").c_str(), pointLights[i].radius);
				}
			}
			for (int i = pointLights.size(); i < 16; i++)
			{
				shader->setVec3(std::string("pointLights[" + std::to_string(i) + "].position").c_str(), glm::vec3(0.0f));
				shader->setVec3(std::string("pointLights[" + std::to_string(i) + "].color").c_str(), glm::vec3(0.0f));

				glActiveTexture(GL_TEXTURE10 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				shader->setInts(std::string("pointLights[" + std::to_string(i) + "].shadowMap").c_str(), 10 + i);
				shader->setInts(std::string("pointLights[" + std::to_string(i) + "].renderShadows").c_str(), false);

				shader->setFloats(std::string("pointLights[" + std::to_string(i) + "].radius").c_str(), 0);
			}
		}

		// bind pbr maps
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		shader->setInts("irradianceMap", 7);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter);
		shader->setInts("prefilterMap", 8);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, brdf);
		shader->setInts("brdfLUT", 9);

		shader->setFloats("far_plane", 50.0f);

		shader->setVec3("viewPosition", transform.location);

		// bind the enable data to the shader to stop or start calculations
		shader->setInts("useShadows", enableShadows);
		shader->setInts("useSSAO", enableSSAO);
		shader->setInts("useFog", enableFog);
		shader->setInts("usePBR", enablePBR);
	}

	void bindMatrixToShader(Shader* shader)
	{
		shader->bindProgram();
		shader->setMat4("projection", perspective);
		shader->setMat4("view", view);
		shader->setInts("useParallax", enableParallax);
	}

	glm::mat4 getPerspectiveMatrix()
	{
		return perspective;
	}

	glm::mat4 getViewMatrix()
	{
		return view;
	}

	int addSpotLight(SpotLight light)
	{
		spotLights.push_back(light);

		return spotLights.size() - 1;
	}

	int addPointLight(PointLight light)
	{
		float constant = 1.0;
		float linear = 0.7;
		float quadratic = 1.8;
		float lightMax = std::fmaxf(std::fmaxf(light.color.x, light.color.y), light.color.z);
		float radius =
			(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
			/ (2 * quadratic);

		PointLight& newLight = light;
		newLight.radius = radius;
		pointLights.push_back(newLight);
		//std::cout << light.position.x << light.position.y << light.position.z << std::endl;

		return pointLights.size() - 1;
	}

	void setPointLight(PointLight light, int index)
	{
		pointLights[index] = light;
	}

	std::vector<SpotLight> getSpotLights()
	{
		return spotLights;
	}

	std::vector<PointLight> getPointLights()
	{
		return pointLights;
	}

	std::vector<SpotLight> spotLights;
	std::vector<PointLight> pointLights;

	// settings
	bool enableShadows = true;
	bool enablePBR = true;
	bool enableBloom = true;
	bool enableSSAO = true;
	bool enablePostProcessing = true;
	bool enableFog = true;
	bool enableParallax = true;
	bool enableReflections = true;

private:
	glm::mat4 perspective = glm::mat4(0.0f);
	glm::mat4 view = glm::mat4(0.0f);

	// lights
	SpotLight flashlight;
};