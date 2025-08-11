#ifndef MDIE_SINGLETON_H
#define MDIE_SINGLETON_H

#include <mutex>

template<typename t_singleton>
class Singleton {
protected:
	Singleton() {}
	Singleton(Singleton &other) = delete;
	Singleton(const Singleton &other) = delete;
	Singleton &operator=(const Singleton &other) = delete;
	Singleton &operator=(Singleton &other) = delete;
	virtual ~Singleton() {}

	static std::mutex m_mutex;
	static t_singleton *m_instance;

public:
	static t_singleton *get_instance() {
		const std::lock_guard<std::mutex> lock(m_mutex);
		if (m_instance == nullptr) { m_instance = new t_singleton; }
		return m_instance;
	}
	static void delete_instance() {
		const std::lock_guard<std::mutex> lock(m_mutex);
		delete m_instance;
		m_instance = nullptr;
	}
};

template<typename t_singleton>
inline std::mutex Singleton<t_singleton>::m_mutex = std::mutex();
template<typename t_singleton>
inline t_singleton *Singleton<t_singleton>::m_instance = nullptr;

#endif// !MDIE_SINGLETON_H
