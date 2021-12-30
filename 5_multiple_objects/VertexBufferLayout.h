#pragma once

#include <vector>

struct VertexLayoutElement
{
	unsigned int count;
	unsigned int type;
	unsigned int is_normalized;
	unsigned int size;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	void AddElement(unsigned int type, unsigned int count);

	inline unsigned int GetAttributeCount() { return attribute_cnt; }
	inline unsigned int GetStride() { return stride; }
	inline std::vector<VertexLayoutElement> GetLayoutElements() { return elements; }
private:
	unsigned int attribute_cnt;
	unsigned int stride;

	std::vector<VertexLayoutElement> elements;
};

