
#ifndef RASTERIZATION_H_
#define RASTERIZATION_H_

#include "Shader.h"
#include "FrameBuffer.h"

class Rasterization {

private: 

	int Width;					//�ֱ��ʿ�
	int Height;					//�ֱ��ʳ�
	FrameBuffer* FrontBuffer;	//������
	Shader* shader;

	glm::mat4 ViewPortMatrix;


public:

	Rasterization(const int& w, const int& h);
	~Rasterization();

	void setModelMatrix(const glm::mat4& model)
	{
		shader->setModelMatrix(model);
	}
	void setViewMatrix(const glm::mat4& view)
	{
		shader->setViewMatrix(view);
	}
	void setProjectMatrix(const glm::mat4& project)
	{
		shader->setProjectMatrix(project);
	}

	void Init();

	//�����ֱ���
	void Resize(const int& w, const int& h);
	//���������
	void ClearBuffer(const glm::vec4& color);

	void Show();

	//��������
	void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	//ɨ���߷�
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

	//�ϰ벿������������ֵ����ɫ
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//�°벿������������ֵ����ɫ
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//ɨ��ÿ���߲���ɫ
	void ScanLine(const V2F& left, const V2F& right);
};


#endif // !DRAW_H