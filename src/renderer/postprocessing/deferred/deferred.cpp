#include "deferred.h"
#include <Imgui/imgui.h>

DeferredRenderer::DeferredRenderer()
{
	glGenFramebuffers(1, &gBuffer);
	glGenTextures(1, &gPosition);
	glGenTextures(1, &gNormals);
	glGenTextures(1, &gTangents);
	glGenTextures(1, &gAlbedo);
	glGenTextures(1, &gRoughMetalEmissive);
	glGenTextures(1, &gViewPosition);
	glGenTextures(1, &gTrueNormals);
	glGenRenderbuffers(1, &rboDepth);
}

void DeferredRenderer::buildDeferred(UpdateData& data)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// position
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal
	glBindTexture(GL_TEXTURE_2D, gNormals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormals, 0);

	// tangents
	glBindTexture(GL_TEXTURE_2D, gTangents);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gTangents, 0);

	// albedo
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAlbedo, 0);

	// rough, metal, and emissive properties
	glBindTexture(GL_TEXTURE_2D, gRoughMetalEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gRoughMetalEmissive, 0);

	// view position
	glBindTexture(GL_TEXTURE_2D, gViewPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gViewPosition, 0);

	// true normals
	glBindTexture(GL_TEXTURE_2D, gTrueNormals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, gTrueNormals, 0);

	unsigned int attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	glDrawBuffers(7, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, data.window->getWidth(), data.window->getHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int quadVAO5 = 0;
unsigned int quadVBO5;
void renderStaticQuad5()
{
	if (quadVAO5 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO5);
		glGenBuffers(1, &quadVBO5);
		glBindVertexArray(quadVAO5);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO5);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DeferredRenderer::drawDeferred(UpdateData& data, std::vector<Entity*>& entities)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, data.window->getWidth(), data.window->getHeight());

	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities[i]->draw(data, entities);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::draw(UpdateData& data, std::vector<Entity*>& entities, unsigned int SSAO)
{
	// draw to quad
	glViewport(0, 0, data.window->getWidth(), data.window->getHeight());
	Textures::bindTexture2D(gPosition, GL_TEXTURE0);
	Textures::bindTexture2D(gNormals, GL_TEXTURE1);
	Textures::bindTexture2D(gTangents, GL_TEXTURE2);
	Textures::bindTexture2D(gAlbedo, GL_TEXTURE3);
	Textures::bindTexture2D(gRoughMetalEmissive, GL_TEXTURE4);
	Textures::bindTexture2D(SSAO, GL_TEXTURE5);
	Textures::bindTexture2D(gTrueNormals, GL_TEXTURE6);

	data.shaders->deferredShader.bindProgram();
	data.shaders->deferredShader.setInts("positionTexture", 0);
	data.shaders->deferredShader.setInts("normalTexture", 1);
	data.shaders->deferredShader.setInts("tangentTexture", 2);
	data.shaders->deferredShader.setInts("albedoTexture", 3);
	data.shaders->deferredShader.setInts("roughMetalEmissiveTexture", 4);
	data.shaders->deferredShader.setInts("ssaoTexture", 5);
	data.shaders->deferredShader.setInts("trueNormalsTexture", 6);
	renderStaticQuad5();
}

void DeferredRenderer::drawUserInterface(UpdateData& data)
{
	ImGui::Begin("Deferred");

	ImGui::Text("View World Position");
	ImGui::Image((ImTextureID)gViewPosition, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("World Position");
	ImGui::Image((ImTextureID)gPosition, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("Normals");
	ImGui::Image((ImTextureID)gNormals, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("True Normals");
	ImGui::Image((ImTextureID)gTrueNormals, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("Tangents");
	ImGui::Image((ImTextureID)gTangents, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("Albedo");
	ImGui::Image((ImTextureID)gAlbedo, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Text("Roughness, Metallic, and Specular");
	ImGui::Image((ImTextureID)gRoughMetalEmissive, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}