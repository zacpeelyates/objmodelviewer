#include "Application.h"
#include <glm\glm.hpp>

class ModelLoaderApp : public Application
{
public:
	ModelLoaderApp();
	virtual ~ModelLoaderApp();
protected:
	virtual bool onCreate();
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void Destroy();
private:
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	unsigned int m_uiProgram;
};