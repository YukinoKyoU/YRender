
#ifndef RASTERIZATION_H_
#define RASTERIZATION_H_

#include "Shader.h"
#include "FrameBuffer.h"

class Rasterization {

private: 

	int Width;					//分辨率宽
	int Height;					//分辨率长
	FrameBuffer* FrontBuffer;	//缓存器
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

	//调整分辨率
	void Resize(const int& w, const int& h);
	//清除缓存器
	void ClearBuffer(const glm::vec4& color);

	void Show();

	//画三角形
	void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	//扫描线法
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

	//上半部分三角形做插值并着色
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//下半部分三角形做插值并着色
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//扫描每条线并着色
	void ScanLine(const V2F& left, const V2F& right);
};


#endif // !DRAW_H