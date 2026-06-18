#include "CMultiThread.h"

std::map<int, CMultiThreadCore*> CMultiThread::m_instance;
CMultiThreadCore::CMultiThreadCore() : m_resourceLoader(nullptr), m_loaded(false)
{

	if(m_resourceLoader ) delete m_resourceLoader;
	m_resourceLoader = nullptr;
}

CMultiThreadCore::~CMultiThreadCore()
{
	if (m_resourceLoader)
	{
		m_resourceLoader->join();
		delete m_resourceLoader;
	}
	m_resourceLoader = nullptr;
}

void CMultiThreadCore::Thread(std::function<void()> loadProc) {

	loadProc();
	m_loaded = true;
}
void CMultiThreadCore::Start(std::function<void()> loadProc) {
	m_loaded = false;
	if (m_resourceLoader) {
		m_resourceLoader->join();
		delete m_resourceLoader;
		m_resourceLoader = nullptr;
	}


	m_resourceLoader = new  std::thread(std::bind(&CMultiThreadCore::Thread, this, loadProc));



}
bool CMultiThreadCore::CheckEnd() {
	return (!m_resourceLoader || m_loaded);
}

CMultiThread::CMultiThread() {

}
CMultiThread::~CMultiThread() {
	ClearInstance();
}
CMultiThreadCore* CMultiThread::GetInstance(int no) {
	if (!m_instance[no]) m_instance[no] = new CMultiThreadCore();
	return m_instance[no];
}
void CMultiThread::ClearInstance(int no) {
	if (no >= 0) {
		auto c = m_instance.find(no);
		if (c != m_instance.end()) {
			delete c->second;
			m_instance.erase(no);
		}
	} else {
		for (auto& c : m_instance) {
			if (!c.second) delete c.second;
		}
		m_instance.clear();
	}
}