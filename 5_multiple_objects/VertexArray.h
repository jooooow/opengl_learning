#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void SetBuffer(VertexBuffer& buffer, VertexBufferLayout& layout);

	void Bind();
	void UnBind();
private:
	unsigned int rid;
};

