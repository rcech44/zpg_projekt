#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

enum TransformationType
{
	Translation = 0,
	Rotation = 1,
	Scale = 2,
	RotationAxis = 3
};

class Transformation
{
private:
	glm::vec3 values{0.f};
	int type;

	// RotationAxis
	float angle;
public:
	Transformation(glm::vec3 vals, int type);
	Transformation(glm::vec3 vals, float angle, int type);
	int getType();
	glm::vec3 getValues();
	float getAngle();
};

