
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
	o.worldPos = ModelMatrix * a2v.position;				//计算世界空间下坐标World Space
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;	//计算裁剪空间下坐标(Homogeneous Clip Space)

	o.Z = 1 / o.windowPos.w;

	o.color = a2v.color * o.Z;									//记录颜色信息
	o.normal = a2v.normal * o.Z;								//记录发现信息	
	o.texcoord = a2v.texcoord * o.Z;							//记录纹理信息

	return o;
}

glm::vec4 Shader::FragmentShader(const V2F& v)
{
	glm::vec4 color = texture->Sample2D(v.texcoord);
	return color;
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
