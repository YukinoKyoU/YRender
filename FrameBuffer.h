
#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Global.h"

//֡����������
//ÿһ֡����һ���ֱ��� ��*��*4 ��֡��������
class FrameBuffer
{
public:
	int Width;
	int Height;		//�ֱ���
	std::vector<unsigned char> colorBuffer;		//֡��������

	FrameBuffer(const int& w = 800, const int& h = 600);

	//�����������ֱ���
	void Resize(const int& w, const int& h);

	//��������յ�ǰ���������ڱ������ɫֵ
	void ClearColorBuffer(const glm::vec4& color);

	//����������ɫ��Ϣд�����ص�
	void WritePoint(const int& x, const int& y, const glm::vec4& color);

};

#endif // !FRAMEBUFFER_H_