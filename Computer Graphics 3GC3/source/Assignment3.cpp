#include "Assignments.h"

#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image/stb_image.h>

#include <tiny_obj_loader/tiny_obj_loader.h>

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string filepath[] = { "assets/a3/Amy.obj", "assets/a3/floor.obj", "assets/a3/bucket.obj"};
std::string texpath[] = { "assets/a3/Amy.png", "assets/a3/floor.jpg", "assets/a3/bucket.jpg"};

#define M_PI 3.1415926535897932384626433832795


// Shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexPos;\n"
"out vec3 Normal;\n"
"out vec3 FragPos;"
"out vec2 TexPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"   Normal = mat3(model) * aNormal;\n"
"	FragPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0f));\n"
"	TexPos = aTexPos;\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"#define M_PI 3.1415926535897932384626433832795\n"
"out vec4 FragColor;\n"
"in vec3 Normal;\n"
"in vec2 TexPos;\n"
"in vec3 FragPos;\n"
"uniform sampler2D Texture;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 rPos;\n"
"uniform vec3 gPos;\n"
"uniform vec3 bPos;\n"
"void main()\n"
"{\n"
"   vec4 objColor = texture(Texture, TexPos);\n"
"	vec4 ambientLight  = vec4(0.2, 0.2, 0.2, 1.0);\n"
"	vec4 diffuseLight = vec4(1.0, 1.0, 1.0, 1.0);\n"
"	vec3 attenCoeff = vec3(1.0, 0.000035, 0.000044);"
"	vec3 norm = normalize(Normal);\n"
"	vec3 lightDir = normalize(lightPos - FragPos);\n"	
// r
"	vec3 rDir = normalize(lightPos - rPos);\n"
"	float red = acos(dot(lightDir, rDir)) < M_PI/6.0f ? max(0, dot(norm, lightDir)) : 0;\n"
// g
"	vec3 gDir = normalize(lightPos - gPos);\n"
"	float green = acos(dot(lightDir, gDir)) < M_PI/6.0f ? max(0, dot(norm, lightDir)) : 0;\n"
// b
"	vec3 bDir = normalize(lightPos - bPos);\n"
"	float blue = acos(dot(lightDir, bDir)) < M_PI/6.0f ? max(0, dot(norm, lightDir)) : 0;\n"
"	float lightDist = length(lightPos - FragPos);"
"	float attenuation = 1.0 / (attenCoeff.x + attenCoeff.y * lightDist + attenCoeff.z * lightDist * lightDist);"
"	FragColor = ambientLight * objColor + \n"
"		diffuseLight * objColor * vec4(red, green, blue, 1.0) * attenuation;\n"
"}\n\0";

// Settings
const GLfloat BG_COLOR[4] = {0.3, 0.4, 0.5, 1.0};
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 576;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// global operating variables
// model data
const unsigned int NUM_OBJS = 3;
std::vector<float> vbuffer[NUM_OBJS];
std::vector<float> nbuffer[NUM_OBJS];
std::vector<float> tbuffer[NUM_OBJS];

tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

// textures
const unsigned int NUM_TEXTURES = 3;
unsigned char* texData[NUM_TEXTURES];
int width[NUM_TEXTURES], height[NUM_TEXTURES], nrChannels[NUM_TEXTURES];


bool load_obj(const char* obj_path, unsigned int objId) {
	// tinyobj load obj
	std::string warn, err;
	bool bTriangulate = true;
	bool bSuc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_path, nullptr, bTriangulate);
	if (!bSuc)
	{
		std::cout << "tinyobj error: " << err.c_str() << std::endl;
		return false;
	}
	for (auto id : shapes[0].mesh.indices) {
		int vid = id.vertex_index;
		int nid = id.normal_index;
		int tid = id.texcoord_index;
		//fill in vertex positions
		vbuffer[objId].push_back(attrib.vertices[vid * 3]);
		vbuffer[objId].push_back(attrib.vertices[vid * 3 + 1]);
		vbuffer[objId].push_back(attrib.vertices[vid * 3 + 2]);
		//normal
		nbuffer[objId].push_back(attrib.normals[nid * 3]);
		nbuffer[objId].push_back(attrib.normals[nid * 3 + 1]);
		nbuffer[objId].push_back(attrib.normals[nid * 3 + 2]);
		//tex coord
		tbuffer[objId].push_back(attrib.texcoords[tid * 2]);
		tbuffer[objId].push_back(attrib.texcoords[tid * 2 + 1]);
	}
	return true;
}

void load_img(const char* texPath, unsigned int id) {
	stbi_set_flip_vertically_on_load(true);
	texData[id] = stbi_load(texPath, &width[id], &height[id], &nrChannels[id], 0);
}


int render() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// SHADERS
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check shader compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	//~~~ Config done ~~~
	

	// Vertices
	unsigned int VBO[NUM_OBJS][3], VAO[NUM_OBJS];
    glGenVertexArrays(NUM_OBJS, VAO);


	for (int iobject = 0; iobject < NUM_OBJS; iobject++) {
		glGenBuffers(3, VBO[iobject]);

		glBindVertexArray(VAO[iobject]);

		// position attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO[iobject][0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vbuffer[iobject].size(), &vbuffer[iobject].front(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO[iobject][1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nbuffer[iobject].size(), &nbuffer[iobject].front(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		// texpos attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO[iobject][2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tbuffer[iobject].size(), &tbuffer[iobject].front(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	// Object 0 [bucket]



	// Texture setup
	unsigned int textureT[NUM_TEXTURES];
	glGenTextures(NUM_TEXTURES, textureT);
	for (int i = 0; i < NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, textureT[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB,
			GL_UNSIGNED_BYTE, texData[i]);//define the texture using image data
		stbi_image_free(texData[i]);//don’t forget to release the image data
	}

	// render loop
	float theta = 0;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// reset
		glClearColor(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], BG_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);


		// transformation matrices
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(0, 100, 180), glm::vec3(0.0f, 80.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(60.0f), (float)16 / (float)9, 0.1f, 1000.0f);

		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
		// pass setup matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		// setup lights
		glm::vec3 lightPos = glm::vec3(0.0f, 200.0f, 0.0f);
		//glm::vec3 lightPos = glm::vec3(std::sin(theta)*150, 100, std::cos(theta)*150);
		theta += 0.05f;
		unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

		glm::vec3 rPos = glm::vec3(std::sin(-theta) * 50.0f, -200.0f, std::cos(-theta) * -50.0f);
		glm::vec3 gPos = glm::vec3(std::sin(-theta - M_PI / 3 * 2) * 50.0f, -200.0f, std::cos(-theta - M_PI / 3 * 2) * -50.0f);
		glm::vec3 bPos = glm::vec3(std::sin(-theta - M_PI / 3 * 4) * 50.0f, -200.0f, std::cos(-theta - M_PI / 3 * 4) * -50.0f);
		glUniform3fv(glGetUniformLocation(shaderProgram, "rPos"), 1, glm::value_ptr(rPos));
		glUniform3fv(glGetUniformLocation(shaderProgram, "gPos"), 1, glm::value_ptr(gPos));
		glUniform3fv(glGetUniformLocation(shaderProgram, "bPos"), 1, glm::value_ptr(bPos));

		// draw models
		// amy
		glBindVertexArray(VAO[0]);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, textureT[0]);
		glDrawArrays(GL_TRIANGLES, 0, vbuffer[0].size());

		// floor
		glBindVertexArray(VAO[1]);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, textureT[1]);
		glDrawArrays(GL_TRIANGLES, 0, vbuffer[1].size());

		// bucket
		glBindVertexArray(VAO[2]);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, textureT[2]);
		glDrawArrays(GL_TRIANGLES, 0, vbuffer[2].size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	for (int i = 0; i < NUM_OBJS; i++)
		glDeleteBuffers(3, VBO[i]);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

static unsigned int ss_id = 0;
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte* pixels = new GLubyte[totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id++;
    delete[] pixels;
    fout.flush();
    fout.close();
}

//key board control
void processInput(GLFWwindow* window)
{
    //press escape to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //press p to capture screen
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("Assignment0-ss", buffer_width, buffer_height);
    }
}


int A3::run() {
	int count = 0;
	for (std::string str : filepath)
		load_obj(str.c_str(), count++);
	count = 0;
	for (std::string str : texpath)
		load_img(str.c_str(), count++);


	return render();
}