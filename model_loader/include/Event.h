#pragma once

class Event 
{
public:
	Event() : m_bHandled(false) {};
	virtual ~Event();
	using DescriptorType = const char*;
	virtual DescriptorType type() const = 0;
	void Handled() { m_bHandled = true; };
	bool IsHandled() { return m_bHandled; };
private:
	bool m_bHandled;
};

//event overloads
class WindowResizeEvent : public Event
{
public:
	virtual ~WindowResizeEvent() {};
	WindowResizeEvent(unsigned int a_width, unsigned int a_height) : m_width(a_width), m_height(a_height) {}
	static constexpr DescriptorType descriptor = "WindowResizeEvent"; //should this be in header file? Jamie makes a dispatcher.cpp for 1 line of code because we dont want to initialize inside header but we initialize this here? :S
	virtual DescriptorType Type() const { return descriptor; };
	inline unsigned int GetWidth() { return m_width; }
	inline unsigned int GetHeight() { return m_height; }
private:
	unsigned int m_width;
	unsigned int m_height;
}