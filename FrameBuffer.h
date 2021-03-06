
#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Global.h"

//帧缓冲数组类
//每一帧生成一个分辨率 宽*高*4 的帧缓冲数组
class FrameBuffer
{
public:
	int Width;
	int Height;		//分辨率
	std::vector<unsigned char> colorBuffer;		//帧缓冲数组
	std::vector<float> depthBuffer;				//深度缓冲数组

	FrameBuffer(const int& w = 800, const int& h = 600);

	//函数：调整分辨率
	void Resize(const int& w, const int& h);

	//函数：清空当前缓冲数组内保存的颜色值
	void ClearColorBuffer(const glm::vec4& color);

	//函数：将颜色信息写入像素点
	void WritePoint(const int& x, const int& y, const glm::vec4& color);

	//函数：读取储存在buffer中当前像素的深度值
	float GetDepth(const int& x, const int& y);

	//函数：将当前像素点的深度值写入buffer
	void WriteDepth(const int& x, const int& y, const float& depth);

};

#endif // !FRAMEBUFFER_H_