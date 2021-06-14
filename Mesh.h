#pragma once

#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"

class Mesh
{
public:
	std::vector<Vertex> VBO;			//���㻺�����(Vertex Buffer Object)	
	std::vector<unsigned int> EBO;		//�����������(Element Buffer Object)		
	Mesh() = default;
	/**
	* @brief Mesh���캯��������VBO��EBO�Ĵ�С
	* @param ����������������ε�����*2
	*/
	Mesh(const int& vNum, const int& iNum);
	~Mesh() = default;
	Mesh(const Mesh& mesh) :VBO(mesh.VBO), EBO(mesh.EBO) {}
	Mesh& operator = (const Mesh& mesh);
	Mesh& operator += (const Mesh& mesh);

	/**
	* @brief ���Mesh
	* @param ��Ҫ��ӵ�Mesh
	*/
	void AddMesh(const Mesh& mesh);

	/**
	* @brief ���һ��������Mesh
	* @param ��Ҫ��ӵ������ε���������
	*/
	void AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	/**
	* @brief ��������ƽ��Mesh
	* @param һ��ƽ������ϣ����ϣ����£����£�˳ʱ�뷽�򣩶������꣬ƽ��ķ��߷���
	* @return ���ɵ�ƽ������
	*/
	Mesh CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom,
		const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal);

	/**
	* @brief ����һ������Mesh
	* @param �������ĵ����꣬���ĵ㵽һ����ľ���
	* @return ���ɵ���������
	*/
	Mesh CreateBox(const glm::vec3& center, float radius);
};

#endif // !MESH_H