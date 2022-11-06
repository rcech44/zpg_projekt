#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include others
#include <vector>
#include <cstdlib>
#include <ctime>

//Include classes
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
private:
	Camera camera;
	std::vector<Light> lights;
	std::vector<DrawableObject> objects;
	std::vector<DrawableObject> lightObjects;
	bool flashlight = true;

public:
	Scene() {};
	void addObject(DrawableObject* obj);
	void addLightObject(DrawableObject* obj);
	void addLight(Light* l);
	Camera* getCamera();
	std::vector<Light>* getLights();
	void update();
	void init();
	void toggleFlashlight();
	bool flashlightStatus();

	float orbit = 1.f;
};

