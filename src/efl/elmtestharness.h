#ifndef __WAYLAND_EFL_ELMTESTHARNESS_H__
#define __WAYLAND_EFL_ELMTESTHARNESS_H__

#include <boost/function.hpp>
#include <deque>

#include "application.h"

class ElmTestHarness
{
public:
	typedef boost::function<void()>		Callback;
	typedef std::pair<Callback, Callback>	ModifyCheckCallback;
	typedef std::deque<ModifyCheckCallback>	ModifyCheckCallbacks;

	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	/**
	 * This'll kick-off the setup, modify/check, teardown
	 * sequence of calls through idle callbacks.
	 **/
	void run();

	void queueCallback(ModifyCheckCallback);
	
	/**
	 * Optionally override this to do any special
	 * setup before processing ModifyCheckCallbacks.
	 * i.e. you can queue all your callbacks from here.
	 **/
	virtual void setup() { };
	
	/**
	 * Optionally override teardown to do any special
	 * cleanup before the application exits.
	 **/
	virtual void teardown() { };

private:
	static Eina_Bool idleSetup(void*);
	static Eina_Bool doTestSetup(void*, int, void*);

	static Eina_Bool idleModify(void*);
	static Eina_Bool doTestModify(void*, int, void*);

	static Eina_Bool idleCheck(void*);
	static Eina_Bool doTestCheck(void*, int, void*);

	static Eina_Bool idleTeardown(void*);
	static Eina_Bool doTestTeardown(void*, int, void*);

	bool haveMore() const { return callbacks_.size() > 0; }
	void modify();
	void check();
	
	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
	ModifyCheckCallbacks	callbacks_;
};

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness) \
\
BOOST_AUTO_TEST_CASE(Harness##_shm_engine) \
{ \
	Application::setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
} \

//\
//BOOST_AUTO_TEST_CASE(Harness##_egl_engine) \
//{ \
//	Application::setEngine(Application::ENGINE_EGL); \
//	Harness().run(); \
//} \

#define EFL_CHECK_SIZE(obj, w, h)\
		queueCallback( \
			ModifyCheckCallback( \
				boost::bind(&EvasObject::setSize, boost::ref(obj), w, h), \
				boost::bind(&EvasObject::checkSize, boost::ref(obj), w, h) \
			) \
		)

#define EFL_CHECK_POSITION(obj, x, y) \
		queueCallback( \
			ModifyCheckCallback( \
				boost::bind(&EvasObject::setPosition, boost::ref(obj), x, y), \
				boost::bind(&EvasObject::checkPosition, boost::ref(obj), x, y) \
			) \
		)

#define EFL_CHECK_SHOW(obj) \
		queueCallback( \
			ModifyCheckCallback( \
				boost::bind(&EvasObject::show, boost::ref(obj)), \
				boost::bind(&EvasObject::checkVisible, boost::ref(obj), EINA_TRUE) \
			) \
		)

#define EFL_CHECK_HIDE(obj) \
		queueCallback( \
			ModifyCheckCallback( \
				boost::bind(&EvasObject::hide, boost::ref(obj)), \
				boost::bind(&EvasObject::checkHidden, boost::ref(obj), EINA_TRUE) \
			) \
		)

#endif

