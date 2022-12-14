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

//Include SOIL
#include <SOIL.h>

enum TextureType
{
	Standard = 0,
	CubeMap = 1
};

class Texture
{
private:
	int id = -1;
	int normal_id = -1;
	int type = 0;

public:
	Texture(int type);
	void assignTexture(const char* path);
	void assignTextureWithNormal(const char* path_t, const char* path_n);
	void assignTexture(const char* path1, const char* path2, const char* path3, const char* path4, const char* path5, const char* path6);
	void setTextureType(int type);
	int getID();
	int getNormalID();
};

