#include "scene.h"

#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "scenes/sceneLoader.h"
#include "../entities/actors/camera/camera.h"

Scene::Scene(UpdateData& data)
{
	// scene loading
	SceneLoader::SCENE_visual(data, m_Entities);

	// pre lighting
	preLighting(data);
}

void Scene::preLighting(UpdateData& data)
{
	// draw the shadow map for static lights
	m_PBRRenderer.buildPBR(data);
	m_ShadowRenderer.buildShadows(data);
	m_PostProcessRenderer.buildPostProcess(data);
	m_BloomRenderer.buildBloom(data);
	m_DeferredRenderer.buildDeferred(data);
	m_SSAORenderer.buildSSAO(data);
	m_ReflectionRenderer.buildReflections(data);
}

void Scene::update(UpdateData& data)
{
	// update entities
	for (unsigned int i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i]->update(data, m_Entities);
		if (m_Entities[i]->getDestroyState())
		{
			m_Entities.erase(m_Entities.begin() + i);
		}
	}

	// window rebuild
	if (data.window->rebuildWindow)
	{
		m_ShadowRenderer.buildShadows(data);
		m_PostProcessRenderer.buildPostProcess(data);
		m_BloomRenderer.buildBloom(data);
		m_DeferredRenderer.buildDeferred(data);
		m_SSAORenderer.buildSSAO(data);
		m_ReflectionRenderer.buildReflections(data);
	}
}

void Scene::draw(UpdateData& data)
{
	// update matrix
	data.shaders->createCameraMatrix(data.window->getWidth(), data.window->getHeight(), m_Entities[0]->getTransform());

	data.shaders->bindMatrixToShader(&data.shaders->defaultShader); 
	data.shaders->bindMatrixToShader(&data.shaders->deferredShader);
	//data.shaders->bindMatrixToShader(&data.shaders->lightShader);

	data.shaders->bindLightingPropertiesToShader(&data.shaders->deferredShader, m_Entities[0]->getTransform(),
		data.textures->irradianceMap,
		data.textures->prefilteredMap,
		data.textures->brdfLUTMap,
		m_ShadowRenderer.lightSpaceMatrix);

	////////////////////////////////////////////////////////////////////

	/* Order
		- GBuffer
		- Deferred + Forward
		- Shadows
		- Post Process
		- Bloom
		- Finalise
	*/

	m_ShadowRenderer.drawShaderMap(data, m_Entities);
	m_SSAORenderer.drawSSAO(data, m_DeferredRenderer.gViewPosition, m_DeferredRenderer.gNormals);

	if (data.shaders->enableReflections)
	{
		glEnable(GL_CLIP_DISTANCE0);
		data.shaders->defaultShader.bindProgram();
		data.shaders->defaultShader.setVec4("plane", glm::vec4(0.0f, 1.0f, 0.0f, 1.995f));

		// start
		Transform transform{ glm::vec3(0.0f), glm::vec3(0.0f) };
		Camera* cameraReference = dynamic_cast<Camera*>(m_Entities[0]);
		if (cameraReference)
		{
			transform = cameraReference->getTransform();
			float distance = 2.0f * (transform.location.y - (-2.0f));
			transform.location.y -= distance;
			transform.rotation = cameraReference->calculateDirection(cameraReference->m_Yaw, -cameraReference->m_Pitch);
		}

		data.shaders->createCameraMatrix(data.window->getWidth(), data.window->getHeight(), transform);
		data.shaders->bindMatrixToShader(&data.shaders->defaultShader);

		m_DeferredRenderer.drawDeferred(data, m_Entities);
		m_PostProcessRenderer.drawPostProcessing(data, m_Entities, &m_DeferredRenderer, m_SSAORenderer.ssaoColorBufferBlur);
		m_BloomRenderer.drawBloom(data, m_PostProcessRenderer.m_ScreenTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ReflectionRenderer.m_ReflectionFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_BloomRenderer.drawToFinal(data, m_PostProcessRenderer.m_ScreenTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		data.shaders->createCameraMatrix(data.window->getWidth(), data.window->getHeight(), m_Entities[0]->getTransform());
		data.shaders->bindMatrixToShader(&data.shaders->defaultShader);

		data.textures->reflectTexture = m_ReflectionRenderer.m_ReflectionTexture;
	}

	glDisable(GL_CLIP_DISTANCE0);
	m_DeferredRenderer.drawDeferred(data, m_Entities);
	m_PostProcessRenderer.drawPostProcessing(data, m_Entities, &m_DeferredRenderer, m_SSAORenderer.ssaoColorBufferBlur);
	m_BloomRenderer.drawBloom(data, m_PostProcessRenderer.m_ScreenTexture);
	m_BloomRenderer.drawToFinal(data, m_PostProcessRenderer.m_ScreenTexture);

	////////////////////////////////////////////////////////////////////

	// gui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// app
	ImGui::Begin("Application");
	ImGui::Text("Application Average %.3f ms/frame (%.f FPS))", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::SliderFloat3("Player Location", &m_Entities[0]->getTransform().location.x, 0.0f, 1.0f);
	ImGui::End();

	m_DeferredRenderer.drawUserInterface(data);
	m_BloomRenderer.drawUserInterface();
	m_SSAORenderer.drawUserInterface(data);
	m_ReflectionRenderer.drawUserInterface(data);
	data.shaders->drawUserInterface();
	for (unsigned int i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i]->userInterface(data, m_Entities);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}