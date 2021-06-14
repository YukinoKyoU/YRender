
#ifndef RASTERIZATION_H_
#define RASTERIZATION_H_

#include "Shader.h"
#include "FrameBuffer.h"
#include "Mesh.h"

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
	void SetTexture(Texture* tex)
	{
		shader->texture = tex;
	}

	//�����ֱ���
	void Resize(const int& w, const int& h);
	//���������
	void ClearBuffer(const glm::vec4& color);

	void Show();

	//��������
	void DrawMesh(const Mesh& mesh);

	//ɨ���߷�
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

	//�ϰ벿������������ֵ����ɫ
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//�°벿������������ֵ����ɫ
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//ɨ��ÿ���߲���ɫ
	void ScanLine(const V2F& left, const V2F& right);

	/**
	* @brief ͸�ӳ���������μ��ÿռ������ȫ������w
	* @param ����V2F�ṹ��
	*/
	void PerspectiveDivision(V2F& v);
};


#endif // !DRAW_H