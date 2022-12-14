#pragma once

#include "Models/suziSmooth.h"
#include "Models/sphere.h"
#include "Models/gift.h"
#include "Models/tree.h"
#include "Models/skycube.h"
#include "ShadersCollection.h"
#include "Scene.h"

// Name conflict - both const float plain[]
namespace plain
{
	#include "Models/plain.h"
}
namespace bushes
{
	#include "Models/bushes.h"
}

void Scene::addObject(DrawableObject* obj)
{
	this->objects.emplace_back(*obj);
	printf("[SCENE] Added one object %d to scene. Current objects: %d\n", obj->getID(), objects.size());
}

void Scene::placeNewObject(glm::vec3 pos, int object_name, int shader)
{
 	DrawableObject do_new = DrawableObject(models.find(object_name)->second, shaders.find(shader)->second, this, this->object_id++);
	do_new.addTransformation(pos, Translation);
	switch (object_name)
	{
		case Tree2:
			do_new.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_new.assignTexture(textures.find(Tree2)->second->getID());
			break;
		case Zombie:
			do_new.addTransformation(glm::vec3(1.3f, 1.3f, 1.3f), Scale);
			do_new.assignTexture(textures.find(Zombie)->second->getID());
			break;
		case Sphere:
			double r = rand() / (RAND_MAX + 1.);
			double g = rand() / (RAND_MAX + 1.);
			double b = rand() / (RAND_MAX + 1.);
			do_new.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_new.addTransformation(glm::vec3(0.f, 1.f, 0.f), Translation);
			do_new.setColor(glm::vec3(r, g, b));

			Light do_light = Light(LightType::Point);
			do_light.setColor(glm::vec3(r, g, b)); 
			do_light.setPosition(glm::vec3(pos.x, pos.y + 1, pos.z));
			do_light.setStrength(1.f);
			addLight(&do_light);

			break;
	}
	
	addObject(&do_new);
}

void Scene::removeObject(int id)
{
	int found = -1;
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects.at(i).getID() == id) found = i;
	}

	if (found != -1)
	{
		this->objects.erase(objects.begin() + found);
		printf("[SCENE] Removed one object from the scene. Current objects: %d\n", objects.size());
	}
}

void Scene::startRotate(int id)
{
	for (auto &o : this->objects)
	{
		if (o.getID() == id)
		{
			o.assignMovement(new ObjectMovement(o.getPosition(), 0, 1, Orbit));
		}
	}
}

void Scene::addSkybox(DrawableObject* obj)
{
	this->skyboxes.emplace_back(*obj);
	printf("[SCENE] Added one skybox to scene. Current lights: %d\n", skyboxes.size());
}

void Scene::addLight(Light* l)
{
	this->lights.emplace_back(*l);
	printf("[SCENE] Added one light to scene. Current lights: %d\n", lights.size());
}

void Scene::addTexture(Texture* t, int id)
{
	this->textures.insert(std::pair<int, Texture*>(id, t));
	printf("[SCENE] Added one texture to scene. Current textures: %d\n", textures.size());
}

Camera* Scene::getCamera()
{
	return &camera;
}

void Scene::addShader(Shader* sh, int id)
{
	this->shaders.insert(std::pair<int, Shader*>(id, sh));
}

void Scene::addModel(Model* m, int id)
{
	this->models.insert(std::pair<int, Model*>(id, m));
}

std::vector<Light>* Scene::getLights()
{
	return &lights;
}

void Scene::update()
{
	for (auto& s : skyboxes)
	{	
		s.render();
	}
	for (auto& o : objects)
	{
		o.move();
		o.render();
	}
}

void Scene::init()
{
	srand(time(NULL));

	// Create shaders
	Shader* sh1 = new Shader("Shaders/fs_standard_object.glsl", "Shaders/vs_standard_object.glsl", StandardObject);													// standard shader
	Shader* sh6 = new Shader("Shaders/fs_standard_object_textured.glsl", "Shaders/vs_standard_object_textured.glsl", StandardObjectTextured);						// standard shader with textures
	Shader* sh8 = new Shader("Shaders/fs_standard_object_textured_normals.glsl", "Shaders/vs_standard_object_textured_normals.glsl", StandardObjectTexturedNormal);	// standard shader with textures and normals
	Shader* sh5 = new Shader("Shaders/fs_constant_object.glsl", "Shaders/vs_constant_object.glsl", ConstantObject);													// object with constant color shader with texture
	Shader* sh7 = new Shader("Shaders/fs_skybox.glsl", "Shaders/vs_skybox.glsl", SkyBox);																			// standard shader without light check

	// Create objects
	Model* m1 = new Model(Monkey, HeaderType, suziSmooth, sizeof(suziSmooth), GL_TRIANGLES, 2904, 3, 3);
	Model* m2 = new Model(Sphere, HeaderType, sphere, sizeof(sphere), GL_TRIANGLES, 2880, 3, 3);
	Model* m4 = new Model(Plain, HeaderType, plain::plain, sizeof(plain::plain), GL_TRIANGLES, 6, 3, 3);
	Model* m5 = new Model(Tree, HeaderType, tree, sizeof(tree), GL_TRIANGLES, 92814, 3, 3);
	Model* m6 = new Model(Bush, HeaderType, bushes::plain, sizeof(bushes::plain), GL_TRIANGLES, 8730, 3, 3);
	Model* m7 = new Model(GiftBox, HeaderType, gift, sizeof(gift), GL_TRIANGLES, 66624, 3, 3);
	Model* m8 = new Model(SkyCube, HeaderType, skycube, sizeof(skycube), GL_TRIANGLES, 36, 3, 0);
	Model* m9 = new Model(Building, ObjectType, "Models/building/model.obj", GL_TRIANGLES, 3, 3);
	Model* m10 = new Model(Zombie, ObjectType, "Models/zombie/zombie.obj", GL_TRIANGLES, 3, 3);
	Model* m11 = new Model(Tree2, ObjectType, "Models/tree/tree.obj", GL_TRIANGLES, 3, 3);
	Model* m12 = new Model(Plain2, ObjectType, "Models/teren/teren.obj", GL_TRIANGLES, 3, 3);
	Model* m13 = new Model(Box, ObjectType, "Models/box/model.obj", GL_TRIANGLES, 3, 3);

	// Models init
	m1->set();
	m2->set();
	m4->setWithTexture();
	m5->set();
	m6->set();
	m7->set();
	m8->setSkyBox();
	m9->setObject();
	m10->setObject();
	m11->setObject();
	m12->setObject();
	m13->setObjectWithNormals();

	// Add models
	addModel(m1, Monkey);
	addModel(m2, Sphere);
	addModel(m4, Plain);
	addModel(m5, Tree);
	addModel(m6, Bush);
	addModel(m7, GiftBox);
	addModel(m8, SkyCube);
	addModel(m9, Building);
	addModel(m10, Zombie);
	addModel(m11, Tree2);
	addModel(m12, Plain2);
	addModel(m13, Box);

	// Shaders init
	sh1->set();
	sh5->set();
	sh6->set();
	sh7->set();
	sh8->set();
	addShader(sh1, StandardObject);
	addShader(sh5, ConstantObject);
	addShader(sh6, StandardObjectTextured);
	addShader(sh7, SkyBox);
	addShader(sh8, StandardObjectTexturedNormal);

	// Load textures
	Texture* t1 = new Texture(TextureType::Standard);
	t1->assignTexture("Textures/grass.png");
	addTexture(t1, Plain2);

	Texture* t5 = new Texture(TextureType::Standard);
	t5->assignTexture("Models/building/model.png");
	addTexture(t5, Building);

	Texture* t6 = new Texture(TextureType::Standard);
	t6->assignTexture("Models/zombie/zombie.png");
	addTexture(t6, Zombie);

	Texture* t7 = new Texture(TextureType::Standard);
	t7->assignTexture("Models/tree/tree.png");
	addTexture(t7, Tree2);

	Texture* t4 = new Texture(TextureType::CubeMap);
	t4->assignTexture("Textures/skybox2/px.png", "Textures/skybox2/nx.png", "Textures/skybox2/py.png", "Textures/skybox2/ny.png", "Textures/skybox2/pz.png", "Textures/skybox2/nz.png");
	addTexture(t4, SkyCube);

	Texture* t8 = new Texture(TextureType::Standard);
	t8->assignTextureWithNormal("Models/box/albedo.png", "Models/box/normalmap.png");
	addTexture(t8, Box);

	// Scene 0 - lighting test - four spheres with light source
	// Scene 1 - larger scene with multiple models and shaders

 	int set_scene = 0;

	switch (set_scene)
	{
		case 0:
		{
			ObjectMovement* orbit_move_4 = new ObjectMovement(glm::vec3(0.f, 0.f, -3.f), 8, 0.5, OrbitVertical);

			// Create four spheres
			DrawableObject do_sphere_1 = DrawableObject(m2, sh1, this, this->object_id++);
			do_sphere_1.addTransformation(glm::vec3(-4, 0, -3), Translation);
			do_sphere_1.setColor(glm::vec3(1, 0, 0));
			addObject(&do_sphere_1);

			DrawableObject do_sphere_2 = DrawableObject(m2, sh1, this, this->object_id++);
			do_sphere_2.addTransformation(glm::vec3(0, 4, -3), Translation);
			do_sphere_2.setColor(glm::vec3(0, 1, 0));
			addObject(&do_sphere_2);

			DrawableObject do_sphere_3 = DrawableObject(m2, sh1, this, this->object_id++);
			do_sphere_3.addTransformation(glm::vec3(4, 0, -3), Translation);
			do_sphere_3.setColor(glm::vec3(0, 0, 1));
			addObject(&do_sphere_3);

			DrawableObject do_sphere_4 = DrawableObject(m2, sh1, this, this->object_id++);
			do_sphere_4.addTransformation(glm::vec3(0, -4, -3), Translation);
			do_sphere_4.setColor(glm::vec3(0, 1, 1));
			addObject(&do_sphere_4);

			DrawableObject do_sphere_5 = DrawableObject(m2, sh1, this, this->object_id++);
			do_sphere_5.setColor(glm::vec3(1, 1, 0));
			do_sphere_5.assignMovement(orbit_move_4);
			addObject(&do_sphere_5);

			// Add plain (ground)
			DrawableObject do_ground = DrawableObject(m4, sh1, this, this->object_id++);
			do_ground.addTransformation(glm::vec3(0, -8, 0), Translation);
			do_ground.addTransformation(glm::vec3(100, 1, 100), Scale);
			do_ground.setColor(glm::vec3(0.3, 0.3, 0.3));
			addObject(&do_ground);

			// Add light between spheres
			Light light_point = Light(LightType::Point);
			light_point.setPosition(glm::vec3(0, 0, -3));
			addLight(&light_point);

			// Add spotlight
			Light light_spotlight = Light(LightType::SpotlightCamera);
			light_spotlight.setCutoff(glm::cos(glm::radians(30.f)));
			light_spotlight.setOuterCutoff(glm::cos(glm::radians(35.f)));
			addLight(&light_spotlight);

			// Render all light sources as light spheres
			for (Light& l : lights)
			{
				if (l.type == LightType::Point)
				{
					DrawableObject do_temp = DrawableObject(m2, sh5, this, object_id++);
					do_temp.setColor(l.color);
					do_temp.addTransformation(l.position, Translation);
					do_temp.addTransformation(glm::vec3(0.4, 0.4, 0.4), Scale);
					addObject(&do_temp);
				}
			}

			break;
		}
		case 1:
		{
			Light light3 = Light(LightType::Point);
			Light light4 = Light(LightType::SpotlightCamera);
			Light light5 = Light(LightType::Directional);
			Light light6 = Light(LightType::Point);
			Light light7 = Light(LightType::Point);
			Light light8 = Light(LightType::Point);
			Light light9 = Light(LightType::Spotlight);

			light3.setColor(glm::vec3(0.8, 0, 1));
			light6.setColor(glm::vec3(1, 1, 0));
			light7.setColor(glm::vec3(0, 0.8, 1));
			light3.setPosition(glm::vec3(-12.0, 2.0, -10.0));
			light4.setPosition(glm::vec3(-3.0, -2.0, 0.0));
			light6.setPosition(glm::vec3(-4.0, 2.0, -15.0));
			light7.setPosition(glm::vec3(-6.8, 6.0, -9.5));
			light8.setPosition(glm::vec3(0, 2, 0));
			light4.setDirection(glm::vec3(0.0, 4.0, 0.0));
			light4.setCutoff(glm::cos(glm::radians(30.f)));
			light4.setOuterCutoff(glm::cos(glm::radians(35.f)));
			light5.setDirection(glm::vec3(0.0, -1.0, 0.0));
			light5.setStrength(0.05f);
			light3.setStrength(2.f);
			light6.setStrength(2.f);
			light7.setStrength(2.f);
			light8.setStrength(1.f);
			light9.setPosition(glm::vec3(0.0, 2.0, 3.0));
			light9.setDirection(glm::vec3(0.3, 0.0, -1.0));
			light9.setStrength(1.f);

			addLight(&light3);
			addLight(&light4);
			addLight(&light5);
			addLight(&light6);
			addLight(&light7);
			//addLight(&light8);
			addLight(&light9);

			// Create movement
			ObjectMovement* line_move = new ObjectMovement(glm::vec3(-2.f, 0.f, -7.f), glm::vec3(-6.f, 0.f, -7.f), 0.01);
			ObjectMovement* orbit_move = new ObjectMovement(glm::vec3(0, 25, 0), 15, 1, Orbit);
			ObjectMovement* orbit_move_2 = new ObjectMovement(glm::vec3(-2.f, 0.f, -7.f), 3, 0.5, Orbit);
			ObjectMovement* orbit_move_3 = new ObjectMovement(glm::vec3(0.f, 0.f, 0.f), 100, 0.5, OrbitVertical);

			// Render all light sources as light spheres
			for (Light& l : lights)
			{
				if (l.type != LightType::SpotlightCamera && l.type != LightType::Directional && l.type != LightType::Spotlight)
				{
					DrawableObject do_temp = DrawableObject(m2, sh5, this, object_id++);
					do_temp.setColor(l.color);
					do_temp.addTransformation(l.position, Translation);
					do_temp.addTransformation(glm::vec3(0.4, 0.4, 0.4), Scale);
					addObject(&do_temp);
				}
			}

			DrawableObject do_box = DrawableObject(m13, sh8, this, object_id++);
			do_box.assignTexture(t8->getID(), t8->getNormalID());
			do_box.addTransformation(glm::vec3(3, 0.5, -3), Translation);
			addObject(&do_box);

			DrawableObject do_box2 = DrawableObject(m13, sh8, this, object_id++);
			do_box2.assignTexture(t8->getID(), t8->getNormalID());
			do_box2.addTransformation(glm::vec3(1.9, 0.5, -3.8), Translation);
			do_box2.addTransformation(glm::vec3(0, glm::radians(40.f), 0), Rotation);
			addObject(&do_box2);

			DrawableObject do_box3 = DrawableObject(m13, sh8, this, object_id++);
			do_box3.assignTexture(t8->getID(), t8->getNormalID());
			do_box3.addTransformation(glm::vec3(3, 0.5, -4.5), Translation);
			do_box3.addTransformation(glm::vec3(0 , glm::radians(60.f), 0), Rotation);
			addObject(&do_box3);

			DrawableObject do_box4 = DrawableObject(m13, sh8, this, object_id++);
			do_box4.assignTexture(t8->getID(), t8->getNormalID());
			do_box4.addTransformation(glm::vec3(2.7, 1.55, -4.1), Translation);
			do_box4.addTransformation(glm::vec3(0, glm::radians(60.f), 0), Rotation);
			addObject(&do_box4);

			DrawableObject do_tree1 = DrawableObject(m11, sh6, this, object_id++);
			do_tree1.addTransformation(glm::vec3(0.f, 0.f, -10.f), Translation);
			do_tree1.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_tree1.assignTexture(t7->getID());
			addObject(&do_tree1);

			DrawableObject do_tree2 = DrawableObject(m11, sh6, this, object_id++);
			do_tree2.addTransformation(glm::vec3(0.f, 0.f, -25.f), Translation);
			do_tree2.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_tree2.assignTexture(t7->getID());
			addObject(&do_tree2);

			DrawableObject do_tree3 = DrawableObject(m11, sh6, this, object_id++);
			do_tree3.addTransformation(glm::vec3(-30.f, 0.f, -20.f), Translation);
			do_tree3.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_tree3.assignTexture(t7->getID());
			addObject(&do_tree3);

			DrawableObject do_tree4 = DrawableObject(m11, sh6, this, object_id++);
			do_tree4.addTransformation(glm::vec3(-20.f, 0.f, -5.f), Translation);
			do_tree4.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_tree4.assignTexture(t7->getID());
			addObject(&do_tree4);

			DrawableObject do_tree5 = DrawableObject(m11, sh6, this, object_id++);
			do_tree5.addTransformation(glm::vec3(-5.f, 0.f, 2.f), Translation);
			do_tree5.addTransformation(glm::vec3(0.3f, 0.3f, 0.3f), Scale);
			do_tree5.assignTexture(t7->getID());
			addObject(&do_tree5);

			DrawableObject do_building = DrawableObject(m9, sh6, this, object_id++);
			do_building.addTransformation(glm::vec3(-20.0f, 0.0f, -30.0f), Translation);
			do_building.addTransformation(glm::vec3(2.0f, 2.0f, 2.0f), Scale);
			do_building.addTransformation(glm::vec3(0.0f, glm::radians(33.f), 0.0f), Rotation);
			do_building.assignTexture(t5->getID());
			addObject(&do_building);

			DrawableObject do_zombie = DrawableObject(m10, sh6, this, object_id++);
			do_zombie.addTransformation(glm::vec3(-2.f, 0.f, -7.f), Translation);
			do_zombie.addTransformation(glm::vec3(1.3f, 1.3f, 1.3f), Scale);
			do_zombie.addTransformation(glm::vec3(0.0, 0.5, 0.0), glm::radians(45.f), RotationAxis);
			do_zombie.assignTexture(t6->getID());
			do_zombie.assignMovement(line_move);
			addObject(&do_zombie);

			DrawableObject do_ground = DrawableObject(m12, sh6, this, object_id++);
			do_ground.assignTexture(t1->getID());
			addObject(&do_ground);

			DrawableObject do_skybox = DrawableObject(m8, sh7, this, object_id++);
			do_skybox.assignTexture(t4->getID());
			addSkybox(&do_skybox);

			DrawableObject do_sun = DrawableObject(m2, sh5, this, object_id++);
			do_sun.setColor(glm::vec3(1, 1, 0));
			do_sun.assignMovement(orbit_move_3);
			addObject(&do_sun);

			// Create random bushes
			for (int i = 0; i < 30; i++)
			{
				DrawableObject do_bush = DrawableObject(m6, sh6, this, object_id++);
				do_bush.assignTexture(t1->getID());

				int x = -10 + rand() % (10 - -10 + 1);
				int z = -10 + rand() % (10 - -10 + 1);
				do_bush.addTransformation(glm::vec3(x, 0, z), Translation);
				do_bush.addTransformation(glm::vec3(4, 4, 4), Scale);

				addObject(&do_bush);
			}

			// Create moving sphere
			DrawableObject do_box1 = DrawableObject(m13, sh8, this, object_id++);
			do_box1.assignTexture(t8->getID(), t8->getNormalID());
			do_box1.assignMovement(orbit_move);
			addObject(&do_box1);

			break;
		}
	}

	// Register observers
	getCamera()->registerObserver(*sh1);
	getCamera()->registerObserver(*sh5);
	getCamera()->registerObserver(*sh6);
	getCamera()->registerObserver(*sh7);
	getCamera()->registerObserver(*sh8);
}

void Scene::toggleFlashlight()
{
	this->flashlight = !flashlight;
}

bool Scene::flashlightStatus()
{
	return flashlight;
}
std::vector<DrawableObject>* Scene::getObjects()
{
	return &this->objects;
}