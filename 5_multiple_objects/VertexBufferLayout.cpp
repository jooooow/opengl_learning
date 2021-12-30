#include "VertexBufferLayout.h"
#include <GL/glew.h>
#include <assert.h>

VertexBufferLayout::VertexBufferLayout()
{
	stride = 0;
	attribute_cnt = 0;
}

VertexBufferLayout::~VertexBufferLayout()
{

}

void VertexBufferLayout::AddElement(unsigned int type, unsigned int count)
{
	switch (type)
	{
	case GL_FLOAT:
		elements.push_back({ count,type,GL_FALSE,(unsigned int)(count * sizeof(GLfloat)) });
		stride += count * sizeof(GLfloat);
		attribute_cnt++;
		break;
	case GL_UNSIGNED_INT:
		elements.push_back({ count,type,GL_FALSE,(unsigned int)(count * sizeof(GLuint)) });
		stride += count * sizeof(GLuint);
		attribute_cnt++;
		break;
	case GL_UNSIGNED_BYTE:
		elements.push_back({ count,type,GL_TRUE,(unsigned int)(count * sizeof(GLubyte)) });
		stride += count * sizeof(GLubyte);
		attribute_cnt++;
		break;
	default:
		assert(1 == 0);
		break;
	}
}
