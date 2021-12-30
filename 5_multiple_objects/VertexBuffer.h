#pragma once
class VertexBuffer
{
public:
	VertexBuffer(unsigned int size, const void* data);
	~VertexBuffer();

	void Bind();
	void UnBind();

	inline unsigned int GetRid() { return rid; }
private:
	unsigned int rid;
};

