
#ifndef SHADER_H_
#define SHADER_H_

#include "Vertex.h"
#include "Texture.h"

//��ɫ����
//����������ɫ����ƬԪ��ɫ��
class Shader {
public:
	Shader();
	~Shader() = default; 
	/**
	* @brief ������ɫ������Vertex����ת��ΪV2F�ṹ�壨����任�ȣ�
	* @param Vertex����
	* @return �����ɵ�V2F�ṹ��
	*/
	V2F VertexShader(const Vertex& a2v);
	//ƬԪ��ɫ
	glm::vec4 FragmentShader(const V2F& v);
	
	void setModelMatrix(const glm::mat4& model);

	void setViewMatrix(const glm::mat4& view);

	void setProjectMatrix(const glm::mat4& project);

	Texture* texture;
	glm::mat4 ModelMatrix;		//Model Space To World Space
	glm::mat4 ViewMatrix;		//World Space To View Space
	glm::mat4 ProjectMatrix;	//View Space To CVV(��μ��ÿռ�)
	

};

#endif // !SHADER_H_
