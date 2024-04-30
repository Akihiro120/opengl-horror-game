#include "bloom.h"
#include "Imgui/imgui.h"

BloomRenderer::BloomRenderer()
{
	glGenFramebuffers(1, &m_BloomFBO);
}

void BloomRenderer::buildBloom(UpdateData & data)
{
	if (data.shaders->enableBloom)
	{
		std::cout << "Build Bloom Filter" << std::endl;

		drawMipSamples(data);
		drawBloomRenderer(data);
	}
}

void BloomRenderer::drawBloom(UpdateData& data, unsigned int srcTexture)
{
	if (data.shaders->enableBloom)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFBO);

		renderDownsamples(data, srcTexture);
		renderUpsamples(data, m_FilterRadius);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// restore framebuffer
		glViewport(0, 0, data.window->getWidth(), data.window->getHeight());
	}
}

unsigned int quadVAO4 = 0;
unsigned int quadVBO4;
void renderStaticQuad4()
{
	if (quadVAO4 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO4);
		glGenBuffers(1, &quadVBO4);
		glBindVertexArray(quadVAO4);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO4);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO4);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void BloomRenderer::renderDownsamples(UpdateData& data, unsigned int srcTexture)
{
	const std::vector<BloomMip>& mipChain = m_MipChain;

	data.shaders->downSamplingShader.bindProgram();
	data.shaders->downSamplingShader.setVec2("srcResolution", m_ViewportSizeFloat);

	// bind src texture
	Textures::bindTexture2D(srcTexture, GL_TEXTURE0);

	for (unsigned int i = 0; i < mipChain.size(); i++)
	{
		const BloomMip& mip = mipChain[i];
		glViewport(0, 0, mip.size.x, mip.size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mip.texture, 0);

		renderStaticQuad4();

		data.shaders->downSamplingShader.setVec2("srcResolution", mip.size);
		Textures::bindTexture2D(mip.texture, GL_TEXTURE0);
	}
}

void BloomRenderer::renderUpsamples(UpdateData& data, float filterRadius)
{
	const std::vector<BloomMip>& mipChain = m_MipChain;

	data.shaders->upSamplingShader.bindProgram();
	data.shaders->upSamplingShader.setFloats("filterRadius", filterRadius);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	for (unsigned int i = mipChain.size() - 1; i > 0; i--)
	{
		const BloomMip& mip = mipChain[i];
		const BloomMip& nextMip = mipChain[i - 1];

		Textures::bindTexture2D(mip.texture, GL_TEXTURE0);

		glViewport(0, 0, nextMip.size.x, nextMip.size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nextMip.texture, 0);

		renderStaticQuad4();
	}

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Restore if this was default
	glDisable(GL_BLEND);
}

void BloomRenderer::drawMipSamples(UpdateData& data)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFBO);
	m_MipChain.clear();

	glm::vec2 mipSize((float)data.window->getWidth(), (float)data.window->getHeight());
	glm::ivec2 mipIntSize((int)data.window->getWidth(), (int)data.window->getHeight());

	for (unsigned int i = 0; i < m_MipChainLength; i++)
	{
		BloomMip mip;

		mipSize *= 0.5f;
		mipIntSize /= 2;
		mip.size = mipSize;
		mip.intSize = mipIntSize;

		glGenTextures(1, &mip.texture);
		glBindTexture(GL_TEXTURE_2D, mip.texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, (int)mipSize.x, (int)mipSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_MipChain.emplace_back(mip);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MipChain[0].texture, 0);

	// attachments
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomRenderer::drawBloomRenderer(UpdateData& data)
{
	m_ViewportSize = glm::ivec2(data.window->getWidth(), data.window->getHeight());
	m_ViewportSizeFloat = glm::vec2((float)data.window->getWidth(), (float)data.window->getHeight());
}

void BloomRenderer::drawBloomFilter(UpdateData, unsigned int srcTexture)
{

}

void BloomRenderer::drawToFinal(UpdateData& data, unsigned int srcTexture)
{
	// draw the quad
	glViewport(0, 0, data.window->getWidth(), data.window->getHeight());

	data.shaders->postProcessShader.bindProgram();

	Textures::bindTexture2D(srcTexture, GL_TEXTURE0);
	data.shaders->postProcessShader.setInts("screenTexture", 0);

	Textures::bindTexture2D(m_MipChain[0].texture, GL_TEXTURE1);
	//Textures::bindTexture2D(srcTexture, GL_TEXTURE1);
	data.shaders->postProcessShader.setInts("bloomTexture", 1);

	data.shaders->postProcessShader.setFloats("gamma", gamma);
	data.shaders->postProcessShader.setFloats("exposure", exposure);
	data.shaders->postProcessShader.setFloats("contrast", contrast);
	data.shaders->postProcessShader.setFloats("bloomStrength", bloomStrength);
	data.shaders->postProcessShader.setInts("useBloom", data.shaders->enableBloom);
	data.shaders->postProcessShader.setInts("usePostProcessing", data.shaders->enablePostProcessing);

	renderStaticQuad4();
}

void BloomRenderer::drawUserInterface()
{
	ImGui::Begin("Post Processing");

	ImGui::Text("Basic");
	ImGui::SliderFloat("Contrast", &contrast, 0.0f, 1.0f);
	ImGui::SliderFloat("Exposure", &exposure, 0.0f, 10.0f);
	ImGui::SliderFloat("Gamma", &gamma, 0.0f, 10.0f);
	ImGui::SliderFloat("Bloom Strength", &bloomStrength, 0.0f, 1.0f);
	ImGui::DragInt("Bloom Mip Amount", &m_MipChainLength);
	ImGui::SliderFloat("Bloom Filter Radius", &m_FilterRadius, 0.0f, 1.0f);

	ImGui::End();
}