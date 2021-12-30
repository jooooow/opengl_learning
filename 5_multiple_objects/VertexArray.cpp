#include "VertexArray.h"
#include <vector>
#include <GL/glew.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rid);
	glBindVertexArray(rid);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rid);
}

void VertexArray::SetBuffer(VertexBuffer& buffer, VertexBufferLayout& layout)
{
	buffer.Bind();

	std::vector<VertexLayoutElement> elements = layout.GetLayoutElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		VertexLayoutElement element = elements.at(i);
		glVertexAttribPointer(
			i,
			element.count,
			element.type,
			element.is_normalized,
			layout.GetStride(),
			(void*)(offset));
		glEnableVertexAttribArray(i);
		offset += element.size;
	}
}

void VertexArray::Bind()
{
	glBindVertexArray(rid);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
