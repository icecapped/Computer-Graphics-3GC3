//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColour;\n"
//"out vec3 Colour;\n"
//"uniform mat4 model;\n"
//"uniform mat4 view;\n"
//"uniform mat4 projection;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"   Colour = vec3(aColour.x, aColour.y, aColour.z);\n"
//"}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec3 aColour;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(aColour.x, aColour.y, aColour.z, 1.0f);\n"
//"}\n\0";
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    // build and compile our shader program
//    // ------------------------------------
//    // vertex shader
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//    // check for shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // fragment shader
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//    // check for shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    // link shaders
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    // check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    // set up vertex data (and buffer(s)) and configure vertex attributes
//    // ------------------------------------------------------------------
//    float vertices[] = {
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
//    };
//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // texture coord attribute
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
//
//        glUseProgram(shaderProgram);
//
//        //every square draw
//
//        // create transformations
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        glm::mat4 view = glm::mat4(1.0f);
//        glm::mat4 projection = glm::mat4(1.0f);
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        // retrieve the matrix uniform locations
//        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
//        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
//        // pass them to the shaders (3 different ways)
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//        // render box
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}