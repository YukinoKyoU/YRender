
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
	//��ǰ���㻺������Ĵ�С
	int offset = VBO.size();
	//����ӵ�mesh��VBO���ݲ������ڵ�VBO����
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	//����ǰEBO���·���capacity�������ı�����size
	EBO.reserve(EBO.size() + mesh.EBO.size());
	//���ж����� + ����mesh�Ķ�������
	for (int i = 0; i < mesh.EBO.size(); i++) 
	{
		EBO.push_back(mesh.EBO[i] + offset);
	}
}

void Mesh::AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	//��ǰ���㻺������Ĵ�С
	int offset = VBO.size();
	//VBO���������ζ���
	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);
	//EBO���붥�������
	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}

Mesh Mesh::CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal) {
    //������������
    Mesh result(4, 6);
    //���϶�����Ϣ��Vertex�ࣩ
    result.VBO[0].position = glm::vec4(leftTop, 1.0f);
    result.VBO[0].normal = normal;
    result.VBO[0].color = glm::vec4(255, 0, 0, 255);
    result.VBO[0].texcoord = glm::vec2(0.0f, 1.0f);
    //���϶�����Ϣ��Vertex�ࣩ
    result.VBO[1].position = glm::vec4(rightTop, 1.0f);
    result.VBO[1].normal = normal;
    result.VBO[1].color = glm::vec4(0, 255, 0, 255);
    result.VBO[1].texcoord = glm::vec2(1.0f, 1.0f);
    //���¶�����Ϣ��Vertex�ࣩ
    result.VBO[2].position = glm::vec4(rightBottom, 1.0f);
    result.VBO[2].normal = normal;
    result.VBO[2].color = glm::vec4(0, 0, 255, 0);
    result.VBO[2].texcoord = glm::vec2(1.0f, 0.0f);
    //���¶�����Ϣ��Vertex�ࣩ
    result.VBO[3].position = glm::vec4(leftBottom, 1.0f);
    result.VBO[3].normal = normal;
    result.VBO[3].color = glm::vec4(255, 0, 255, 255);
    result.VBO[3].texcoord = glm::vec2(0.0f, 0.0f);

    //���������ε�˳���� ��->��->�� ����˳ʱ�뷽�� 
    //��һ�������εĶ���˳��
    result.EBO[0] = 0;
    result.EBO[1] = 1;
    result.EBO[2] = 2;
    //�ڶ��������εĶ���˳��
    result.EBO[3] = 0;
    result.EBO[4] = 2;
    result.EBO[5] = 3;

    return result;
}

Mesh Mesh::CreateBox(const glm::vec3& center, float radius) {
    Mesh result;
    //������ǰ��
    Mesh front = CreatePlane(
        center + glm::vec3(-radius, radius, radius),        //����
        center + glm::vec3(-radius, -radius, radius),       //����
        center + glm::vec3(radius, -radius, radius),        //����
        center + glm::vec3(radius, radius, radius),         //����
        glm::vec3(0, 0, 1)
    );
    result.AddMesh(front);
    //����������
    Mesh left = CreatePlane(
        center + glm::vec3(-radius, radius, -radius),
        center + glm::vec3(-radius, -radius, -radius),
        center + glm::vec3(-radius, -radius, radius),
        center + glm::vec3(-radius, radius, radius),
        glm::vec3(-1, 0, 0)
    );
    result.AddMesh(left);
    //����������
    Mesh right = CreatePlane(
        center + glm::vec3(radius, radius, radius),
        center + glm::vec3(radius, -radius, radius),
        center + glm::vec3(radius, -radius, -radius),
        center + glm::vec3(radius, radius, -radius),
        glm::vec3(1, 0, 0)
    );
    result.AddMesh(right);
    //���������
    Mesh back = CreatePlane(
        center + glm::vec3(radius, radius, -radius),
        center + glm::vec3(radius, -radius, -radius),
        center + glm::vec3(-radius, -radius, -radius),
        center + glm::vec3(-radius, radius, -radius),
        glm::vec3(0, 0, -1)
    );
    result.AddMesh(back);
    //����������
    Mesh up = CreatePlane(
        center + glm::vec3(-radius, radius, -radius),
        center + glm::vec3(-radius, radius, radius),
        center + glm::vec3(radius, radius, radius),
        center + glm::vec3(radius, radius, -radius),
        glm::vec3(0, 1, 0)
    );
    result.AddMesh(up);
    //����������
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
