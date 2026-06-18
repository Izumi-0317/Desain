#pragma once
#include "GL.h"
#include <map>
#include <thread>
#include <functional>
class CMultiThreadCore {
private:
	bool		m_loaded;
	std::thread* m_resourceLoader;
	void Thread(std::function<void()> loadProc);
public:
	CMultiThreadCore();
	~CMultiThreadCore();
	void Start(std::function<void()> loadProc);
	bool CheckEnd();

};
class CMultiThread {
	static std::map<int,CMultiThreadCore*> m_instance;
public:
	static CMultiThreadCore* GetInstance(int no=0);
	static void ClearInstance(int no=-1);
	CMultiThread();
	~CMultiThread();
};