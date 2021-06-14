
#ifndef MATH_H_
#define	MATH_H_

#include "Global.h" 


//函数及重载：插值功能的实现
glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor);

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor);

glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor);

float Lerp(const float& f1, const float& f2, float factor);



// Translate Matrix:	Rotation Matrix:
//[1, 0, 0, -x_eye      [right.x,  right.y,  right.z, 0		//x方向向量
// 0, 1, 0, -y_eye	        up.x,     up.y,     up.z, 0		//y方向向量
// 0, 0, 1, -z_eye      -front.x, -front.y  -fornt.z, 0		//z方向向量
// 0, 0, 0,     1]	           0,        0,        0, 1]
// viewMatrix = T * R
/**
 * @brief 世界空间到观察空间矩阵
 * @param 观察点的位置，观察点的x方向，y方向，z（look at）方向
 * @return 观察空间转化矩阵
*/
glm::mat4 GetViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up);

//Matrix:
//[n/r,   0,         0,          0  
//   0, n/t,         0,          0	
//   0,   0,   -(f+n)/(f-n), -2fn/(f-n)     
//   0,   0,        -1,          0]
//r为近平面的右边界，t为近平面的上边界，可根据FOV和aspect求出
/**
 * @brief 观察空间到齐次剪裁空间矩阵
 * @param 相机的FOV，宽高比，近平面，远平面（正数）
 * @return 齐次剪裁空间转化矩阵
*/
glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f);

//Matrix:
//[w/2,   0, 0, w/2 
//   0, h/2, 0, h/2	
//   0,   0, 1,  0     
//   0,   0, 0,  1]
/**
 * @brief 视口变换矩阵
 * @param 屏幕原点的x坐标，y坐标，屏幕宽度，屏幕高度
 * @return 齐次剪裁空间转化矩阵
*/
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);

bool equal(const float& a, const float& b);

float min(const float& a, const float& b);
float max(const float& a, const float& b);


#endif // !MATRIXFORMULA_H_
