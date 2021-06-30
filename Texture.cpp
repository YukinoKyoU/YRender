#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() : width(-1), height(-1), channels(-1), data(nullptr)
{}

Texture::Texture(const std::string& Path) 
{
	LoadTexture(Path);
}

Texture::~Texture() 
{
	if (data)
		free(data);
}

Texture::Texture(const Texture& t) 
{
	width = t.width;
	height = t.height;
	channels = t.channels;
	if (t.data) {
		data = (unsigned char*)malloc(width * height * channels);
		memcpy(data, t.data, width * height * channels);
	}
}

Texture& Texture::operator=(const Texture& t) 
{
	width = t.width;
	height = t.height;
	channels = t.channels;
	if (t.data) {
		data = (unsigned char*)malloc(width * height * channels);
		memcpy(data, t.data, width * height * channels);
	}
	return *this;
}

void Texture::LoadTexture(const std::string& Path) 
{
	stbi_set_flip_vertically_on_load(false);
	if (data)
		free(data);
	data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
}

glm::vec4 Texture::Sample2D(const glm::vec2& texcoord) 
{
	//GL_REPEAT
	float x = texcoord.x - (float)floor(texcoord.x);
	float y = texcoord.y - (float)floor(texcoord.y);
	return GetColor(x * (width - 1), y * (height - 1));
}


glm::vec4 Texture::GetColor(int x, int y)
{
	if (x <0 || x > width || y < 0 || y> height)
		return glm::vec4(0, 0, 0, 0);
	int xy = channels * (y * width + x);
	return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));
}