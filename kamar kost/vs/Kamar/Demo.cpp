#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildCube();

	BuildAlmari();

	BuildSampah();

	BuildLaptop();

	BuildTembok();

	BuildPintu();

	BuildJendela();

	BuildRobot();

	BuildPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {
	if (angle2 < 1.5)
	{
		angle2 += (float)((deltaTime * 1.5f) / 1000);
	}
	else if (ex <= 10 && zed >= 0)
	{
		ex += (deltaTime / 1000) * 4;
	}

	if (ex >= 10 && zed >= -10)
	{
		if (angle2 < 3)
		{
			angle2 += (float)((deltaTime * 1.5f) / 1000);
		}
		else {
			zed -= (deltaTime / 1000) * 4;
		}

	}

	if (ex >= 0 && zed <= -10)
	{
		if (angle2 < 4.5)
		{
			angle2 += (float)((deltaTime * 1.5f) / 1000);
		}
		else {
			ex -= (deltaTime / 1000) * 4;
		}
	}

	if (ex <= 0 && zed <= 0)
	{
		if (angle2 < 6.3)
		{
			angle2 += (float)((deltaTime * 1.5f) / 1000);
		}
		else {
			zed += (deltaTime / 1000) * 4;
		}

	}
	if (ex <= 10 && zed >= 0 && angle2 >= 6.3) {
		angle2 = 0;
	}
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawCube();

	DrawAlmari();

	DrawSampah();

	DrawLaptop();

	DrawTembok();

	DrawPintu();

	DrawJendela();

	DrawRobot();

	DrawPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kasur.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// position ------- color
		//depan besar
		-6.58294, 1.5, -3.32647, 0.01f, 0.65f, //front top left 0
		-6.58294, 1.5, 3.32647, 0.4f, 0.58f, //front top right 1
		-6.58294, -0.00309, 3.32647f, 0.42f, 0.41f, //front bottom right 3
		-6.58294, -0.00309, -3.32647, 0.025f, 0.525f, //front bottom left 2
		//kiri besar
		-6.58294, 1.5, -3.32647, 0.01f, 0.65f, //front top left 0
		-6.58294, -0.00309, -3.32647, 0.025f, 0.525f, //front bottom left 2
		5.00659f, -0.00309f, -3.32647f, 0.42f, 0.41f, //backfront bottom left 6
		5.00659f, 1.5f, -3.32647f, 0.4f, 0.58f, //backfront left 4
		
		//kanan besar
		-6.58294, 1.5, 3.32647, 0.4f, 0.58f, //front top right 1
		-6.58294, -0.00309, 3.32647f, 0.42f, 0.41f, //front bottom right 3
		5.00659f, -0.00309f, 3.32647f, 0.025f, 0.525f,  //backfront bottom right 7
		5.00659f, 1.5, 3.32647f, 0.01f, 0.65f,  //backfront right 5
		//atas besar
		5.00659f, 1.5, -3.32647f, 0.346f, 0.75f, //backfront left 4
		5.00659f, 1.5, 3.32647f, 0.651f, 0.715f,  //backfront right 5
		-6.58294, 1.11430, 3.32647, 0.4f, 0.58f, //front top right 1
		-6.58294, 1.11430, -3.32647, 0.01f, 0.65f, //front top left 0
		
		//depan kecil
		5.00659f, -0.00309f, -3.32647f, 0.36f, 0.76f, //backfront bottom left 6
		5.00659f, -0.00309f, 3.32647f, 0.65f, 0.75f,  //backfront bottom right 7
		5.00659f, 2.5, 3.32647f, 0.66f, 0.86f,  //backfront top right 9
		5.00659f, 2.5, -3.32647f, 0.36f, 0.88f,  //backfront top left 8
		
		//kiri kecil
		5.00659f, -0.00309f, -3.32647f, 0.0f, 0.003f, //backfront bottom left 6
		5.00659f, 2.5, -3.32647f, 0.309f, 0.003f,  //backfront top left 8
		6.58294f, 2.5, -3.32647f, 0.309f, 0.03f, //back top left 10
		6.58294f, -0.00309f, -3.32647f, 0.0f, 0.03f, //back bottom left 14
		//kanan kecil
		5.00659f, -0.00309f, 3.32647f,  0.0f, 0.003f,  //backfront bottom right 7
		5.00659f, 2.5, 3.32647f, 0.309f, 0.003f,  //backfront top right 9
		6.58294f, 2.5, 3.32647f, 0.309f, 0.03f,  //back top right 11 
		6.58294f, -0.00309f, 3.32647f, 0.0f, 0.03f, //back bottom right 15
		//belakang kecil
		6.58294f, 2.5, -3.32647f, 0.0f, 0.003f, //back top left 10
		6.58294f, 2.5, 3.32647f, 0.309f, 0.003f,  //back top right 11 
		6.58294f, -0.00309f, 3.32647f, 0.309f, 0.03f, //back bottom right 15
		6.58294f, -0.00309f, -3.32647f, 0.0f, 0.03f, //back bottom left 14
		//atas kecil
		5.00659f, 2.5, -3.32647f, 0.0f, 0.003f,  //backfront top left 8
		5.00659f, 2.5, 3.32647f, 0.309f, 0.003f,  //backfront top right 9
		6.58294f, 2.5, 3.32647f, 0.309f, 0.03f,  //back top right 11
		6.58294f, 2.5, -3.32647f, 0.0f, 0.03f  //back top left 10
						


	};
	unsigned int indices[] = {  // note that we start from 0!
		
		

		0,  1,  2,  0,  2,  3,
		4,  5,  6,  4,  6,  7,
		8,  9,  10, 8,  10, 11,
		12, 14, 13, 12, 15, 14,
		16, 18, 17, 16, 19, 18,
		20, 22, 21, 20, 23, 22,
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		32, 33, 34, 32, 34, 35
	

		
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 20));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 186, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildAlmari() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("almariedut.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-2, -2, -8.5, 0.119, 0.718,  // 0
		4, -2, -8.5, 0.215, 0.762,   // 1
		4,  7, -8.5, 0.225, 0.97,   // 2
		-2,  7, -8.5, 0.116, 0.99,  // 3

		// right
		4,  7,  -8.5, 0.088, 0.743,  // 4
		4,  7, -11, 0.118, 0.72,  // 5
		4, -2, -11, 0.115, 0.99,  // 6
		4, -2,  -8.5, 0.08, 0.982,  // 7

		// back
		-2, -2, -11, 0.088, 0.743, // 8 
		4,  -2, -11, 0.118, 0.72, // 9
		4,   7, -11, 0.115, 0.99, // 10
		-2,  7, -11, 0.08, 0.982, // 11

		// left
		-2, -2, -11, 0.088, 0.743, // 12
		-2, -2,  -8.5, 0.118, 0.72, // 13
		-2,  7,  -8.5, 0.115, 0.99, // 14
		-2,  7, -11, 0.08, 0.982, // 15

		// upper
		4, 7,  -8.5, 0.088, 0.743,   // 16
		-2, 7,  -8.5, 0.118, 0.72,  // 17
		-2, 7, -11, 0.115, 0.99,  // 18
		4, 7, -11, 0.08, 0.982,   // 19

		// bottom
		-2, -2, -11, 0.088, 0.743, // 20
		4, -2, -11, 0.118, 0.72,  // 21
		4, -2,  -11, 0.115, 0.99,  // 22
		-2, -2,  -11, 0.08, 0.982, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawAlmari()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildSampah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sampah.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		0.0, -1.0, 5.75, 0.234, 0.33, // 0
		0.75, -1.0, 5.75, 0.45, 0.302, // 1
		1.0,  0.0, 6.0, 0.468, 0.835, // 2
		-0.25,  0.0, 6.0, 0.195, 0.86, // 3

		 // right
		 1.0,  0.0,  6.0, 0.195, 0.86,// 4
		 1.0,  0.0, 5.0,  0.468, 0.835,// 5 
		 0.75, -1.0, 5.25, 0.45, 0.302,// 6 
		 0.75, -1.0,  5.75, 0.234, 0.33,// 7 

		// back
		0.0, -1.0, 5.25, 0.234, 0.33,// 8 
		0.75,  -1.0, 5.25, 0.45, 0.302,// 9
		1.0,   0.0, 5.0, 0.468, 0.835,// 10
		-0.25,  0.0, 5.0, 0.195, 0.86,// 11

		 // left
		 0.0, -1.0, 5.25, 0.234, 0.33,// 12
		 0.0, -1.0,  5.75, 0.45, 0.302,// 13
		 -0.25,  0.0,  6.0, 0.468, 0.835,// 14
		 -0.25,  0.0, 5.0, 0.195, 0.86,// 15

		// bottom
		0.0, -1.0, 5.25, 0, 0, // 20
		0.75, -1.0, 5.25, 1, 0,  // 21
		0.75, -1.0,  5.75, 1, 1,  // 22
		0.0, -1.0,  5.75, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawSampah()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3, -10));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLaptop() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("laptopRog.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// position ------- color
		//front board
		-0.944814f, 0.281568f, 8.5f, 0.0f, 0.1f, //front top left 0 (board)
		0.954158f, 0.276586f, 8.5f, 0.54f, 0.1f, //front top right 1 (board)
		0.944813f, 0.213118f, 8.5f, 0.54f, 0.067f, //front bottom right 3 (board)
		-0.944814f, 0.213118f, 8.5f, 0.0f, 0.067f, //front bottom left 2 (board)
		
		//top board
		-0.944814f, 0.281568f, 8.5f, 0.49f, 0.675f, //front top left 0 (board)
		0.954158f, 0.276586f, 8.5f, 1.0f, 0.675f, //front top right 1 (board)
		0.944814f, 0.281568f, 7.5f, 1.0f, 0.971f, //back top right 5 (board)
		-0.944813f, 0.281568f, 7.5f, 0.49f, 0.971f, //back top left 4 (board)
		
		//back board
		-0.944813f, 0.281568f, 7.5f, 1.0f, 0.022f, //back top left 4 (board)
		0.944814f, 0.281568f, 7.5f, 0.65f, 0.022f, //back top right 5 (board)
		0.944813f, 0.213118f, 7.5f, 0.65f, 0.0f, //back bottom right 7 (board)
		-0.944813f, 0.213118f, 7.5f, 1.f, 0.0f, //back bottom left 6 (board)
		
		//right board
		0.954158f, 0.276586f, 8.5f, 0.595f, 0.2f, //front top right 1 (board)
		0.944814f, 0.281568f, 7.5f, 0.0f, 0.2f, //back top right 5 (board)
		0.944813f, 0.213118f, 7.5f, 0.0f, 0.152f, //back bottom right 7 (board)
		0.944813f, 0.213118f, 8.5f, 0.595f, 0.152f, //front bottom right 3 (board)
				
		//left board
		-0.944813f, 0.281568f, 7.5f, 0.0f, 0.2f, //back top left 4 (board)
		-0.944814f, 0.281568f, 8.5f, 0.595f, 0.2f, //front top left 0 (board)
		-0.944814f, 0.213118f, 8.5f, 0.595f, 0.152f, //front bottom left 2 (board)
		-0.944813f, 0.213118f, 7.5f, 0.0f, 0.152f, //back bottom left 6 (board)

		//bottom board
		-0.944814f, 0.213118f, 8.5f, 1.0f, 0.0f, //front bottom left 2 (board)
		0.944813f, 0.213118f, 8.5f, 1.0f, 0.0f, //front bottom right 3 (board)
		0.944813f, 0.213118f, 7.5f, 0.0f, 0.0f, //back bottom right 7 (board)
		-0.944813f, 0.213118f, 7.5f, 0.0f, 0.0f, //back bottom left 6 (board)

		//front layar
		-0.944813f, 1.59148f, 6.9, 0.0f, 0.6f, //front top left 8 (layar)
		0.944813f, 1.59148f, 6.9, 0.52f, 0.6f, //front top right 9 (layar)
		0.944813f, 0.281568f, 7.5, 0.52f, 0.252f, //front bottom right 11 (layar)
		-0.944813f, 0.281568f, 7.5, 0.0f, 0.252f, //front bottom left 10 (layar)
		
		//top layar		
		-0.944813f, 1.59148f, 6.9f, 0.2f, 0.28f, //back top left 12 (layar)
		0.944813f, 1.59148f, 6.9f, 0.0f, 0.28f, //back top right 13 (layar)
		0.944813f, 1.59148f, 6.9f, 0.0f, 0.3f, //front top right 9 (layar)
		-0.944813f, 1.59148f, 6.9f, 0.2f, 0.3f, //front top left 8 (layar)

		//back layar
		-0.944813f, 1.59148f, 6.9f, 0.4f, 1.0f, //back top left 12 (layar)
		0.944813f, 1.59148f, 6.9f, 0.0f, 1.0f, //back top right 13 (layar)
		0.944813f, 0.281568f, 7.5f, 0.0f, 0.7f, //back bottom right 15 (layar)
		-0.944813f, 0.281568f, 7.5f, 0.4f, 0.7f, //back bottom left 14 (layar)

		//kanan layar
		0.944813f, 1.59148f, 6.9f, 0.2f, 0.28f, //front top right 9 (layar)
		0.944813f, 0.281568f, 7.5f, 0.0f, 0.28f, //front bottom right 11 (layar)
		0.944813f, 0.281568f, 7.5f, 0.0f, 0.3f, //back bottom right 15 (layar)
		0.944813f, 1.59148f, 6.9f, 0.2f, 0.3f, //back top right 13 (layar)

		//kiri layar
		-0.944813f, 1.59148f, 6.9f, 0.2f, 0.28f, //front top left 8 (layar)
		-0.944813f, 0.281568f, 7.5f, 0.0f, 0.28f, //front bottom left 10 (layar)
		-0.944813f, 0.281568f, 7.5f, 0.0f, 0.3f, //back bottom left 14 (layar)
		-0.944813f, 1.59148f, 6.9f, 0.2f, 0.3f, //back top left 12 (layar)
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 21, 22, 20, 22, 23,   // bottom
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		32, 33, 34, 32, 34, 35,
		36, 37, 38, 36, 38, 39,
		40, 41, 42, 40, 42, 43,
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawLaptop()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3.4, 0));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembok() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("white.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-16.7, -1.4, 16.7, 0.119, 0.718,  // 0  0,  0,
		16.7, -1.4, 16.7, 0.215, 0.762,   // 1
		16.7,  7, 16.7, 0.225, 0.97,   // 2
		-16.7,  7, 16.7, 0.116, 0.99,  // 3

		// right
		6.7,  7,  16.7, 0.088, 0.743,  // 4
		6.7,  7, -16.7, 0.118, 0.72,  // 5
		6.7, -1.4, -16.7, 0.115, 0.99,  // 6
		6.7, -1.4,  16.7, 0.08, 0.982,  // 7

		// back
		-16.7, -1.4, -12, 0.088, 0.743, // 8 
		16.7,  -1.4, -12, 0.118, 0.72, // 9
		16.7,   7, -12, 0.115, 0.99, // 10
		-16.7,  7, -12, 0.08, 0.982, // 11

		// left
		-16.7, -1.4, -16.7, 0.088, 0.743, // 12
		-16.7, -1.4,  16.7, 0.118, 0.72, // 13
		-16.7,  7,  16.7, 0.115, 0.99, // 14
		-16.7,  7, -16.7, 0.08, 0.982, // 15


	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembok()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPintu() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("door.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-2, -2, 16.6, 0, 0,  // 0
		4, -2, 16.6, 1, 0,   // 1
		4,  6, 16.6, 1, 1,   // 2
		-2,  6, 16.6, 0, 1,  // 3


	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPintu()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildJendela() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("jendela.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// right
		6.6,  5,  -4, 0, 0,  // 4
		6.6,  5, -9, 1, 0,  // 5
		6.6, 0, -9, 1, 1,  // 6
		6.6, 0,  -4, 0, 1,  // 7

		// right
		6.6,  5,  13, 0, 0,  // 4
		6.6,  5, 8, 1, 0,  // 5
		6.6, 0, 8, 1, 1,  // 6
		6.6, 0,  13, 0, 1,  // 7
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawJendela()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRobot() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("robot.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//kepala
		//bawah
		0.3, 1.3, 0.5, 0.5f, 0.91f,//0
		-0.3, 1.3, 0.5, 0.4f, 0.91f,//1
		-0.3, 1.3, -0.1, 0.4f, 1.0f,//2
		0.3, 1.3, -0.1, 0.5f, 1.0f,//3

		//depan
		0.3, 1.3, 0.5, 0.15f, 0.91f,//4
		-0.3, 1.3, 0.5, 0.0f, 0.91f,//5
		-0.3, 1.55, 0.5, 0.0f, 1.0f,//6
		0.3, 1.55, 0.5, 0.15f, 1.0f,//7

		//belakang
		0.3, 1.3, -0.1, 0.5f, 0.91f,//8
		-0.3, 1.3, -0.1, 0.4f, 0.91f,//9
		-0.3, 1.55, -0.1, 0.4f, 1.0f,//10
		0.3, 1.55, -0.1, 0.5f, 1.0f,//11

		//kiri
		0.3, 1.3, 0.5, 0.5f, 0.91f,//12
		0.3, 1.3, -0.1, 0.4f, 0.91f,//13
		0.3, 1.55, -0.1, 0.4f, 1.0f,//14
		0.3, 1.55, 0.5, 0.5f, 1.0f,//15

		//kanan
		-0.3, 1.3, 0.5, 0.5f, 0.91f,//16
		-0.3, 1.3, -0.1, 0.4f, 0.91f,//17
		-0.3, 1.55, -0.1, 0.4f, 1.0f,//18
		-0.3, 1.55, 0.5, 0.5f, 1.0f,//19

		//atas
		0.3, 1.55, 0.5, 0.5f, 0.91f,//20
		-0.3, 1.55, 0.5, 0.4f, 0.91f,//21
		-0.3, 1.55, -0.1, 0.4f, 1.0f,//22
		0.3, 1.55, -0.1, 0.5f, 1.0f,//23

		//leher

		//depan
		0.1, 1.2, 0.3, 0.08f, 0.8f,//24
		-0.1, 1.2, 0.3, 0.02f, 0.8f,//25
		-0.1, 1.3, 0.3, 0.02f, 0.89f,//26
		0.1, 1.3, 0.3, 0.08f, 0.89f,//27

		//belakang
		0.1, 1.2, 0.1, 0.08f, 0.8f,//28
		-0.1, 1.2, 0.1, 0.02f, 0.8f,//29
		-0.1, 1.3, 0.1, 0.02f, 0.89f,//30
		0.1, 1.3, 0.1, 0.08f, 0.89f,//31
		
		//kanan
		-0.1, 1.2, 0.3, 0.08f, 0.8f,//32
		-0.1, 1.2, 0.1, 0.02f, 0.8f,//33
		-0.1, 1.3, 0.1, 0.02f, 0.89f,//34
		-0.1, 1.3, 0.3, 0.08f, 0.89f,//35

		//kiri
		0.1, 1.2, 0.3, 0.08f, 0.8f,//36
		0.1, 1.2, 0.1, 0.02f, 0.8f,//37
		0.1, 1.3, 0.1, 0.02f, 0.89f,//38
		0.1, 1.3, 0.3, 0.08f, 0.89f,//39

		//badan atas
		// front
		-0.5, 0.5, 0.5, 0.15f, 0.8f,  //40
		0.5, 0.5, 0.5, 0.4f, 0.8f,   // 41
		0.5,  1.2, 0.5, 0.4f, 1.0f,   //42
		-0.5,  1.2, 0.5, 0.15f, 1.0f,  //43

		// kiri
		0.5,  1.2,  0.5, 0.5f, 0.91f,  // 44
		0.5,  1.2, -0.1, 0.4f, 0.91f,  // 45
		0.5, 0.5, -0.1, 0.4f, 1.0f,  // 46
		0.5, 0.5,  0.5, 0.5f, 1.0f,   //47

		// back
		-0.5, 0.5, -0.1, 0.5f, 0.91f, // 48
		0.5,  0.5, -0.1, 0.4f, 0.91f, // 49
		0.5,   1.2, -0.1, 0.4f, 1.0f, // 50
		-0.5,  1.2, -0.1, 0.5f, 1.0f, // 51

		// kanan
		-0.5, 0.5, -0.1, 0.5f, 0.91f, // 52
		-0.5, 0.5,  0.5, 0.4f, 0.91f, // 53
		-0.5, 1.2,  0.5, 0.4f, 1.0f, // 54
		-0.5, 1.2, -0.1, 0.5f, 1.0f, // 55

		// atas
		0.5, 1.2,  0.5, 0.5f, 0.91f,   // 56
		-0.5, 1.2,  0.5, 0.4f, 0.91f,  // 57
		-0.5, 1.2, -0.1, 0.4f, 1.0f,  // 58
		0.5, 1.2, -0.1, 0.5f, 1.0f,   // 59

		//bahu kiri
		//atas
		0.5, 1.1, 0.4, 0.5f, 0.91f,//60
		0.5, 1.1, 0.0, 0.4f, 0.91f,//61
		0.8, 1.1, 0.0, 0.4f, 1.0f,//62
		0.8, 1.1, 0.4, 0.5f, 1.0f,//63

		//bawah
		0.5, 0.8, 0.0, 0.5f, 0.91f,//64
		0.5, 0.8, 0.4, 0.4f, 0.91,//65
		0.9, 0.7, 0.4, 0.4f, 1.0,//66
		0.9, 0.7, 0.0, 0.5f, 1.0f,//67

		//kiri
		0.8, 1.1, 0.0, 0.5f, 0.91f,//68
		0.9, 0.7, 0.0, 0.4f, 0.91f,//69
		0.9, 0.7, 0.4, 0.4f, 1.0f,//70
		0.8, 1.1, 0.4, 0.5f, 1.0f,//71

		//belakang
		0.8, 1.1, 0.0, 0.5f, 0.91f,//72
		0.9, 0.7, 0.0, 0.4f, 0.91f,//73
		0.5, 0.8, 0.0, 0.4f, 1.0f,//74
		0.5, 1.1, 0.0, 0.5f, 1.0f,//75
		
		//depan
		0.8, 1.1, 0.4, 0.5f, 0.91f,//76
		0.9, 0.7, 0.4, 0.4f, 0.91f,//77
		0.5, 0.8, 0.4, 0.4f, 1.0f,//78
		0.5, 1.1, 0.4, 0.5f, 1.0f,//79

		//bahu kanan
				
		//atas
		-0.5, 1.1, 0.4, 0.5f, 0.91f,//80
		-0.5, 1.1, 0.0, 0.4f, 0.91f,//81
		-0.8, 1.1, 0.0, 0.4f, 1.0f,//82
		-0.8, 1.1, 0.4, 0.5f, 1.0f,//83

		//bawah
		-0.5, 0.8, 0.0, 0.5f, 0.91f,//84
		-0.5, 0.8, 0.4, 0.4f, 0.91,//85
		-0.9, 0.7, 0.4, 0.4f, 1.0,//86
		-0.9, 0.7, 0.0, 0.5f, 1.0f,//87

		//kiri
		-0.8, 1.1, 0.0, 0.5f, 0.91f,//88
		-0.9, 0.7, 0.0, 0.4f, 0.91f,//89
		-0.9, 0.7, 0.4, 0.4f, 1.0f,//90
		-0.8, 1.1, 0.4, 0.5f, 1.0f,//91

		//belakang
		-0.8, 1.1, 0.0, 0.5f, 0.91f,//92
		-0.9, 0.7, 0.0, 0.4f, 0.91f,//93
		-0.5, 0.8, 0.0, 0.4f, 1.0f,//94
		-0.5, 1.1, 0.0, 0.5f, 1.0f,//95

		//depan
		-0.8, 1.1, 0.4, 0.5f, 0.91f,//96
		-0.9, 0.7, 0.4, 0.4f, 0.91f,//97
		-0.5, 0.8, 0.4, 0.4f, 1.0f,//98
		-0.5, 1.1, 0.4, 0.5f, 1.0f,//99


		//badan bawah
		// front
		-0.3, -0.5, 0.2, 0.06f, 0.605f,  // 100
		0.3, -0.5, 0.2, 0.24f, 0.605f,   // 101
		0.5,  0.5, 0.5, 0.295f, 0.8f,   // 102
		-0.5,  0.5, 0.5, 0.0f, 0.8f,  // 103

		// left
		0.5,  0.5,  0.5, 0.5f, 0.94f,  // 104
		0.5,  0.5, -0.1, 0.4f, 0.94f,  // 105
		0.3, -0.5, 0.0, 0.4f, 0.91f,  // 106
		0.3, -0.5,  0.2, 0.5f, 0.91f,  // 107

		// back
		-0.3, -0.5, 0.0, 0.5f, 0.94f, // 108
		0.3,  -0.5, 0.0, 0.4f, 0.94f, // 109
		0.5,   0.5, -0.1, 0.4f, 0.91f, // 110
		-0.5,  0.5, -0.1, 0.5f, 0.91f, // 111

		// right
		-0.3, -0.5, 0.0, 0.5f, 0.94f, // 112
		-0.3, -0.5,  0.2, 0.4f, 0.94f, // 113
		-0.5,  0.5,  0.5, 0.4f, 0.91f, // 114
		-0.5,  0.5, -0.1, 0.5f, 0.91f, // 115

		// bottom
		-0.3, -0.5, 0.0, 0.5f, 0.94f, // 116
		0.3, -0.5, 0.0, 0.4f, 0.94f,  // 117
		0.3, -0.5,  0.2, 0.4f, 0.91f,  // 118
		-0.3, -0.5,  0.2, 0.5f, 0.91f, // 119
		
	};
	unsigned int indices[] = {  // note that we start from 0!
		
		//kepala
		0,  1,  2,  0,  2,  3,   
		4,  5,  6,  4,  6,  7,   
		8,  9,  10, 8,  10, 11,  
		12, 14, 13, 12, 15, 14,  
		16, 18, 17, 16, 19, 18,  
		20, 22, 21, 20, 23, 22,  

		//leher
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		32, 33, 34, 32, 34, 35,
		36, 37, 38, 36, 38, 39,

		//badan atas
		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59,

		//bahu kiri
		60, 61, 62, 60, 62, 63,
		64, 65, 66, 64, 66, 67,
		68, 69, 70, 68, 70, 71,
		72, 73, 74, 72, 74, 75,
		76, 77, 78, 76, 78, 79,

		//bahu kanan
		80, 81, 82, 80, 82, 83,
		84, 85, 86, 84, 86, 87,
		88, 89, 90, 88, 90, 91,
		92, 93, 94, 92, 94, 95,
		96, 97, 98, 96, 98, 99,

		//badan bawah
		100, 101, 102, 100, 102,103,
		104, 105, 106, 104, 106, 107,
		108, 109, 110, 108, 110, 111,
		112, 113, 114, 112, 114, 115,
		116, 117, 118, 116, 118, 119
		
	};

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawRobot()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(ex, 0, zed));

	model = glm::rotate(model, angle2, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 186, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = -40.0f;
	posCamY = 10.0f;
	posCamZ = 0.0;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.0010f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Transformation: Transform Cube", 800, 600, false, false);
}