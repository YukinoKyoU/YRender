
#include "Mesh.h"

Mesh::Mesh(const int& vNum, const int& iNum)
{
	VBO.resize(vNum);
	EBO.resize(iNum);
}
Mesh& Mesh::operator = (const Mesh& mesh)
{
	if (&mesh == this)
		return *this;

	VBO = mesh.VBO;
	EBO = mesh.EBO;
	return *this;
}

Mesh& Mesh::operator += (const Mesh& mesh)
{
	AddMesh(mesh);
	return *this;
}

void Mesh::AddMesh(const Mesh& mesh)
{
	//当前顶点缓冲数组的大小
	int offset = VBO.size();
	//将添加的mesh的VBO数据插入现在的VBO后面
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	//给当前EBO重新分配capacity，但不改变现有size
	EBO.reserve(EBO.size() + mesh.EBO.size());
	//现有顶点数 + 插入mesh的顶点索引
	for (int i = 0; i < mesh.EBO.size(); i++) 
	{
		EBO.push_back(mesh.EBO[i] + offset);
	}
}

void Mesh::AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	//当前顶点缓冲数组的大小
	int offset = VBO.size();
	//VBO传入三角形顶点
	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);
	//EBO传入顶点的索引
	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}

Mesh Mesh::CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal) {
    //保存结果的网格
    Mesh result(4, 6);
    //左上顶点信息（Vertex类）
    result.VBO[0].position = glm::vec4(leftTop, 1.0f);
    result.VBO[0].normal = normal;
    result.VBO[0].color = glm::vec4(255, 0, 0, 255);
    result.VBO[0].texcoord = glm::vec2(0.0f, 1.0f);
    //右上顶点信息（Vertex类）
    result.VBO[1].position = glm::vec4(rightTop, 1.0f);
    result.VBO[1].normal = normal;
    result.VBO[1].color = glm::vec4(0, 255, 0, 255);
    result.VBO[1].texcoord = glm::vec2(1.0f, 1.0f);
    //右下顶点信息（Vertex类）
    result.VBO[2].position = glm::vec4(rightBottom, 1.0f);
    result.VBO[2].normal = normal;
    result.VBO[2].color = glm::vec4(0, 0, 255, 0);
    result.VBO[2].texcoord = glm::vec2(1.0f, 0.0f);
    //左下顶点信息（Vertex类）
    result.VBO[3].position = glm::vec4(leftBottom, 1.0f);
    result.VBO[3].normal = normal;
    result.VBO[3].color = glm::vec4(255, 0, 255, 255);
    result.VBO[3].texcoord = glm::vec2(0.0f, 0.0f);

    //绘制三角形的顺序是 上->右->左 都是顺时针方向 
    //第一个三角形的顶点顺序
    result.EBO[0] = 0;
    result.EBO[1] = 1;
    result.EBO[2] = 2;
    //第二个三角形的顶点顺序
    result.EBO[3] = 0;
    result.EBO[4] = 2;
    result.EBO[5] = 3;

    return result;
}

Mesh Mesh::CreateBox(const glm::vec3& center, float radius) {
    Mesh result;
    //立方体前面
    Mesh front = CreatePlane(
        center + glm::vec3(-radius, radius, radius),        //左上
        center + glm::vec3(-radius, -radius, radius),       //左下
        center + glm::vec3(radius, -radius, radius),        //右下
        center + glm::vec3(radius, radius, radius),         //右上
        glm::vec3(0, 0, 1)
    );
    result.AddMesh(front);
    //立方体左面
    Mesh left = CreatePlane(
        center + glm::vec3(-radius, radius, -radius),
        center + glm::vec3(-radius, -radius, -radius),
        center + glm::vec3(-radius, -radius, radius),
        center + glm::vec3(-radius, radius, radius),
        glm::vec3(-1, 0, 0)
    );
    result.AddMesh(left);
    //立方体右面
    Mesh right = CreatePlane(
        center + glm::vec3(radius, radius, radius),
        center + glm::vec3(radius, -radius, radius),
        center + glm::vec3(radius, -radius, -radius),
        center + glm::vec3(radius, radius, -radius),
        glm::vec3(1, 0, 0)
    );
    result.AddMesh(right);
    //立方体后面
    Mesh back = CreatePlane(
        center + glm::vec3(radius, radius, -radius),
        center + glm::vec3(radius, -radius, -radius),
        center + glm::vec3(-radius, -radius, -radius),
        center + glm::vec3(-radius, radius, -radius),
        glm::vec3(0, 0, -1)
    );
    result.AddMesh(back);
    //立方体上面
    Mesh up = CreatePlane(
        center + glm::vec3(-radius, radius, -radius),
        center + glm::vec3(-radius, radius, radius),
        center + glm::vec3(radius, radius, radius),
        center + glm::vec3(radius, radius, -radius),
        glm::vec3(0, 1, 0)
    );
    result.AddMesh(up);
    //立方体下面
    Mesh down = CreatePlane(
        center + glm::vec3(-radius, -radius, radius),
        center + glm::vec3(-radius, -radius, -radius),
        center + glm::vec3(radius, -radius, -radius),
        center + glm::vec3(radius, -radius, radius),
        glm::vec3(0, -1, 0)
    );
    result.AddMesh(down);

    return result;
}
