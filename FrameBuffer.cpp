
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const int& w, const int& h)
{
	Width = w;
	Height = h;
	colorBuffer.resize(w* h * 4.0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::Resize(const int& w, const int& h)
{
	Width = w;
	Height = h;
	colorBuffer.resize(w * h * 4, 0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::ClearColorBuffer(const glm::vec4& color)
{
	unsigned char* p = colorBuffer.data();//传入缓冲数组的地址
	for (int i = 0; i < Width * Height * 4; i += 4)
	{
		*(p + i + 0) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
	}
	depthBuffer.assign(Width * Height, 1.0f);
}

void FrameBuffer::WritePoint(const int& x, const int& y, const glm::vec4& color)
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;

	int i = (y * Width + x);	//（x，y）像素点所对应的缓冲数组的下标
	unsigned char* p = colorBuffer.data();
	*(p + i * 4 + 0) = color.r ;
	*(p + i * 4 + 1) = color.g ;
	*(p + i * 4 + 2) = color.b ;
	*(p + i * 4 + 3) = color.a ;
}

float FrameBuffer::GetDepth(const int& x, const int& y)
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return 1.0f;

	return *(depthBuffer.data() + y * Width + x);
}

void FrameBuffer::WriteDepth(const int& x, const int& y, const float& depth)
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	float* p = depthBuffer.data();
	*(p + y * Width + x) = depth;
}

