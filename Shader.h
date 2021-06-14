
#ifndef SHADER_H_
#define SHADER_H_

#include "Vertex.h"
#include "Texture.h"

//着色器类
//包括顶点着色器，片元着色器
class Shader {
public:
	Shader();
	~Shader() = default; 
	/**
	* @brief 顶点着色器，将Vertex顶点转化为V2F结构体（坐标变换等）
	* @param Vertex顶点
	* @return 新生成的V2F结构体
	*/
	V2F VertexShader(const Vertex& a2v);
	//片元着色
	glm::vec4 FragmentShader(const V2F& v);
	
	void setModelMatrix(const glm::mat4& model);

	void setViewMatrix(const glm::mat4& view);

	void setProjectMatrix(const glm::mat4& project);

	Texture* texture;
	glm::mat4 ModelMatrix;		//Model Space To World Space
	glm::mat4 ViewMatrix;		//World Space To View Space
	glm::mat4 ProjectMatrix;	//View Space To CVV(齐次剪裁空间)
	

};

#endif // !SHADER_H_
