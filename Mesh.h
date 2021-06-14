#pragma once

#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"

class Mesh
{
public:
	std::vector<Vertex> VBO;			//顶点缓冲对象(Vertex Buffer Object)	
	std::vector<unsigned int> EBO;		//索引缓冲对象(Element Buffer Object)		
	Mesh() = default;
	/**
	* @brief Mesh构造函数，调整VBO及EBO的大小
	* @param 顶点的数量，三角形的数量*2
	*/
	Mesh(const int& vNum, const int& iNum);
	~Mesh() = default;
	Mesh(const Mesh& mesh) :VBO(mesh.VBO), EBO(mesh.EBO) {}
	Mesh& operator = (const Mesh& mesh);
	Mesh& operator += (const Mesh& mesh);

	/**
	* @brief 添加Mesh
	* @param 将要添加的Mesh
	*/
	void AddMesh(const Mesh& mesh);

	/**
	* @brief 添加一个三角形Mesh
	* @param 将要添加的三角形的三个顶点
	*/
	void AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	/**
	* @brief 创建单个平面Mesh
	* @param 一个平面的左上，右上，右下，左下（顺时针方向）顶点坐标，平面的法线方向
	* @return 生成的平面网格
	*/
	Mesh CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom,
		const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal);

	/**
	* @brief 创建一个箱子Mesh
	* @param 箱子中心点坐标，中心点到一个面的距离
	* @return 生成的立体网格
	*/
	Mesh CreateBox(const glm::vec3& center, float radius);
};

#endif // !MESH_H