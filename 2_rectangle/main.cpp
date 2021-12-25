#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


string GetShaderSource(string filepath);

unsigned int CompileShader(unsigned int type, const string& src);

unsigned int GetShader(const string& vertex_shader, const string& fragment_shader);


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "2_rectangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* init glew */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("error : %s\n", glewGetErrorString(err));
    }

    float positions[] = { 
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f,  0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    /*prepare shader*/
    string vertex_shader = GetShaderSource("vs.vert");
    string fragment_shader = GetShaderSource("fs.frag");
    unsigned int shader = GetShader(vertex_shader, fragment_shader);
    if (0 == shader)
    {
        glfwTerminate();
        return 0;
    }
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* draw triangle*/
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

string GetShaderSource(string filepath)
{
    ifstream stream(filepath);
    string source = "";
    string line;

    while (getline(stream, line))
    {
        source += line + "\n";
    }

    return source;
}

unsigned int CompileShader(unsigned int type, const string& src)
{
    GLuint id = glCreateShader(type);
    const char* code = src.c_str();

    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);

    int flag;
    glGetShaderiv(id, GL_COMPILE_STATUS, &flag);
    if (GL_FALSE == flag)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* msg = (char*)_malloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, msg);

        printf("compile %s shader failed!\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int GetShader(const string& vertex_shader, const string& fragment_shader)
{
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    if (vs == 0) return 0;
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
    if (fs == 0) return 0;

    unsigned int program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
