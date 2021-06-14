#include "Math.h" 

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}

glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}

float Lerp(const float& f1, const float& f2, float factor)
{
	return (1.0 - factor) * f1 + factor * f2;
}

glm::mat4 GetViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up)
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix[0][0] = right.x;
	viewMatrix[1][0] = right.y;
	viewMatrix[2][0] = right.z;
	viewMatrix[3][0] = -glm::dot(right, pos);
	viewMatrix[0][1] = up.x;
	viewMatrix[1][1] = up.y;
	viewMatrix[2][1] = up.z;
	viewMatrix[3][1] = -glm::dot(up, pos);
	viewMatrix[0][2] = -front.x;
	viewMatrix[1][2] = -front.y;
	viewMatrix[2][2] = -front.z;
	viewMatrix[3][2] = glm::dot(front, pos);
	return viewMatrix;

}


glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float& n, const float& f)
{
	glm::mat4 projection = glm::mat4(0.0f);
	const float tanHalfFov = tan(fovy * 0.5f);
	projection[0][0] = 1.0 / (aspect * tanHalfFov);
	projection[1][1] = 1.0f / (tanHalfFov);
	projection[2][2] = -(f + n) / (f - n);
	projection[2][3] = -1.0f;
	projection[3][2] = (-2.0f * n * f) / (f - n);
	return projection;
}


glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height) {

	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = width / 2.0f;
	result[3][0] = ox + (width / 2.0f);
	result[1][1] = height / 2.0f;
	result[3][1] = oy + (height / 2.0f);
	return result;
}

void ViewingFrustumPlanes(std::vector<glm::vec4>& result, const glm::mat4& vp)
{
	//左侧  
	result[0].x = vp[0][3] + vp[0][0];
	result[0].y = vp[1][3] + vp[1][0];
	result[0].z = vp[2][3] + vp[2][0];
	result[0].w = vp[3][3] + vp[3][0];
	//右侧
	result[1].x = vp[0][3] - vp[0][0];
	result[1].y = vp[1][3] - vp[1][0];
	result[1].z = vp[2][3] - vp[2][0];
	result[1].w = vp[3][3] - vp[3][0];
	//上侧
	result[2].x = vp[0][3] - vp[0][1];
	result[2].y = vp[1][3] - vp[1][1];
	result[2].z = vp[2][3] - vp[2][1];
	result[2].w = vp[3][3] - vp[3][1];
	//下侧
	result[3].x = vp[0][3] + vp[0][1];
	result[3].y = vp[1][3] + vp[1][1];
	result[3].z = vp[2][3] + vp[2][1];
	result[3].w = vp[3][3] + vp[3][1];
	//Near
	result[4].x = vp[0][3] + vp[0][2];
	result[4].y = vp[1][3] + vp[1][2];
	result[4].z = vp[2][3] + vp[2][2];
	result[4].w = vp[3][3] + vp[3][2];
	//Far
	result[5].x = vp[0][3] - vp[0][2];
	result[5].y = vp[1][3] - vp[1][2];
	result[5].z = vp[2][3] - vp[2][2];
	result[5].w = vp[3][3] - vp[3][2];
}

bool Point2Plane(const glm::vec3& v, const glm::vec4& p) {
	//点到平面距离 d =  Ax + By + Cz + D;
	// d < 0 点在平面法向反方向所指的区域
	// d > 0 点在平面法向所指的区域
	// d = 0 在平面上
	// d < 0为 false
	return p.x * v.x + p.y * v.y + p.z * v.z + p.w >= 0;
}

bool equal(const float& a, const float& b) {
	float c = a - b;
	if (c > 1e-6 || c < -1e-6) {
		return false;
	}
	else
		return true;
}

float min(const float& a, const float& b)
{
	return a > b ? b : a;
}
float max(const float& a, const float& b)
{
	return a > b ? a : b;
}