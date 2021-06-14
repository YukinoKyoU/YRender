
#ifndef VERTEX_H_
#define VERTEX_H_

#include "Math.h"

//顶点数据
class Vertex
{
public:
	glm::vec4 position;		//模型空间下坐标（Model Space）
	glm::vec4 color;		//顶点颜色
	glm::vec2 texcoord;		//顶点纹理坐标
	glm::vec3 normal;		//顶点法线

	Vertex() = default;
	~Vertex() = default;

	/** 
	* @brief 顶点数据构造函数
	* @param 顶点坐标，顶点颜色信息，顶点纹理坐标，顶点发现信息
	*/
	Vertex(
		const glm::vec4& _pos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
	) :
		position(_pos), color(_color), texcoord(_tex), normal(_normal) {}


	Vertex(
		const glm::vec3& _pos,
		const glm::vec4& _color = glm::vec4(0, 0, 0, 0),
		const glm::vec2& _tex = glm::vec2(0, 0),
		const glm::vec3& _normal = glm::vec3(0, 0, 1)
	) :
		position(_pos, 1.0f), color(_color), texcoord(_tex), normal(_normal) {}

	Vertex(const Vertex& v) :position(v.position), color(v.color), texcoord(v.texcoord), normal(v.normal) {}
};

//vertex shader to fragment
//V2F结构体：用来存放计算结果，把顶点着色器的计算结果传递到片元着色器。
class V2F {
public:
	glm::vec4 worldPos;		//顶点的世界空间下的坐标
	glm::vec4 windowPos;		//顶点裁剪(投影)空间下坐标(Homogeneous Clip Space)
	glm::vec4 color;		//顶点颜色信息
	glm::vec2 texcoord;		//顶点纹理信息
	glm::vec3 normal;		//顶点法线信息

	float Z;

	V2F() = default;
	~V2F() = default;

	/**
	* @brief V2F结构体构造函数
	* @param 顶点世界空间坐标，剪裁空间下坐标，颜色信息，纹理信息，发现信息
	*/
	V2F(
		const glm::vec4& _wPos,
		const glm::vec4& _pPos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
	) :
		worldPos(_wPos), windowPos(_pPos), color(_color), texcoord(_tex), normal(_normal) {}

	V2F(const V2F& v) :
		worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texcoord(v.texcoord), normal(v.normal), Z(v.Z) {}

	/**
	* @brief 顶点插值函数，根据两个点得到两个点之间所有点包含的信息
	* @param 两个V2F结构体，插值的系数
	* @return 新生成的顶点的V2F结构体
	*/
	static V2F lerp(const V2F& v1, const V2F& v2, const float& factor) {
		V2F result;
		//新顶点V2F结构体的世界空间坐标
		result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
		//新顶点V2F结构体的剪裁空间坐标
		result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
		//新顶点V2F结构体的裁剪空间坐标
		result.color = Lerp(v1.color, v2.color, factor);
		//新顶点V2F结构体的纹理信息
		result.texcoord = Lerp(v1.texcoord, v2.texcoord, factor);
		//新顶点V2F结构体的法线
		result.normal = Lerp(v1.normal, v2.normal, factor);
		//新顶点V2F结构体的深度值
		result.Z = Lerp(v1.Z, v2.Z, factor);
		return result;
	}
};

#endif // !VERTEX_H_
