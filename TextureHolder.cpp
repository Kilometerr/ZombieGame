#include "stdafx.h"
#include "TextureHolder.h"
#include <assert.h>

using namespace sf;
using namespace std;

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this; //assign pointer to instance
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	auto& m = m_s_Instance->m_Textures;

	auto keyValuePair = m.find(filename);
	if (keyValuePair != m.end())
	{
		return keyValuePair->second;
	}
	else
	{
		//if filename not found, create new key
		auto& texture = m[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}