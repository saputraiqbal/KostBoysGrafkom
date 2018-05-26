#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2;
	GLuint VBO3, VAO3, EBO3, texture3;
	GLuint VBO4, VAO4, EBO4, texture4;
	GLuint VBO5, VAO5, EBO5, texture5;
	GLuint VBO6, VAO6, EBO6, texture6;
	GLuint VBO7, VAO7, EBO7, texture7;
	GLuint VBO8, VAO8, EBO8, texture8;
	GLuint VBO9, VAO9, EBO9, texture9;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ , CAMERA_SPEED, fovy;
	float angle = 0;
	float angle2 = 0;
	float ex = 1, zed = 1;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildCube();
	void BuildAlmari();
	void BuildSampah();
	void BuildLaptop();
	void BuildPlane();
	void DrawCube();
	void DrawAlmari();
	void DrawSampah();
	void BuildTembok();
	void DrawTembok();
	void BuildPintu();
	void DrawPintu();
	void BuildRobot();
	void DrawRobot();
	void DrawLaptop();
	void BuildJendela();
	void DrawJendela();
	void DrawPlane();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

