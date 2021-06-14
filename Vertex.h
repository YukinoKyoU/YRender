
#ifndef VERTEX_H_
#define VERTEX_H_

#include "Math.h"

//��������
class Vertex
{
public:
	glm::vec4 position;		//ģ�Ϳռ������꣨Model Space��
	glm::vec4 color;		//������ɫ
	glm::vec2 texcoord;		//������������
	glm::vec3 normal;		//���㷨��

	Vertex() = default;
	~Vertex() = default;

	/** 
	* @brief �������ݹ��캯��
	* @param �������꣬������ɫ��Ϣ�������������꣬���㷢����Ϣ
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
//V2F�ṹ�壺������ż��������Ѷ�����ɫ���ļ��������ݵ�ƬԪ��ɫ����
class V2F {
public:
	glm::vec4 worldPos;		//���������ռ��µ�����
	glm::vec4 windowPos;		//����ü�(ͶӰ)�ռ�������(Homogeneous Clip Space)
	glm::vec4 color;		//������ɫ��Ϣ
	glm::vec2 texcoord;		//����������Ϣ
	glm::vec3 normal;		//���㷨����Ϣ

	float Z;

	V2F() = default;
	~V2F() = default;

	/**
	* @brief V2F�ṹ�幹�캯��
	* @param ��������ռ����꣬���ÿռ������꣬��ɫ��Ϣ��������Ϣ��������Ϣ
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
	* @brief �����ֵ����������������õ�������֮�����е��������Ϣ
	* @param ����V2F�ṹ�壬��ֵ��ϵ��
	* @return �����ɵĶ����V2F�ṹ��
	*/
	static V2F lerp(const V2F& v1, const V2F& v2, const float& factor) {
		V2F result;
		//�¶���V2F�ṹ�������ռ�����
		result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
		//�¶���V2F�ṹ��ļ��ÿռ�����
		result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
		//�¶���V2F�ṹ��Ĳü��ռ�����
		result.color = Lerp(v1.color, v2.color, factor);
		//�¶���V2F�ṹ���������Ϣ
		result.texcoord = Lerp(v1.texcoord, v2.texcoord, factor);
		//�¶���V2F�ṹ��ķ���
		result.normal = Lerp(v1.normal, v2.normal, factor);
		//�¶���V2F�ṹ������ֵ
		result.Z = Lerp(v1.Z, v2.Z, factor);
		return result;
	}
};

#endif // !VERTEX_H_
