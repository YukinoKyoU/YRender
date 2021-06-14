
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

void Rasterization::DrawMesh(const Mesh& mesh) {
	if (mesh.EBO.empty()) {
		return;
	}

	for (int i = 0; i < mesh.EBO.size(); i += 3) {
		Vertex p1, p2, p3;
		p1 = mesh.VBO[mesh.EBO[i]];
		p2 = mesh.VBO[mesh.EBO[i + 1]];
		p3 = mesh.VBO[mesh.EBO[i + 2]];

		//将输入的顶点转化为V2F结构体
		V2F v1, v2, v3;
		v1 = shader->VertexShader(p1);
		v2 = shader->VertexShader(p2);
		v3 = shader->VertexShader(p3);

		//做透视除法，将坐标变换到NDC空间中
		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);

		//视口变换
		v1.windowPos = ViewPortMatrix * v1.windowPos;
		v2.windowPos = ViewPortMatrix * v2.windowPos;
		v3.windowPos = ViewPortMatrix * v3.windowPos;
		ScanLineTriangle(v1, v2, v3);
	}
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

		float depth = FrontBuffer->GetDepth(v.windowPos.x, v.windowPos.y);
		if (v.windowPos.z < depth) 
		{
			FrontBuffer->WritePoint(v.windowPos.x, v.windowPos.y, shader->FragmentShader(v));
			FrontBuffer->WriteDepth(v.windowPos.x, v.windowPos.y, v.windowPos.z);
		}
	}
}

void Rasterization::PerspectiveDivision(V2F& v) 
{
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;

	//透视除法之后Z除了深度测试已经没用了,将深度值归一化[0,1]
	v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
}


