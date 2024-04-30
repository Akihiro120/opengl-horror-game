#include "camera.h"
#include "../../../physics/AABB/aabb.h"
#include "../../../physics/raycasting/raycasting.h"
#include "../../../data/items/itemCollection.h"

#include "Imgui/imgui.h"

Camera::Camera(Transform transform, UpdateData& data) : Entity(transform)
{
	m_Colliders.push_back(AABBCollision::makeAABBCollider(data.models->cubeModel.getMeshes()[0].getVertices(), glm::vec3(0.0f)));
}

void Camera::update(UpdateData& data, std::vector<Entity*>& entities)
{
	float currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_LastTime;
	m_LastTime = currentTime;

	movement(data, entities);
	mouse(data);
	interaction(data, entities);

	// mouse check
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		glfwSetInputMode(data.window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_Mouse = true;
	}
	else {
		glfwSetInputMode(data.window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_Mouse = false;
	}

	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_T) == GLFW_PRESS)
	{
		if (!DELEGATE_itemTesting)
		{
			addItemToInventory(ItemCollection::ITEM_test());

			DELEGATE_itemTesting = true;
		}
	}
	else {
		DELEGATE_itemTesting = false;
	}

	// open inventory
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_InventoryOpen = true;
	}
	else {
		m_InventoryOpen = false;
	}
}
	
void Camera::movement(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::vec3 newLocation = glm::vec3(0.0f);
	glm::vec3 newRotation = calculateDirection(m_Yaw, 0.0f);
	float newSpeed = m_CameraSpeed * m_DeltaTime;

	// calculate movement
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		newLocation += newSpeed * newRotation;
	}
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		newLocation -= newSpeed * newRotation;
	}
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		newLocation -= glm::normalize(glm::cross(newRotation, glm::vec3(0.0f, 1.0f, 0.0f))) * newSpeed;
	}
	if (glfwGetKey(data.window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		newLocation += glm::normalize(glm::cross(newRotation, glm::vec3(0.0f, 1.0f, 0.0f))) * newSpeed;
	}

	// collision
	AABB colliderX{ m_Colliders[0].min + m_Transform.location, m_Colliders[0].max + m_Transform.location };
	colliderX.min.x = m_Colliders[0].min.x + m_Transform.location.x + newLocation.x;
	colliderX.max.x = m_Colliders[0].max.x + m_Transform.location.x + newLocation.x;

	AABB colliderZ{ m_Colliders[0].min + m_Transform.location, m_Colliders[0].max + m_Transform.location };
	colliderZ.min.z = m_Colliders[0].min.z + m_Transform.location.z + newLocation.z;
	colliderZ.max.z = m_Colliders[0].max.z + m_Transform.location.z + newLocation.z;

	// intersection test
	for (int i = 1; i < entities.size(); i++)
	{
		for (int j = 0; j < entities[i]->getColliders().size(); j++)
		{
			if (AABBCollision::checkAABBCollision(colliderX, entities[i]->getColliders()[j]) && entities[i]->getColliders()[j].activeCollider)
			{
				newLocation.x = 0.0f;
				break;
			}
			if (AABBCollision::checkAABBCollision(colliderZ, entities[i]->getColliders()[j]) && entities[i]->getColliders()[j].activeCollider)
			{
				newLocation.z = 0.0f;
				break;
			}
		}
	}

	m_Transform.location += newLocation;

	// restrict y level
	m_Transform.location.y = floorLevel + 2.5f;
}

void Camera::mouse(UpdateData& data)
{
	if (!m_Mouse)
	{
		// retrive mouse coords
		double xPos, yPos;
		glfwGetCursorPos(data.window->getWindow(), &xPos, &yPos);

		float xoffset = xPos - m_LastX;
		float yoffset = m_LastY - yPos; // reversed since y-coordinates range from bottom to top
		m_LastX = xPos;
		m_LastY = yPos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

		m_Transform.rotation = calculateDirection(m_Yaw, m_Pitch);
	}
}

glm::vec3 Camera::calculateDirection(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::normalize(direction);
}

void Camera::interaction(UpdateData& data, std::vector<Entity*>& entities)
{
	Ray raycast = RayCasting::createRay(m_Transform.location, m_Transform.rotation);
	for (int i = 1; i < entities.size(); i++)
	{
		for (int j = 0; j < entities[i]->getColliders().size(); j++)
		{
			// perform ray intersection test
			if (RayCasting::checkAABBRayIntersection(&raycast, &entities[i]->getColliders()[j], 5.0f)
				&& entities[i]->getColliders()[j].activeCollider)
			{
				m_CurrentInteractingActor = i;
				m_CurrentInteractingCollider = j;
				//entities[i]->onRayHoverInteract(entities, j);
				if (glfwGetKey(data.window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
				{
					if (!DELEGATE_interact)
					{
						entities[i]->onRayInteract(data, entities, m_CurrentInteractingCollider);
						DELEGATE_interact = true;
					}
				}
				else {
					DELEGATE_interact = false;
				}
			}
			else {
				m_CurrentInteractingActor = 0;
				m_CurrentInteractingCollider = 0;
				entities[i]->onStopRayHoverInteract(entities);
			}
		}
	}
}

void Camera::addItemToInventory(Item item)
{
	m_Inventory.push_back(item);
}

void Camera::userInterface(UpdateData& data, std::vector<Entity*>& entities)
{
	if (m_InventoryOpen)
	{
		ImGui::Begin("Inventory", &m_InventoryOpen);

		for (int i = 0; i < m_Inventory.size(); i++)
		{
			ImGui::Text(m_Inventory[i].name.c_str());
		}

		ImGui::End();
	}
}

void Camera::removeItemAtIndex(int index)
{
	m_Inventory.erase(m_Inventory.begin() + index);
}