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
class UserListInteractionTest : public ElmTestHarness
{
public:
	UserListInteractionTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("UserListInteractionTest", "User List Interaction Test")
		, list_(elm_list_add(window_))
		, itemSelected_(false)
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
		std::vector<std::string> lbl;
		
		for (int i(0); i <= 13; i++)
			lbl.push_back(boost::lexical_cast<std::string>(i));
		
		foreach (std::string s, lbl)
			elm_list_item_append(list_, s.c_str(), NULL, NULL, itemSelectCallback, this); //last pointer is called "Input handle pointer"
		
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
		
		queueStep(boost::bind(&UserListInteractionTest::addEventsTest, boost::ref(*this)));
		queueStep(boost::bind(&UserListInteractionTest::addKeyScrollTests, boost::ref(*this)));
		queueStep(boost::bind(&UserListInteractionTest::resetCallbacks, boost::ref(*this)));
		queueStep(boost::bind(&UserListInteractionTest::addMouseScrollTests, boost::ref(*this)));
	}
	
	void addEventsTest()
	{
		pointerKeyPress(BTN_LEFT, 1);
		pointerKeyPress(BTN_LEFT, 0);
		std::cout << "...checking for firstSelected callback" << std::endl;
		while(!itemSelected_){
			Application::yield();
		}
		
		pointerKeyPress(BTN_LEFT, 1);
		std::cout << "...checking for longpressed callback" << std::endl;
		while(!longPressed_){
			Application::yield();
		}
		pointerKeyPress(BTN_LEFT, 0);
		
		pointerKeyPress(KEY_ENTER, 1);
		pointerKeyPress(KEY_ENTER, 0);
		std::cout << "...checking for activated callback" << std::endl;
		while(!activated_){
			Application::yield();
		}
		
		pointerKeyPress(BTN_LEFT, 1);
		pointerKeyPress(BTN_LEFT, 0);
		pointerKeyPress(BTN_LEFT, 1);
		pointerKeyPress(BTN_LEFT, 0);
		std::cout << "...checking for clicked,double callback" << std::endl;
		while(!doubleClicked_){
			Application::yield();
		}
		
		std::cout << "...checking for unselected callback" << std::endl;
		while(!unselected_){
			pointerKeyPress(KEY_DOWN, 1);
			pointerKeyPress(KEY_DOWN, 0);
			Application::yield();
		}

	}
	
	void addKeyScrollTests()
	{
		std::cout << "...checking for edge,top callback" << std::endl;
		while(!edgeTop_){
			pointerKeyPress(KEY_UP, 1);
			pointerKeyPress(KEY_UP, 0);
			Application::yield();
		}

		std::cout << "...checking for edge,bottom callback" << std::endl;
		while(!edgeBottom_){
			pointerKeyPress(KEY_DOWN, 1);
			pointerKeyPress(KEY_DOWN, 0);
			Application::yield();
		}
		
		elm_list_horizontal_set(list_, EINA_TRUE);
		
		std::cout << "...checking for edge,left callback" << std::endl;
		while(!edgeLeft_){
			pointerKeyPress(KEY_LEFT, 1);
			pointerKeyPress(KEY_LEFT, 0);
			Application::yield();
		}

		std::cout << "...checking for edge,right callback" << std::endl;
		while(!edgeRight_){
			pointerKeyPress(KEY_RIGHT, 1);
			pointerKeyPress(KEY_RIGHT, 0);
			Application::yield();
		}
		
		elm_list_horizontal_set(list_, EINA_FALSE);
	}
	
	void resetCallbacks()
	{
		itemSelected_ = false;
		unselected_ = false;
		edgeTop_ = false;
		edgeBottom_ = false;
		edgeLeft_ = false;
		edgeRight_ = false;
		activated_ = false;
		doubleClicked_ = false;
		longPressed_ = false;
	}
	
	void addMouseScrollTests()
	{	
		int delta(40);
		
		std::cout << "...checking for edge,top callback" << std::endl;
		setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
		while(!edgeTop_){
			pointerKeyPress(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second + delta);
			pointerKeyPress(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000); //animation has to be displayed before callback is made :-/
		}
		
		std::cout << "...checking for edge,bottom callback" << std::endl;
		while(!edgeBottom_){
			pointerKeyPress(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second - delta);
			pointerKeyPress(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}
		
		elm_list_horizontal_set(list_, EINA_TRUE);
		
		std::cout << "...checking for edge,left callback" << std::endl;
		while(!edgeLeft_){
			pointerKeyPress(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first + delta, centerOfWindow_.second);
			pointerKeyPress(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}

		std::cout << "...checking for edge,right callback" << std::endl;
		while(!edgeRight_){
			pointerKeyPress(BTN_LEFT, 1);
			setGlobalPointerPosition(centerOfWindow_.first - delta, centerOfWindow_.second);
			pointerKeyPress(BTN_LEFT, 0);
			setGlobalPointerPosition(centerOfWindow_.first, centerOfWindow_.second);
			Application::yield(500000);
		}
		
		elm_list_horizontal_set(list_, EINA_FALSE);
	}
	
	static void itemSelectCallback(void* data, Evas_Object* li, void*)
	{
   		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		
   		Elm_Object_Item *lit = elm_list_selected_item_get(li);
   		std::cout << "...received itemSelectCallback for item: " << elm_object_item_text_get(lit) << std::endl;
 		
 		test->itemSelected_ = true;
	}
	
	static void onUnselected(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
		test->unselected_ = true;
		std::cout << "...received unselected event" << std::endl;
	}
	
	static void onEdgeTop(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->edgeTop_ = true;
		std::cout << "...received edge,top event" << std::endl;
	}
	
	static void onEdgeBottom(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->edgeBottom_ = true;
		std::cout << "...received edge,bottom event" << std::endl;
	}

	static void onEdgeLeft(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->edgeLeft_ = true;
		std::cout << "...received edge,left event" << std::endl;
	}

	static void onEdgeRight(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->edgeRight_ = true;
		std::cout << "...received edge,right event" << std::endl;
	}

	static void onDoubleClick(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->doubleClicked_ = true;
		std::cout << "...received clicked,double event" << std::endl;
	}

	static void onActivated(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->activated_ = true;
		std::cout << "...received activated event" << std::endl;
	}

	static void onLongpressed(void* data, Evas_Object*, void*)
	{
		UserListInteractionTest *test = static_cast<UserListInteractionTest*>(data);
 		test->longPressed_= true;
		std::cout << "...received longpressed event" << std::endl;
	}
	
private:
	Window		window_;
	EvasObject	list_;
	
	bool		itemSelected_;
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

typedef ResizeObjectTest<List> ListResizeTest;
typedef PositionObjectTest<List> ListPositionTest;
typedef VisibleObjectTest<List> ListVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(UserListInteractionTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListResizeTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListPositionTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListVisibilityTest, "List")

