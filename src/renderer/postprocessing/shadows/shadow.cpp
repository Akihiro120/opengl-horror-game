#include "shadow.h"

ShadowRenderer::ShadowRenderer()
{

}

unsigned int quadVAO2 = 0;
unsigned int quadVBO2;
void renderStaticQuad()
{
	if (quadVAO2 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO2);
		glGenBuffers(1, &quadVBO2);
		glBindVertexArray(quadVAO2);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void ShadowRenderer::drawShadowMapStatic(UpdateData& data, std::vector<Entity*>& entities)
{
	/*lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	data.shaders->depthMapShader.bindProgram();
	data.shaders->depthMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	// 1. first render to depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities[i]->drawToShaderMap(data, &data.shaders->depthMapShader, entities);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	data.textures->shadowMap = depthMap;*/
}

void ShadowRenderer::drawShaderMap(UpdateData& data, std::vector<Entity*>& entities)
{
	//glm::vec3 lightPos = glm::vec3(-2.0f, 8.0f, -1.0f);
	if (data.shaders->enableShadows)
	{
		for (unsigned int i = 0; i < data.shaders->getPointLights().size(); i++)
		{
			if (data.shaders->pointLights[i].castShadows)
			{
				glm::vec3 lightPos = data.shaders->pointLights[i].position;
				float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
				float near = 1.0f;
				float far = 50.0f;
				glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(shadowProj *
					glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, data.shaders->pointLights[i].shadowFBO);

				glClear(GL_DEPTH_BUFFER_BIT);
				//ConfigureShaderAndMatrices();
				data.shaders->depthMapPointShader.bindProgram();
				for (unsigned int i = 0; i < 6; ++i)
					data.shaders->depthMapPointShader.setMat4(std::string("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);

				data.shaders->depthMapPointShader.setFloats("far_plane", far_plane);
				data.shaders->depthMapPointShader.setVec3("lightPos", lightPos);
				//RenderScene();
				glCullFace(GL_FRONT);
				for (unsigned int i = 0; i < entities.size(); i++)
				{
					entities[i]->drawToShaderMap(data, &data.shaders->depthMapPointShader, entities);
				}
				glCullFace(GL_BACK);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}
}

void ShadowRenderer::buildShadows(UpdateData& data)
{
    // create depth cubemap texture
	if (data.shaders->enableShadows)
	{
		for (unsigned int i = 0; i < data.shaders->pointLights.size(); i++)
		{
			if (data.shaders->pointLights[i].castShadows)
			{
				glDeleteFramebuffers(1, &data.shaders->pointLights[i].shadowFBO);
				glDeleteTextures(1, &data.shaders->pointLights[i].shadowMap);
				glGenFramebuffers(1, &data.shaders->pointLights[i].shadowFBO);
				glGenTextures(1, &data.shaders->pointLights[i].shadowMap);
				glBindTexture(GL_TEXTURE_CUBE_MAP, data.shaders->pointLights[i].shadowMap);
				for (unsigned int i = 0; i < 6; ++i)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				// attach depth texture as FBO's depth buffer
				glBindFramebuffer(GL_FRAMEBUFFER, data.shaders->pointLights[i].shadowFBO);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, data.shaders->pointLights[i].shadowMap, 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}
}