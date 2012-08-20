#ifndef __WAYLAND_TEST_SINGLETON_H__
#define __WAYLAND_TEST_SINGLETON_H__

template <class T>
class Singleton
{
public:
	static T& instance()
	{
		static T t;
		return t;
	}
private:
	Singleton(); // non-constructible
	Singleton(const Singleton&); // non-copyable
};

#endif //__WAYLAND_TEST_SINGLETON_H__
