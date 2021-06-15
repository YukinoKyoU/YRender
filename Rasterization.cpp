
#include "Rasterization.h"
#include "FrameBuffer.cpp"

Rasterization::Rasterization(const int& w, const int& h) :
	Width(w), Height(h), FrontBuffer(nullptr), renderMode(Line)
{
	ViewPlanes.resize(6, glm::vec4(0));
}

Rasterization:: ~Rasterization() {
	if (FrontBuffer)
		delete FrontBuffer;
	FrontBuffer = nullptr;
	shader = nullptr;
}

void Rasterization::Resize(const int& w, const int& h)
{
	Width = w;
	Height = h;
	FrontBuffer->Resize(w, h);
}

void Rasterization::Init() 
{
	if (FrontBuffer)
		delete FrontBuffer;
	if (shader)
		delete shader;
	ViewPortMatrix = GetViewPortMatrix(0, 0, Width, Height);
	FrontBuffer = new FrameBuffer(Width, Height);
	shader = new Shader();
}

void Rasterization::changeRenderMode()
{
	if (renderMode == Fill)
		renderMode = Line;
	else
		renderMode = Fill;
}

void Rasterization::UpdateViewPlanes()
{
	ViewingFrustumPlanes(ViewPlanes, shader->ProjectMatrix * shader->ViewMatrix);
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

		if (!ViewCull(v1.worldPos / v1.Z, v2.worldPos / v2.Z, v3.worldPos / v3.Z)) {
			continue;
		}

		//做透视除法，将坐标变换到NDC空间中
		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);

	
		//视口变换
		v1.windowPos = ViewPortMatrix * v1.windowPos;
		v2.windowPos = ViewPortMatrix * v2.windowPos;
		v3.windowPos = ViewPortMatrix * v3.windowPos;

		//切换画线or填充
		if (renderMode == Line) {
			DrawLine(v1, v2);
			DrawLine(v2, v3);
			DrawLine(v3, v1);
		}
		else {

			if (!FaceCulling(v1.windowPos, v2.windowPos, v3.windowPos)) {
				continue;
			}
			ScanLineTriangle(v1, v2, v3);
		}
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
			float z = v.Z;
			v.worldPos /= z;
			v.texcoord /= z;
			v.normal /= z;
			v.color /= z;

			FrontBuffer->WritePoint(v.windowPos.x, v.windowPos.y, shader->FragmentShader(v));
			FrontBuffer->WriteDepth(v.windowPos.x, v.windowPos.y, v.windowPos.z);
		}
	}
}

void Rasterization::DrawLine(const V2F& from, const V2F& to)
{
	int dx = to.windowPos.x - from.windowPos.x;
	int dy = to.windowPos.y - from.windowPos.y;
	int Xstep = 1, Ystep = 1;
	if (dx < 0)
	{
		Xstep = -1;
		dx = -dx;
	}
	if (dy < 0)
	{
		Ystep = -1;
		dy = -dy;
	}
	int currentX = from.windowPos.x;
	int currentY = from.windowPos.y;
	V2F tmp;
	//斜率小于1
	if (dy <= dx)
	{
		int P = 2 * dy - dx;
		for (int i = 0; i <= dx; ++i)
		{
			tmp = V2F::lerp(from, to, ((float)(i) / dx));
			FrontBuffer->WritePoint(currentX, currentY, glm::vec4(255, 0, 0, 0));
			currentX += Xstep;
			if (P <= 0)
				P += 2 * dy;
			else
			{
				currentY += Ystep;
				P += 2 * (dy - dx);
			}
		}
	}
	//斜率大于1，利用对称性画
	else
	{
		int P = 2 * dx - dy;
		for (int i = 0; i <= dy; ++i)
		{
			tmp = V2F::lerp(from, to, ((float)(i) / dy));
			FrontBuffer->WritePoint(currentX, currentY, glm::vec4(255, 0, 0, 0));
			currentY += Ystep;
			if (P <= 0)
				P += 2 * dx;
			else
			{
				currentX += Xstep;
				P -= 2 * (dy - dx);
			}
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

bool Rasterization::FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {

	//用按逆时针方向用其他两点的坐标减去这点，获得该点两边的向量
	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	//两向量叉乘获得面法向量
	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));

	//在NDC下，观察方向 是0,0,1
	glm::vec3 view = glm::vec3(0, 0, 1);
	//如果该面法向与观察方向（该点坐标减去摄像机位置）的夹角小于90度，证明是背向面
	return glm::dot(normal, view) < 0;
}

bool Rasterization::ViewCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {
	//包围盒的两个顶点
	glm::vec3 minPoint, maxPoint;
	//最左边为三个点中最小值
	minPoint.x = min(v1.x, min(v2.x, v3.x));
	minPoint.y = min(v1.y, min(v2.y, v3.y));
	minPoint.z = min(v1.z, min(v2.z, v3.z));
	//最右边为三个点中最大值的向上取整
	maxPoint.x = max(v1.x, max(v2.x, v3.x));
	maxPoint.y = max(v1.y, max(v2.y, v3.y));
	maxPoint.z = max(v1.z, max(v2.z, v3.z));
	// Near 和 Far 剔除时只保留完全在内的
	if (!Point2Plane(minPoint, ViewPlanes[4]) || !Point2Plane(maxPoint, ViewPlanes[4])) {
		return false;
	}
	if (!Point2Plane(minPoint, ViewPlanes[5]) || !Point2Plane(maxPoint, ViewPlanes[5])) {
		return false;
	}
	if (!Point2Plane(minPoint, ViewPlanes[0]) && !Point2Plane(maxPoint, ViewPlanes[0])) {
		return false;
	}
	if (!Point2Plane(minPoint, ViewPlanes[1]) && !Point2Plane(maxPoint, ViewPlanes[1])) {
		return false;
	}
	if (!Point2Plane(minPoint, ViewPlanes[2]) && !Point2Plane(maxPoint, ViewPlanes[2])) {
		return false;
	}
	if (!Point2Plane(minPoint, ViewPlanes[3]) && !Point2Plane(maxPoint, ViewPlanes[3])) {
		return false;
	}
	return true;
}


