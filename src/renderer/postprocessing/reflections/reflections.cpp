#include "reflections.h"

ReflectionRenderer::ReflectionRenderer()
{
	glGenFramebuffers(1, &m_ReflectionFBO);
	glGenTextures(1, &m_ReflectionTexture);
	glGenRenderbuffers(1, &rboDepth);
}

void ReflectionRenderer::buildReflections(UpdateData& data)
{
	// create floating point color buffer
	glBindTexture(GL_TEXTURE_2D, m_ReflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// create depth buffer (renderbuffer)
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, data.window->getWidth(), data.window->getHeight());

	glBindFramebuffer(GL_FRAMEBUFFER, m_ReflectionFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ReflectionTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReflectionRenderer::drawReflections(UpdateData& data, BloomRenderer* bloom, PostProcessRenderer* postProcess)
{

}

void ReflectionRenderer::drawUserInterface(UpdateData& data)
{
	ImGui::Begin("Deferred");

	ImGui::Text("Reflectance Plane");
	ImGui::Image((ImTextureID)m_ReflectionTexture, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}