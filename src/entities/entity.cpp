#include "entity.h"

Entity::Entity(Transform transform)
{
	this->m_Transform = transform;
}

void Entity::draw(UpdateData& data, std::vector<Entity*>& entities)
{

}

void Entity::update(UpdateData& data, std::vector<Entity*>& entities)
{

}

void Entity::userInterface(UpdateData& data, std::vector<Entity*>& entities)
{

}

Transform Entity::getTransform()
{
	return m_Transform;
}

glm::mat4 Entity::makeModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, m_Transform.location);
	model = glm::rotate(model, glm::radians(m_Transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_Transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_Transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, m_Transform.scale);

	return model;
}

std::vector<AABB> Entity::getColliders()
{
	return m_Colliders;
}

void Entity::onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID)
{

}

void Entity::onRayHoverInteract(std::vector<Entity*>& entities, int colliderID)
{

}

void Entity::onStopRayHoverInteract(std::vector<Entity*>& entities)
{

}

void Entity::destroyActor()
{
	m_Destroy = true;
}

void Entity::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{

}