
#ifndef MATH_H_
#define	MATH_H_

#include "Global.h" 


//���������أ���ֵ���ܵ�ʵ��
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor);

float Lerp(const float& f1, const float& f2, float factor);



// Translate Matrix:	Rotation Matrix:
//[1, 0, 0, -x_eye      [right.x,  right.y,  right.z, 0		//x��������
// 0, 1, 0, -y_eye	        up.x,     up.y,     up.z, 0		//y��������
// 0, 0, 1, -z_eye      -front.x, -front.y  -fornt.z, 0		//z��������
// 0, 0, 0,     1]	           0,        0,        0, 1]
// viewMatrix = T * R
/**
 * @brief ����ռ䵽�۲�ռ����
 * @param �۲���λ�ã��۲���x����y����z��look at������
 * @return �۲�ռ�ת������
*/
glm::mat4 GetViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up);

//Matrix:
//[n/r,   0,         0,          0  
//   0, n/t,         0,          0	
//   0,   0,   -(f+n)/(f-n), -2fn/(f-n)     
//   0,   0,        -1,          0]
//rΪ��ƽ����ұ߽磬tΪ��ƽ����ϱ߽磬�ɸ���FOV��aspect���
/**
 * @brief �۲�ռ䵽��μ��ÿռ����
 * @param �����FOV����߱ȣ���ƽ�棬Զƽ�棨������
 * @return ��μ��ÿռ�ת������
*/
glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f);

//Matrix:
//[w/2,   0, 0, w/2 
//   0, h/2, 0, h/2	
//   0,   0, 1,  0     
//   0,   0, 0,  1]
/**
 * @brief �ӿڱ任����
 * @param ��Ļԭ���x���꣬y���꣬��Ļ��ȣ���Ļ�߶�
 * @return ��μ��ÿռ�ת������
*/
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);

bool equal(const float& a, const float& b);

float min(const float& a, const float& b);
float max(const float& a, const float& b);


#endif // !MATRIXFORMULA_H_
