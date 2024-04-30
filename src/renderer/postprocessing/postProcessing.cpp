#include "postProcessing.h"
#include "Imgui/imgui.h"

unsigned int quadVAO3 = 0;
unsigned int quadVBO3;
void renderQuad3()
{
	if (quadVAO3 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO3);
		glGenBuffers(1, &quadVBO3);
		glBindVertexArray(quadVAO3);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

PostProcessRenderer::PostProcessRenderer()
{
	glGenFramebuffers(1, &m_PostProcessFBO);
	glGenTextures(1, &m_ScreenTexture);
	glGenRenderbuffers(1, &rboDepth);
}

void PostProcessRenderer::buildPostProcess(UpdateData& data)
{
	std::cout << "build" << std::endl;

	// create floating point color buffer
	glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.window->getWidth(), data.window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// create depth buffer (renderbuffer)
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, data.window->getWidth(), data.window->getHeight());
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessRenderer::drawPostProcessing(UpdateData& data, std::vector<Entity*>& entities, DeferredRenderer* renderer, unsigned int SSAO)
{
	// record the screen
	glViewport(0, 0, data.window->getWidth(), data.window->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->draw(data, entities, SSAO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// perform bloom operations
}

void PostProcessRenderer::drawUserInterface()
{
	/*ImGui::Begin("Post Processing");

	ImGui::Text("Basic");
	ImGui::SliderFloat("Contrast", &contrast, 0.0f, 1.0f);
	ImGui::SliderFloat("Exposure", &exposure, 0.0f, 10.0f);
	ImGui::SliderFloat("Gamma", &gamma, 0.0f, 10.0f);

	ImGui::End();*/
}