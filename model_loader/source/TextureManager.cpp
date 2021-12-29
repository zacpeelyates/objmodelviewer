#include "TextureManager.h"
#include "Texture.h"
#include <iostream>

TextureManager* TextureManager::m_instance = nullptr;

TextureManager* TextureManager::CreateInstance()
{
	if (m_instance == nullptr) 
	{
		m_instance = new TextureManager();
	}
	return m_instance;
}

TextureManager* TextureManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = CreateInstance();
	}
	return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (m_instance != nullptr) 
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

inline bool TextureManager::TextureExists(const char* a_pName)
{
	return m_TextureMap.find(a_pName) != m_TextureMap.end();
}

unsigned int TextureManager::LoadTexture(const char* a_pfilename)
{
	if (a_pfilename != nullptr)
	{
		auto dictionaryIter = m_TextureMap.find(a_pfilename);
		if (dictionaryIter != m_TextureMap.end()) 
		{
			//existing texture, increment reference count
			TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
			++texRef.refCount;
			return texRef.pTexture->GetTextureID();
		}
		else 
		{
			//load in new texture
			Texture* pTexture = new Texture();
			if (pTexture->Load(a_pfilename))
			{
				TextureRef texRef = { pTexture, 1 };
				m_TextureMap[a_pfilename] = texRef;
				return pTexture->GetTextureID();
			}
			else 
			{
				//failed to load
				std::cout << "Failed to Load Texture! " << a_pfilename << std::endl;
				delete pTexture;
				return 0;
			}
		}
	}
	return 0;
}

unsigned int TextureManager::GetTexture(const char* a_pfilename)
{
	auto dictionaryIter = m_TextureMap.find(a_pfilename);
	if (dictionaryIter != m_TextureMap.end()) 
	{
		TextureRef& texRef = (TextureRef&)dictionaryIter->second;
		texRef.refCount++;
		return texRef.pTexture->GetTextureID();
	}
	return 0;
}

void TextureManager::ReleaseTexture(unsigned int a_texture)
{
	for (auto dictionaryIter = m_TextureMap.begin(); dictionaryIter != m_TextureMap.end(); ++dictionaryIter)
	{
		TextureRef currentRef = (TextureRef&)dictionaryIter->second;
		if (a_texture == currentRef.pTexture->GetTextureID())
		{
			if (--currentRef.refCount == 0) 
			{
				delete currentRef.pTexture;
				currentRef.pTexture = nullptr;
				m_TextureMap.erase(dictionaryIter);
				break;
			}
		}
	}
}

TextureManager::~TextureManager()
{
	m_TextureMap.clear();
}
