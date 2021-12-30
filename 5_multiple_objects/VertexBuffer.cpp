#include "VertexBuffer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(unsigned int size, const void* data)
{
	glGenBuffers(1, &rid);
	glBindBuffer(GL_ARRAY_BUFFER, rid);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &rid);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, rid);
}

void VertexBuffer::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
