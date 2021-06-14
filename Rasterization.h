
#ifndef RASTERIZATION_H_
#define RASTERIZATION_H_

#include "Shader.h"
#include "FrameBuffer.h"
#include "Mesh.h"

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
	void SetTexture(Texture* tex)
	{
		shader->texture = tex;
	}

	//调整分辨率
	void Resize(const int& w, const int& h);
	//清除缓存器
	void ClearBuffer(const glm::vec4& color);

	void Show();

	//画三角形
	void DrawMesh(const Mesh& mesh);

	//扫描线法
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);

	//上半部分三角形做插值并着色
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//下半部分三角形做插值并着色
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	//扫描每条线并着色
	void ScanLine(const V2F& left, const V2F& right);

	/**
	* @brief 透视除法，将齐次剪裁空间的坐标全部除以w
	* @param 顶点V2F结构体
	*/
	void PerspectiveDivision(V2F& v);
};


#endif // !DRAW_H