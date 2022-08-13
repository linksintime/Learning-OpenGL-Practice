

#include <glad/glad.h> // Needs to be included first because it has the required OpenGL headers behind the scenes.
#include <GLFW/glfw3.h>
 
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// GLSL Shader sources
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main();\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0"; 

int main(){

    float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glfwInit(); // Instantiate glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) // for the disgusting Mac users

    // GLFW window creation and Error checking
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // Creates GLFW window pointer
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // GLAD utils error failsafe opens all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile shader program
    // --------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check if compilation is successful or not
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //      Error check to see if compilation was successful or not
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shaders
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders together

    // Creating and assigning ID's to vertex buffer objects (VBO) that store memory to the GPU
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    //      Binding the buffer target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Creating and assigning ID's to the vertex shader objects    // fragment shader
    // Creating and assigning ID's to the vertex shader objects
    // Creating fragment shaders and assigning id to it same deal with the vertexShaders

    // Shader Program; Combines multiple shaders together
    unsigned int shaderProgram = glCreateProgram();

    // Attach shader program to previously compiled shaders and then link with glLinkProgram
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if compilation of shader program is successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    // The result of the program creates a program object that we can activate by calling glUseProgram with shaderProgram as its argument
    glUseProgram(shaderProgram);

    // Delete shader objects that are in the program since they're already linked and so we don't need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /*
    Right now we sent the input vertex data to the GPU and instructed the GPU how it should process the vertex data within a vertex and fragment shader. 
    We're almost there, but not quite yet. 
    OpenGL does not yet know how it should interpret the vertex data in memory and how it should connect the vertex data to the vertex shader's attributes. 
    We'll be nice and tell OpenGL how to do that. 
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*

    ***glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);***

   -The first parameter specifies which vertex attribute we want to configure. 
        Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0).
        This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
   -The next argument specifies the size of the vertex attribute. 
        The vertex attribute is a vec3 so it is composed of 3 values.
   -The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
   -The next argument specifies if we want the data to be normalized. 
        If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. 
        This is not relevant for us so we'll leave this at GL_FALSE.
   -The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. 
        Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.
        Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed).
        Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
   -The last parameter is of type void* and thus requires that weird cast. 
        This is the offset of where the position data begins in the buffer.
        Since the position data is at the start of the data array this value is just 0. 
        We will explore this parameter in more detail later on...
    */

    // Similar to that of a VBO just bind create and bind the object to an ID like so
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind the Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Rendering Loop; EACH ITERATION IS A FRAME!
    while (!glfwWindowShouldClose(window)){ 
        processInput(window);

        // Drawing code in the render loop FINALLY!
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glClearColor(255.0f/255.0f, 154.0f/255.0f, 154.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
