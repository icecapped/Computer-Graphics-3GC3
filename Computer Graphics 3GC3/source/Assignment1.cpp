#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float get_sun_rotate_angle_around_itself(float day);
float get_earth_rotate_angle_around_sun(float day);
float get_earth_rotate_angle_around_itself(float day);
float get_moon_rotate_angle_around_earth(float day);
float get_moon_rotate_angle_around_itself(float day);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;

const float CAM_SPEED = 1.0f;

// operating global vars;
enum TargetBody {
    SUN,
    EARTH,
    MOON
};

glm::vec3 camPos (50.0f, 50.0f, 50.0f);
TargetBody camTarget = SUN;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColour;\n"
"out vec3 Colour;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"   Colour = aColour;\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 Colour;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(Colour.x, Colour.y, Colour.z, 1.0f);\n"
"}\n\0";

/*
    Build, compile, and link the shader
 */
bool setupShader(GLuint shaderProgram, GLenum shaderType, const char** shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, shaderSource, NULL);
    glCompileShader(shader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
        
    // link shader
    glAttachShader(shaderProgram, shader);
    return true;
}

int run()
{
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // vsync 1

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // SHADER BUILD, COMPILE, LINK
    unsigned int shaderProgram = glCreateProgram();

    // vertex shader
    if (!setupShader(shaderProgram, GL_VERTEX_SHADER, &vertexShaderSource)) {
        std::cout << "Stopping because of vertex shader compilation error";
    }
    // fragment shader
    if (!setupShader(shaderProgram, GL_FRAGMENT_SHADER, &fragmentShaderSource)) {
        std::cout << "Stopping because of fragment shader compilation error";
    }

    // link shaders
    glLinkProgram(shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    // set up vertex data (and buffer(s)) and configure vertex attributes for cube
    // ------------------------------------------------------------------
    // order: -z, +z, -x, +x, -y, +y
    // back front left right bottom top
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,

        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,

        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,

         1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // setup cube VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind to finish
    glBindVertexArray(0);

    // render loop
    // -----------
    float day = 365.0f, inc = 1.0f /60;
    while (!glfwWindowShouldClose(window))
    {
        // INPUT
        processInput(window);


        // "PHYSICS" LOOP
        day += inc;

        // SETUP M-V-P MATRICES
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        // MODEL MATRIX
        // sun
        glm::mat4 suntrans = glm::mat4(1.0f);
        suntrans = glm::rotate(suntrans, glm::radians(get_sun_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f));       // rotation
        suntrans = glm::scale(suntrans, glm::vec3(10.0f, 10.0f, 10.0f));

        // earth position
        glm::mat4 earthpos = glm::mat4(1.0f); // post-orbit location for moon
        earthpos = glm::rotate(earthpos, glm::radians(get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f));        // orbit rotation
        earthpos = glm::translate(earthpos, glm::vec3(40.0f, 0.0f, 0.0f));                                                          // orbit offset
        earthpos = glm::rotate(earthpos, -glm::radians(get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f));       // rotation compensation for orbit rotation

        // earth rotation
        glm::mat4 earthtrans = glm::mat4(1.0f);
        earthtrans = glm::rotate(earthtrans, glm::radians(23.4f), glm::vec3(0.0f, 0.0f, -1.0f));                                    // axis offset
        earthtrans = glm::rotate(earthtrans, glm::radians(get_earth_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // rotation
        earthtrans = glm::scale(earthtrans, glm::vec3(4.0f, 4.0f, 4.0f));
        earthtrans = earthpos * earthtrans;

        // moon
        glm::mat4 moontrans = glm::mat4(1.0f);
        moontrans = glm::rotate(moontrans, glm::radians(get_moon_rotate_angle_around_earth(day)), glm::vec3(0.0f, 1.0f, 0.0f));     // orbit rotation
        moontrans = glm::translate(moontrans, glm::vec3(12.0f, 0.0f, 0.0f));                                                        // orbit offset
        moontrans = glm::rotate(moontrans, -glm::radians(get_moon_rotate_angle_around_earth(day)), glm::vec3(0.0f, 1.0f, 0.0f));    // orbit compensation
        moontrans = glm::rotate(moontrans, glm::radians(get_moon_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f));    // rotation
        moontrans = glm::scale(moontrans, glm::vec3(2.0f, 2.0f, 2.0f));
        moontrans = earthpos * moontrans;   // relative to earth position


        // RENDER SETUP
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glUseProgram(shaderProgram);


        // VIEW MATRIX
        // camera position
        glm::vec3 cameraPos = camPos;
        // camera targets
        glm::vec3 sunpos_vec3 = glm::vec3(0.0f);
        glm::vec3 earthpos_vec3 = glm::vec3(earthpos * glm::vec4(1.0f));
        glm::vec3 moonpos_vec3 = glm::vec3(moontrans * glm::vec4(1.0f));
        glm::vec3 cameraTarget;
        
        switch (camTarget) {
        case(SUN):
            cameraTarget = sunpos_vec3;
            break;
        case(EARTH):
            cameraTarget = earthpos_vec3;
            break;
        case(MOON):
            cameraTarget = moonpos_vec3;
            break;
        }
        view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));


        // PROJECTION MATRIX
        // perspective
        projection = glm::perspective(glm::radians(45.0f), (float)16 / (float)9, 0.1f, 1000.0f);
        // ortho
        // projection = glm::ortho(0.0f, 16.0f/9.0f * 70.0f, -35.0f, 35.0f, 0.1f, 1000.0f); 


        // SET SHADER MATRICES
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        // pass them to the shaders (3 different ways)
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        // RENDER OBJECTS
        glBindVertexArray(VAO);

        // render sun
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(suntrans));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render earth
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthtrans));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render moon
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moontrans));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// sun
float get_sun_rotate_angle_around_itself(float day) {
    return 360.0f / 27.0f * day;
}

// earth
float get_earth_rotate_angle_around_sun(float day) {
    return 360.0f / 365.0f * day;
}
float get_earth_rotate_angle_around_itself(float day) {
    return 360.0f * day;
}

// moon
float get_moon_rotate_angle_around_earth(float day) {
    return 360.0f / 28.0f * day;
}
float get_moon_rotate_angle_around_itself(float day) {
    return 360.0f / 28.0f * day;
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos.x += CAM_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos.x -= CAM_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camPos.y += CAM_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camPos.y -= CAM_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos.z += CAM_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos.z -= CAM_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        camTarget = SUN;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        camTarget = EARTH;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        camTarget = MOON;
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}