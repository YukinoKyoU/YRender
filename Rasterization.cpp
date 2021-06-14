
#include "Rasterization.h"
#include "FrameBuffer.cpp"

Rasterization::Rasterization(const int& w, const int& h) :
	Width(w), Height(h), FrontBuffer(nullptr)
{}

Rasterization:: ~Rasterization() {
	if (FrontBuffer)
		delete FrontBuffer;
	FrontBuffer = nullptr;
	shader = nullptr;
}
void Rasterization::Init() {
	if (FrontBuffer)
		delete FrontBuffer;
	if (shader)
		delete shader;
	ViewPortMatrix = GetViewPortMatrix(0, 0, Width, Height);
	FrontBuffer = new FrameBuffer(Width, Height);
	shader = new Shader();
}

void Rasterization::Resize(const int& w, const int& h) 
{
	Width = w;
	Height = h;
	FrontBuffer->Resize(w, h);
}

void Rasterization::ClearBuffer(const glm::vec4& color) 
{
	FrontBuffer->ClearColorBuffer(color);
}

void Rasterization::Show() {
	//glDrawPixels函数读取一个地址的内存数据，按指定的格式在屏幕上画一张图
	glDrawPixels(Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, FrontBuffer->colorBuffer.data());
}

void Rasterization::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	//将输入的顶点转化为V2F结构体
	V2F o1 = shader->VertexShader(v1);
	V2F o2 = shader->VertexShader(v2);
	V2F o3 = shader->VertexShader(v3);

	//视口变换
	o1.windowPos = ViewPortMatrix * o1.windowPos;
	o2.windowPos = ViewPortMatrix * o2.windowPos;
	o3.windowPos = ViewPortMatrix * o3.windowPos;

	ScanLineTriangle(o1, o2, o3);
}


void Rasterization::ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
	std::vector<V2F> arr = { v1,v2,v3 };
	if (arr[0].windowPos.y > arr[1].windowPos.y) {
		V2F tmp = arr[0];
		arr[0] = arr[1];
		arr[1] = tmp;
	}
	if (arr[1].windowPos.y > arr[2].windowPos.y) {
		V2F tmp = arr[1];
		arr[1] = arr[2];
		arr[2] = tmp;
	}
	if (arr[0].windowPos.y > arr[1].windowPos.y) {
		V2F tmp = arr[0];
		arr[0] = arr[1];
		arr[1] = tmp;
	}
	//arr[0] 在最下面  arr[2]在最上面
	//中间跟上面的相等，是底三角形
	if (equal(arr[1].windowPos.y, arr[2].windowPos.y)) {
		DownTriangle(arr[1], arr[2], arr[0]);
	}//顶三角形
	else if (equal(arr[1].windowPos.y, arr[0].windowPos.y)) {
		UpTriangle(arr[1], arr[0], arr[2]);
	}
	else {
		//插值求出中间点对面的那个点，划分为两个新的三角形
		float weight = (arr[2].windowPos.y - arr[1].windowPos.y) / (arr[2].windowPos.y - arr[0].windowPos.y);
		V2F newEdge = V2F::lerp(arr[2], arr[0], weight);
		UpTriangle(arr[1], newEdge, arr[2]);
		DownTriangle(arr[1], newEdge, arr[0]);
	}
}

void Rasterization::UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
	V2F left, right, top;
	left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
	right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
	top = v3;
	left.windowPos.x = int(left.windowPos.x);
	int dy = top.windowPos.y - left.windowPos.y;
	int nowY = top.windowPos.y;
	//从上往下插值
	for (int i = dy; i >= 0; i--) {
		float weight = 0;
		if (dy != 0) {
			weight = (float)i / dy;
		}
		V2F newLeft = V2F::lerp(left, top, weight);
		V2F newRight = V2F::lerp(right, top, weight);
		newLeft.windowPos.x = int(newLeft.windowPos.x);
		newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
		newLeft.windowPos.y = newRight.windowPos.y = nowY;
		ScanLine(newLeft, newRight);
		nowY--;
	}
}
void Rasterization::DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3) {
	V2F left, right, bottom;
	left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
	right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
	bottom = v3;
	int dy = left.windowPos.y - bottom.windowPos.y;
	int nowY = left.windowPos.y;
	//从上往下插值
	for (int i = 0; i < dy; i++) {
		float weight = 0;
		if (dy != 0) {
			weight = (float)i / dy;
		}
		V2F newLeft = V2F::lerp(left, bottom, weight);
		V2F newRight = V2F::lerp(right, bottom, weight);
		newLeft.windowPos.x = int(newLeft.windowPos.x);
		newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
		newLeft.windowPos.y = newRight.windowPos.y = nowY;
		ScanLine(newLeft, newRight);
		nowY--;
	}
}

void Rasterization::ScanLine(const V2F& left, const V2F& right) {
	int length = right.windowPos.x - left.windowPos.x;
	for (int i = 0; i < length; i++) {
		V2F v = V2F::lerp(left, right, (float)i / length);
		v.windowPos.x = left.windowPos.x + i;
		v.windowPos.y = left.windowPos.y;

		FrontBuffer->WritePoint(v.windowPos.x, v.windowPos.y, shader->FragmentShader(v));
	}
}


