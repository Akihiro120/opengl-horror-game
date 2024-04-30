#include "pbrDemo.h"
#include "../lightCube/lightCube.h"

PBRDemo::PBRDemo(Transform transform, UpdateData& data, std::vector<Entity*>& entities) : Entity(transform)
{

}

void PBRDemo::draw(UpdateData& data, std::vector<Entity*>& entities)
{
    data.shaders->defaultShader.bindProgram();
    
    data.textures->loadConcreteMaterial(&data.shaders->defaultShader, 1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < 7; ++row)
    {
        data.shaders->defaultShader.setFloats("metallic", (float)row / (float)7);
        //data.shaders->defaultShader.setFloats("metallic", 1.0f);
        for (int col = 0; col < 7; ++col)
        {
            // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
            // on direct lighting.
            data.shaders->defaultShader.setFloats("roughness", glm::clamp((float)col / (float)7, 0.05f, 1.0f));
            //data.shaders->defaultShader.setFloats("roughness", 0.0f);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (7 / 2)) * 1.35f,
                (float)(row - (7 / 2)) * 1.35f,
                -2.0f
            ));
            model = glm::translate(model, m_Transform.location);
            data.shaders->defaultShader.setMat4("model", model);
            data.models->sphereModel.draw();
        }
    }
}

void PBRDemo::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
    shader->bindProgram();

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 7; ++col)
        {

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (7 / 2)) * 1.35f,
                (float)(row - (7 / 2)) * 1.35f,
                -2.0f
            ));
            model = glm::translate(model, m_Transform.location);
            shader->setMat4("model", model);
            data.models->sphereModel.draw();
        }
    }
}