#pragma once
class IndexBuffer
{
public:
	IndexBuffer(unsigned int size, const void* data);
	~IndexBuffer();

	void Bind();
	void UnBind();

	inline unsigned int GetRid() { return rid; }
private:
	unsigned int rid;
};

