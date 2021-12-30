#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(unsigned int size, const void* data)
{
	glGenBuffers(1, &rid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &rid);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rid);
}

void IndexBuffer::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
