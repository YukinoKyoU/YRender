
#include "Shader.h"

Shader::Shader()
{
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectMatrix = glm::mat4(1.0f);
}

V2F Shader::VertexShader(const Vertex& a2v) 
{
	V2F o; 
	o.worldPos = ModelMatrix * a2v.position;				//��������ռ�������World Space
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;	//����ü��ռ�������(Homogeneous Clip Space)

	o.color = a2v.color;									//��¼��ɫ��Ϣ
	o.normal = a2v.normal;									//��¼������Ϣ	
	o.texcoord = a2v.texcoord;								//��¼������Ϣ

	return o;
}

glm::vec4 Shader::FragmentShader(const V2F& v)
{
	return v.color;
}

void Shader::setModelMatrix(const glm::mat4& model)
{
	ModelMatrix = model;
}

void Shader::setViewMatrix(const glm::mat4& view)
{
	ViewMatrix = view;
}

void Shader::setProjectMatrix(const glm::mat4& project)
{
	ProjectMatrix = project;
}
