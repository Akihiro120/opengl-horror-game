#pragma once
#include "../../entity.h"
#include "../../../renderer/mesh/mesh.h"

class Quad : public Entity
{
public:
	Quad(Transform transform);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;

private:
	Mesh mesh;
};