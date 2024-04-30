#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height);

	void clearScreen(float r, float g, float b);
	void updateWindow();
	bool shouldWindowClose();
	void cleanUp();

	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);
	GLFWwindow* getWindow();

	bool rebuildWindow = false;

private:
	void initWindow(int width, int height);
	void openglSetup();

	GLFWwindow* m_Window;
	int m_WIDTH = 0;
	int m_HEIGHT = 0;
};