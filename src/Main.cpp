#include <iostream>

#include "system/window/window.h"
#include "renderer/collections/shaderCollection.h"
#include "renderer/mesh/mesh.h"
#include "scene/scene.h"

int main()
{
	Window window{ 1980, 1080 };

	// collections
	ShaderCollection shaders;
	ModelCollection models;
	TextureCollection textures;

	// update data
	UpdateData updateData
	{
		&window,
		&shaders,
		&models,
		&textures,
	};

	// scene
	Scene scene{ updateData };
	
	// render loop
	while (!window.shouldWindowClose())
	{
		// update the scene
		scene.update(updateData);

		glViewport(0, 0, window.getWidth(), window.getHeight());
		window.clearScreen(0.1f, 0.1f, 0.1f);  // <- reset the scene view

		// draw the scene
		scene.draw(updateData);

		window.updateWindow(); // <- flush the window -
	}

	window.cleanUp();
	return 0;
}