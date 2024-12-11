//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 1024;
//const unsigned int SCR_HEIGHT = 768;
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
//"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
//"   Colour = aColour;\n"
//"}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec3 Colour;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(Colour.x, Colour.y, Colour.z, 1.0f);\n"
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
//    #ifdef __APPLE__
//        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    #endif
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
//    float vertices[] = {1/
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
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // texture coord attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // render loop
//    // -----------
//    float day = 365, inc = 1.0f / 24;
//    while (!glfwWindowShouldClose(window))
//    {
//        // input
//        // -----
//        processInput(window);
//        day += inc;
//        // render
//        // ------
//        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
//
//        glUseProgram(shaderProgram);
//
//        // camera position
//        glm::vec3 cameraPos = glm::vec3(80.0f, 70.0f, 60.0f);
//
//        // create transformations
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        glm::mat4 view = glm::mat4(1.0f);
//        glm::mat4 projection = glm::mat4(1.0f);
//        view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//        // looking at earth
//        glm::mat4 earthtrans = glm::mat4(1.0f);
//        glm::mat4 earthloc = glm::mat4(1.0f);
//        earthtrans = glm::rotate(earthtrans, glm::radians(get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 0.0f, -1.0f));
//        earthtrans = glm::translate(earthtrans, glm::vec3(24.0f, 0.0f, 0.0f));
//        earthloc = earthtrans;
//
//        //glm::vec4 earthlocvec = glm::vec4(1.0f);
//        //earthlocvec = earthloc * earthlocvec;
//        //view = glm::lookAt(cameraPos, glm::vec3(earthlocvec), glm::vec3(0.0f, 1.0f, 0.0f));
//
//        projection = glm::perspective(glm::radians(30.0f), (float)4 / (float)3, 0.1f, 1000.0f);
//        // retrieve the matrix uniform locations
//        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
//        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
//        // pass them to the shaders (3 different ways)
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//        glBindVertexArray(VAO);
//
//        // render sun
//        glm::mat4 suntrans = glm::mat4(1.0f);
//        suntrans = glm::scale(suntrans, glm::vec3(6.0f, 6.0f, 6.0f));
//        suntrans = glm::rotate(suntrans, glm::radians(get_sun_rotate_angle_around_itself(day)), glm::vec3(0.0f, 0.0f, -1.0f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(suntrans));
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // render earth
//        earthtrans = glm::rotate(earthtrans, glm::radians(23.4f + get_earth_rotate_angle_around_itself(day) - get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 0.0f, -1.0f));
//        earthtrans = glm::scale(earthtrans, glm::vec3(3.0f, 3.0f, 3.0f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthtrans));
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // render moon
//        glm::mat4 moontrans = earthloc;
//        moontrans = glm::rotate(moontrans, glm::radians(get_moon_rotate_angle_around_earth(day)), glm::vec3(0.0f, 0.0f, -1.0f));
//        moontrans = glm::translate(moontrans, glm::vec3(12.0f, 0.0f, 0.0f));
//        //moontrans = glm::rotate(moontrans, glm::radians(get_moon_rotate_angle_around_itself(day)), glm::vec3(0.0f, 0.0f, -1.0f));
//        moontrans = glm::scale(moontrans, glm::vec3(1.5f, 1.5f, 1.5f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moontrans));
//        glDrawArrays(GL_TRIANGLES, 0, 36);
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
//// sun
//float get_sun_rotate_angle_around_itself(float day) {
//    return 360.0f / 27.0f * day;
//}
//
//// earth
//float get_earth_rotate_angle_around_sun(float day) {
//    return 360.0f / 365.0f * day;
//}
//float get_earth_rotate_angle_around_itself(float day) {
//    return 360.0f / 24.0f * day;
//}
//
//// moon
//float get_moon_rotate_angle_around_earth(float day) {
//    return 360.0f / 27.0f * day;
//}
//float get_moon_rotate_angle_around_itself(float day) {
//    return 360.0f / 27.0f * day;
//}
//
//static unsigned int ss_id = 0;
//void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
//    int pixelChannel = 3;
//    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
//    GLubyte* pixels = new GLubyte[totalPixelSize];
//    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
//    std::ofstream fout(file_name);
//    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
//    for (size_t i = 0; i < height; i++)
//    {
//        for (size_t j = 0; j < width; j++)
//        {
//            size_t cur = pixelChannel * ((height - i - 1) * width + j);
//            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
//        }
//        fout << std::endl;
//    }
//    ss_id++;
//    delete[] pixels;
//    fout.flush();
//    fout.close();
//}
//
////key board control
//void processInput(GLFWwindow* window)
//{
//    //press escape to exit
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    //press p to capture screen
//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
//    {
//        std::cout << "Capture Window " << ss_id << std::endl;
//        int buffer_width, buffer_height;
//        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
//        dump_framebuffer_to_ppm("Assignment0-ss", buffer_width, buffer_height);
//    }
//}
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}