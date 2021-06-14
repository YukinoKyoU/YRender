
#ifndef RASTERIZATION_H_
#define RASTERIZATION_H_

#include "Shader.h"
#include "FrameBuffer.h"
#include "Mesh.h"
enum RenderMode
{
	Line,
	Fill
};

class Rasterization {

private: 
	RenderMode renderMode;		//��դ��ģʽ

	int Width;					//�ֱ��ʿ�
	int Height;					//�ֱ��ʳ�
	FrameBuffer* FrontBuffer;	//������
	Shader* shader;

	glm::mat4 ViewPortMatrix;

	std::vector<glm::vec4> ViewPlanes;


public:

	Rasterization(const int& w, const int& h);
	~Rasterization();
	//�����ֱ���
	void Resize(const int& w, const int& h);

	void Init();
	void changeRenderMode();
	//������׶
	void UpdateViewPlanes();
	//���������
	void ClearBuffer(const glm::vec4& color);

	void Show();

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
	//��������shader
	void SetTexture(Texture* tex)
	{
		shader->texture = tex;
	}

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
	//���ߺ���
	void DrawLine(const V2F& from, const V2F& to);

	/**
	* @brief ͸�ӳ���������μ��ÿռ������ȫ������w
	* @param ����V2F�ṹ��
	*/
	void PerspectiveDivision(V2F& v);

	/**
	* @brief �����޳�
	* @param ��������Vertex������
	* @return �Ƿ�Ϊ����
	*/
	bool FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);

	/**
	* @brief ��׶�޳�����
	* @param �����εĶ���
	* @return �Ƿ��޳�
	*/
	bool ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);

};


#endif // !DRAW_H