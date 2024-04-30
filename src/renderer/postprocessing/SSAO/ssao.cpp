#include "ssao.h"
#include <random>
#include "Imgui/imgui.h"

static float ourLerp(float a, float b, float f)
{
	return a + f * (b - a);
}

unsigned int quadVAO6 = 0;
unsigned int quadVBO6;
void renderStaticQuad6()
{
	if (quadVAO6 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO6);
		glGenBuffers(1, &quadVBO6);
		glBindVertexArray(quadVAO6);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO6);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

SSAORenderer::SSAORenderer()
{
	glGenFramebuffers(1, &ssaoFBO); glGenFramebuffers(1, &ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBuffer);
	glGenTextures(1, &ssaoColorBufferBlur);
	glGenTextures(1, &noiseTexture);
}

void SSAORenderer::buildSSAO(UpdateData& data)
{
	if (data.shaders->enableSSAO)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

		// ssao color buffer
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, data.window->getWidth(), data.window->getHeight(), 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, data.window->getWidth(), data.window->getHeight(), 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 16; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / 16.0f;

			// scale samples s.t. they're more aligned to center of kernel
			scale = ourLerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void SSAORenderer::drawSSAO(UpdateData& data, unsigned int gPosition, unsigned int gNormal)
{
	if (data.shaders->enableSSAO)
	{
		glViewport(0, 0, data.window->getWidth(), data.window->getHeight());

		// create ssao
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

		glClear(GL_COLOR_BUFFER_BIT);
		data.shaders->ssaoShader.bindProgram();
		for (unsigned int i = 0; i < 16; ++i)
		{
			data.shaders->ssaoShader.setVec3(std::string("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		data.shaders->ssaoShader.setInts("gPosition", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		data.shaders->ssaoShader.setInts("gNormal", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		data.shaders->ssaoShader.setInts("texNoise", 2);

		data.shaders->ssaoShader.setMat4("projection", data.shaders->getPerspectiveMatrix());
		data.shaders->ssaoShader.setVec2("scrDimensions", glm::vec2(data.window->getWidth(), data.window->getHeight()));

		renderStaticQuad6();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// blur ssao
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		data.shaders->ssaoBlurShader.bindProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		data.shaders->ssaoBlurShader.setInts("ssaoInput", 0);

		renderStaticQuad6();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}   

void SSAORenderer::drawUserInterface(UpdateData& data)
{
	ImGui::Begin("Deferred");

	ImGui::Text("SSAO");
	ImGui::Image((ImTextureID)ssaoColorBufferBlur, ImVec2(256 * 4, 144 * 4), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}