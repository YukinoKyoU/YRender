
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Global.h"

class Texture {

public:
	int width;
	int height;
	int channels;
	unsigned char* data;

	Texture();
	Texture(const std::string& Path);

	~Texture();
	Texture(const Texture& t);
	Texture& operator=(const Texture& t);

	void LoadTexture(const std::string& Path);
	glm::vec4 Sample2D(const glm::vec2& texcoord);

	glm::vec4 GetColor(int x, int y);
};


#endif // !TEXTURE_
