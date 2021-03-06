#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#define PI 3.1415926

string GetShaderSource(string filepath);

unsigned int CompileShader(unsigned int type, const string& src);

unsigned int GetShader(const string& vertex_shader, const string& fragment_shader);

void HSV2RGB(int h, int s, int v, float& r, float& g, float& b);


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "3_circle", NULL, NULL);
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

    vector<float> vertices;
    vector<unsigned int> indices;

    float subdiv = 2.0f;
    float circle_r_out = 0.7f;
    float circle_r_in = 0.5f;
    float rotate = 120.0f;

    int idx = 0;
    for (float i = 0.0f; i <= 360.0f; i += subdiv)
    {
        float theta = i + rotate;
        float r, g, b;
        HSV2RGB(i, 100, 100, r, g, b);

        float x_in = circle_r_in * sin(theta / 180.0f * PI);
        float y_in = circle_r_in * cos(theta / 180.0f * PI);
        vertices.push_back(x_in);
        vertices.push_back(y_in);
        vertices.push_back(r);
        vertices.push_back(b);
        vertices.push_back(g);

        float x_out = circle_r_out * sin(theta / 180.0f * PI);
        float y_out = circle_r_out * cos(theta / 180.0f * PI);
        vertices.push_back(x_out);
        vertices.push_back(y_out);
        vertices.push_back(r);
        vertices.push_back(b);
        vertices.push_back(g);

        if (idx > 0)
        {
            indices.push_back(2 * (idx - 1));
            indices.push_back(2 * (idx - 1) + 1);
            indices.push_back(2 * idx);
            indices.push_back(2 * idx);
            indices.push_back(2 * idx + 1);
            indices.push_back(2 * (idx - 1) + 1);
        }
        idx++;
    }
    unsigned int vertices_size = vertices.size();
    unsigned int indices_num = indices.size();

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_num * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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
        glDrawElements(GL_TRIANGLES, indices_num, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

void HSV2RGB(int h, int s, int v, float& r, float& g, float& b)
{
    float H = h, S = s / 100.0f, V = v / 100.0f;
    float C = V * S;
    float X = C * (1 - abs(fmod(H / 60.0f, 2) - 1));
    float m = V - C;
    int flag = floor(H / 60.0f);

    float R, G, B;

    switch (flag)
    {
    case 0:R = C, G = X, B = 0; break;
    case 1:R = X, G = C, B = 0; break;
    case 2:R = 0, G = C, B = X; break;
    case 3:R = 0, G = X, B = C; break;
    case 4:R = X, G = 0, B = C; break;
    case 5:R = C, G = 0, B = X; break;
    }
    
    r = R + m;
    g = G + m;
    b = B + m;
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
