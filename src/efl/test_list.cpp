#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include "window.h"
#include "elmtestharness.h"
#include "templates.h"
#include <vector>
#include <linux/input.h>

class List : public EvasObject
{
public:
	List(EvasObject &parent)
		: EvasObject::EvasObject(elm_list_add(parent))
	{
		return;
	}
};

//TODO: Add mouse wheel tests
class UserListTestHarness : public ElmTestHarness
{
public:
	UserListTestHarness()
		: ElmTestHarness::ElmTestHarness()
		, window_("UserListInteractionTest", "User List Interaction Test")
		, list_(elm_list_add(window_))
		, firstItemSelected_(false)
		, secondItemSelected_(false)
		, unselected_(false)
		, edgeTop_(false)
		, edgeBottom_(false)
		, edgeLeft_(false)
		, edgeRight_(false)
		, activated_(false)
		, doubleClicked_(false)
		, longPressed_(false)
		, centerOfWindow_(std::pair<int, int>(0,0))
	{
		for (int i(0); i <= 13; i++) {
			const std::string si = boost::lexical_cast<std::string>(i);
			elm_list_item_append(list_, si.c_str(), NULL, NULL, itemSelectCallback, this); //last pointer is called "Input handle pointer"
		}

		evas_object_size_hint_weight_set(list_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(list_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_resize(list_, window_.getWidth(), window_.getHeight());

		evas_object_smart_callback_add(list_, "unselected", onUnselected, this);
		evas_object_smart_callback_add(list_, "edge,top", onEdgeTop, this);
		evas_object_smart_callback_add(list_, "edge,bottom", onEdgeBottom, this);
		evas_object_smart_callback_add(list_, "edge,left", onEdgeLeft, this);
		evas_object_smart_callback_add(list_, "edge,right", onEdgeRight, this);
		evas_object_smart_callback_add(list_, "clicked,double", onDoubleClick, this);
		evas_object_smart_callback_add(list_, "activated", onActivated, this);
		evas_object_smart_callback_add(list_, "longpressed", onLongpressed, this);

	}

	void setup()
	{
		list_.setPosition(0, 0);
		elm_list_go(list_);
		list_.show();
		window_.show();

		Application::yield(0.01*1e6);

		Geometry geo = getSurfaceGeometry(window_.get_wl_surface());
		Geometry geoListItem;
		Geometry geoFS = window_.getFramespaceGeometry();

		Elm_Object_Item *it = elm_list_first_item_get(list_);
		Evas_Object *eo = elm_list_item_object_get(it);
		evas_object_geometry_get(eo, &geoListItem.x, &geoListItem.y, &geoListItem.width, &geoListItem.height);

		centerOfWindow_.first = geo.x + window_.getWidth() / 2;
		centerOfWindow_.second = geo.y + window_.getHeight() / 2;

		//Place pointer at center of top item of list
		queueStep(
			boost::bind(
				&ElmTestHarness::setGlobalPointerPosition, boost::ref(*this),
				geoFS.x + geo.x + geoListItem.x + geoListItem.width / 2,
				geoFS.y + geo.y + geoListItem.y + geoListItem.height / 2
			)
		);

		queueStep(boost::bind(&UserListTestHarness::runTest, boost::ref(*this)));
	}

	virtual void runTest() = 0;

	static void itemSelectCallback(void* data, Evas_Object* li, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);

		Elm_Object_Item *lit = elm_list_selected_item_get(li);
		std::cout << "...received itemSelectCallback for item: " << elm_object_item_text_get(lit) << std::endl;
		
		if(not strcmp(elm_object_item_text_get(lit), "0"))
			test->firstItemSelected_ = true;
		if(not strcmp(elm_object_item_text_get(lit), "1"))
			test->secondItemSelected_ = true;
	}

	static void onUnselected(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->unselected_ = true;
		std::cout << "...received unselected event" << std::endl;
	}

	static void onEdgeTop(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->edgeTop_ = true;
		std::cout << "...received edge,top event" << std::endl;
	}

	static void onEdgeBottom(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->edgeBottom_ = true;
		std::cout << "...received edge,bottom event" << std::endl;
	}

	static void onEdgeLeft(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->edgeLeft_ = true;
		std::cout << "...received edge,left event" << std::endl;
	}

	static void onEdgeRight(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->edgeRight_ = true;
		std::cout << "...received edge,right event" << std::endl;
	}

	static void onDoubleClick(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->doubleClicked_ = true;
		std::cout << "...received clicked,double event" << std::endl;
	}

	static void onActivated(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->activated_ = true;
		std::cout << "...received activated event" << std::endl;
	}

	static void onLongpressed(void* data, Evas_Object*, void*)
	{
		UserListTestHarness *test = static_cast<UserListTestHarness*>(data);
		test->longPressed_= true;
		std::cout << "...received longpressed event" << std::endl;
	}

protected:
	Window		window_;
	EvasObject	list_;

	bool		firstItemSelected_;
	bool		secondItemSelected_;
	bool		unselected_;
	bool		edgeTop_;
	bool		edgeBottom_;
	bool		edgeLeft_;
	bool		edgeRight_;
	bool		activated_;
	bool		doubleClicked_;
	bool		longPressed_;

	std::pair<int, int> centerOfWindow_;
};

class UserListSelectCase : public UserListTestHarness
{
public:
	void runTest()
	{
		ASSERT(not firstItemSelected_);
		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
		std::cout << "...checking for firstSelected callback" << std::endl;
		while(not firstItemSelected_){
			Application::yield();
		}

		std::cout << "...checking for unselected and secondSelected callback" << std::endl;
		ASSERT(not unselected_);
		ASSERT(not secondItemSelected_);
		while(not unselected_ and not secondItemSelected_){
			inputKeySend(KEY_DOWN, 1);
			inputKeySend(KEY_DOWN, 0);
			Application::yield();
		}
	}
};

class UserListLongpressCase : public UserListTestHarness
{
public:
	void runTest()
	{
		ASSERT(longPressed_ == false);
		inputKeySend(BTN_LEFT, 1);
		//TODO: Investigate time and sleeping for longpress
		std::cout << "...checking for longpressed callback" << std::endl;
		while (not longPressed_) {
			Application::yield();
		}
		inputKeySend(BTN_LEFT, 0);
	}
};

class UserListActivatedTest : public UserListTestHarness
{
public:
	void runTest()
	{
		ASSERT(not activated_);
		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
		inputKeySend(KEY_ENTER, 1);
		inputKeySend(KEY_ENTER, 0);
		std::cout << "...checking for activated callback" << std::endl;
		while(not activated_){
			Application::yield();
		}
	}
};

class UserListDoubleClickTest : public UserListTestHarness
{
public:
	void runTest()
	{
		ASSERT(not doubleClicked_);
		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
		std::cout << "...checking for clicked,double callback" << std::endl;
		while(not doubleClicked_){
			Application::yield();
		}
	}
};

class UserListKeyScrollTest : public UserListTestHarness
{
public:
	void runTest()
	{
		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);

		std::cout << "...checking for edge,top callback" << std::endl;
		ASSERT(not edgeTop_);
		while(not edgeTop_){
			inputKeySend(KEY_UP, 1);
			inputKeySend(KEY_UP, 0);
			Application::yield();
		}

		std::cout << "...checking for edge,bottom callback" << std::endl;
		ASSERT(not edgeBottom_);
		while(not edgeBottom_){
			inputKeySend(KEY_DOWN, 1);
			inputKeySend(KEY_DOWN, 0);
			Application::yield();
		}

		elm_list_horizontal_set(list_, EINA_TRUE);

		std::cout << "...checking for edge,left callback" << std::endl;
		ASSERT(not edgeLeft_);
		while(not edgeLeft_){
			inputKeySend(KEY_LEFT, 1);
			inputKeySend(KEY_LEFT, 0);
			Application::yield();
		}

		std::cout << "...checking for edge,right callback" << std::endl;
		ASSERT(not edgeRight_);
		while(not edgeRight_){
			inputKeySend(KEY_RIGHT, 1);
			inputKeySend(KEY_RIGHT, 0);
			Application::yield();
		}

		elm_list_horizontal_set(list_, EINA_FALSE);
	}
};

class UserListMouseScrollTest : public UserListTestHarness
{
public:
	void runTest()
	{
		int delta(40);

		std::cout << "...checking for edge,top callback" << std::endl;
		setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
		ASSERT(not edgeTop_);
		while(not edgeTop_){
			inputKeySend(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second + delta);
			inputKeySend(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000); //animation has to be displayed before callback is made :-/
		}

		std::cout << "...checking for edge,bottom callback" << std::endl;
		ASSERT(not edgeBottom_);
		while(not edgeBottom_){
			inputKeySend(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second - delta);
			inputKeySend(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}

		elm_list_horizontal_set(list_, EINA_TRUE);

		std::cout << "...checking for edge,left callback" << std::endl;
		ASSERT(not edgeLeft_);
		while(not edgeLeft_){
			inputKeySend(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first + delta, centerOfWindow_.second);
			inputKeySend(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}

		std::cout << "...checking for edge,right callback" << std::endl;
		ASSERT(not edgeRight_);
		while(not edgeRight_){
			inputKeySend(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first - delta, centerOfWindow_.second);
			inputKeySend(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}

		elm_list_horizontal_set(list_, EINA_FALSE);
	}
};


typedef ResizeObjectTest<List> ListResizeTest;
typedef PositionObjectTest<List> ListPositionTest;
typedef VisibleObjectTest<List> ListVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(UserListSelectCase, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(UserListLongpressCase, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(UserListActivatedTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(UserListDoubleClickTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(UserListKeyScrollTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(UserListMouseScrollTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListResizeTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListPositionTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListVisibilityTest, "List")

