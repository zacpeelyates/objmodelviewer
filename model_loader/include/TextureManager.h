
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <map>
#include <string>

class Texture;

class TextureManager
{
public:
	static TextureManager* CreateInstance();
	static TextureManager* GetInstance();
	static void DestroyInstance();

	inline bool TextureExists(const char* a_pName);
	unsigned int LoadTexture(const char* a_pfilename, bool bIsCubemap = false);
	unsigned int GetTexture(const char* a_pfilename);
	void ReleaseTexture(unsigned int a_texture);
	

private:
	static TextureManager* m_instance;

	struct TextureRef {
		Texture* pTexture;
		unsigned int refCount;
	};


	std::map <std::string, TextureRef> m_TextureMap;

	TextureManager() {};
	~TextureManager();

};
#endif // TEXTUREMANAGER_H
