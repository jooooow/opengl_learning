#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define PI 3.1415926

void HSV2RGB(int h, int s, int v, float& r, float& g, float& b);


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* use core-profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(500, 500, "5_", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	/* init glew */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("error : %s\n", glewGetErrorString(err));
	}

	std::cout << "OPENGL version : " << glGetString(GL_VERSION) << std::endl;

	std::vector<float> vertices1;
	vertices1.push_back(0.0f);
	vertices1.push_back(0.0f);
	vertices1.push_back(1.0f);
	vertices1.push_back(1.0f);
	vertices1.push_back(1.0f);

	std::vector<unsigned int> indices1;

	float subdiv = 6.0f;
	float circle_r = 0.8f;

	int idx = 1;
	for (float i = 0.0f; i <= 360.0f; i += subdiv)
	{
		float x = circle_r * sin(i / 180.0f * PI);
		float y = circle_r * cos(i / 180.0f * PI);
		vertices1.push_back(x);
		vertices1.push_back(y);

		float r, g, b;
		HSV2RGB(i, 100, 100, r, g, b);

		vertices1.push_back(r);
		vertices1.push_back(b);
		vertices1.push_back(g);

		indices1.push_back(0);
		indices1.push_back(idx++);
		indices1.push_back(idx);
	}
	unsigned int vertices1_size = vertices1.size();
	unsigned int indices1_num = indices1.size();

	float positions[] = {
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f,  1.0f, 1.0f, 0.0f
	};

	unsigned int indices2[] = {
		0,1,2,
		2,3,0
	};

	{
		VertexArray va1;
		VertexBuffer vertex_buffer1(vertices1_size * sizeof(float), vertices1.data());
		VertexBufferLayout vertex_buffer_layout1;
		vertex_buffer_layout1.AddElement(GL_FLOAT, 2);
		vertex_buffer_layout1.AddElement(GL_FLOAT, 3);
		va1.SetBuffer(vertex_buffer1, vertex_buffer_layout1);
		IndexBuffer index_buffer1(indices1_num * sizeof(unsigned int), indices1.data());

		VertexArray va2;
		VertexBuffer vertex_buffer2(4 * 5 * sizeof(float), positions);
		VertexBufferLayout vertex_buffer_layout2;
		vertex_buffer_layout2.AddElement(GL_FLOAT, 2);
		vertex_buffer_layout2.AddElement(GL_FLOAT, 3);
		va1.SetBuffer(vertex_buffer2, vertex_buffer_layout2);
		IndexBuffer index_buffer2(6 * sizeof(unsigned int), indices2);

		/*prepare shader*/
		Shader shader1("vs1.vert", "fs1.frag");
		shader1.Bind();

		Shader shader2("vs2.vert", "fs2.frag");
		shader2.Bind();

		float temp = 0.0f;
		float acc = 0.0f;
		int t = 0;
		float bias_x = 0.0f, bias_y = 0.0f;
		float bias_r = 0.06f;
		float bias_f = 0.2f;

		std::cout << "draw" << std::endl;

		/* unbind all */
		vertex_buffer1.UnBind();
		vertex_buffer2.UnBind();
		va1.UnBind();
		va2.UnBind();
		index_buffer1.UnBind();
		index_buffer2.UnBind();
		shader1.UnBind();
		shader2.UnBind();


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* 1.draw circle*/
			shader1.Bind();
			va1.Bind();
			shader1.SetUniformFloat1("color_rotation", temp + t / 10.0);
			shader1.SetUniformFloat1("alpha1", sin(t / 20.0) * 0.45 + 0.55);
			shader1.SetUniformFloat3("bias1", bias_x, bias_y, 0.0f);
			glDrawElements(GL_LINES, indices1_num, GL_UNSIGNED_INT, 0);

			/* 2.draw rectangle*/
			shader2.Bind();
			va2.Bind();
			shader2.SetUniformFloat1("alpha2", cos(t / 20.0) * 0.45 + 0.55);
			shader2.SetUniformFloat1("vertex_rotation", t / 30.0);
			shader2.SetUniformFloat3("bias2", -bias_x, -bias_y, 0.0f);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			CHECK_OPENGL_ERR;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			temp += acc;
			t += 1;
			acc = 8 * sin(0.05 * t);

			bias_x = bias_r * sin(bias_f * t);
			bias_y = bias_r * cos(bias_f * t);
		}
	}

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
