#pragma once

class Event 
{
	Event() : m_bHandled(false) {};
	virtual ~Event();
	using DesciptorType = const char*;
	virtual DesciptorType type() const = 0;
	void Handled() { m_bHandled = true; };
	bool IsHandled() { return m_bHandled; };
private:
	bool m_bHandled;
};