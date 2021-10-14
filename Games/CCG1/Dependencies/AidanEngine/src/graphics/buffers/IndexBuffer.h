#pragma once
#include <GL/glew.h>
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	//short instead of int copunt?
	IndexBuffer(GLushort* data, GLsizei count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; };
};
