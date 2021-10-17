
struct GLFWwindow;
class Application
{
public:
	Application() : m_window(nullptr), m_windowWidth(0), m_windowHeight(0), m_running(false) {}
	virtual ~Application() {}
	bool Create(const char* ap_appName, unsigned int a_winWidth, unsigned int a_winHeight, bool fullscreen);
	void Run(const char* ap_appName, unsigned int a_winWidth, unsigned int a_winHeight, bool fullscreen);
	void Quit() { m_running = false; };

protected:
	virtual bool onCreate() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;

	GLFWwindow* m_window;
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
	bool m_running;

};