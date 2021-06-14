
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
	std::vector<float> depthBuffer;				//��Ȼ�������

	FrameBuffer(const int& w = 800, const int& h = 600);

	//�����������ֱ���
	void Resize(const int& w, const int& h);

	//��������յ�ǰ���������ڱ������ɫֵ
	void ClearColorBuffer(const glm::vec4& color);

	//����������ɫ��Ϣд�����ص�
	void WritePoint(const int& x, const int& y, const glm::vec4& color);

	//��������ȡ������buffer�е�ǰ���ص����ֵ
	float GetDepth(const int& x, const int& y);

	//����������ǰ���ص�����ֵд��buffer
	void WriteDepth(const int& x, const int& y, const float& depth);

};

#endif // !FRAMEBUFFER_H_