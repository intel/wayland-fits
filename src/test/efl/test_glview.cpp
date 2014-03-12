/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <vector>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class GLView : public EvasObject
{
public:
	GLView(EvasObject &parent)
		: EvasObject::EvasObject(elm_glview_add(parent))
	{
		return;
	}
};

class GLViewResizePolicyTest : public ElmTestHarness
{
	typedef std::vector<Elm_GLView_Resize_Policy> Policies;
public:
	GLViewResizePolicyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewResizePolicyTest", "GLView Resize Policy Test")
		, control_(window_)
		, set_(EINA_FALSE)
		, policies_({
			ELM_GLVIEW_RESIZE_POLICY_RECREATE
			, ELM_GLVIEW_RESIZE_POLICY_SCALE
			, ELM_GLVIEW_RESIZE_POLICY_RECREATE
			, ELM_GLVIEW_RESIZE_POLICY_SCALE
		  })
		, nchecks_(0)
	{
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_GLView_Resize_Policy policy, policies_) {
			synchronized(boost::bind(&GLViewResizePolicyTest::set, boost::ref(*this), policy));
			synchronized(boost::bind(&GLViewResizePolicyTest::check, boost::ref(*this)));
		}
	}

	void set(const Elm_GLView_Resize_Policy policy)
	{
		set_ = elm_glview_resize_policy_set(control_, policy);
	}

	void check(void)
	{
		++nchecks_;
		FAIL_UNLESS_EQUAL(set_, EINA_TRUE);
	}

	void teardown()
	{
		ASSERT(policies_.size() > 0);
		ASSERT(nchecks_ == policies_.size());
	}

private:
	Window		window_;
	GLView		control_;
	Eina_Bool	set_;
	Policies	policies_;
	unsigned	nchecks_;
};

class GLViewRenderPolicyTest : public ElmTestHarness
{
	typedef std::vector<Elm_GLView_Render_Policy> Policies;
public:
	GLViewRenderPolicyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewRenderPolicyTest", "GLView Render Policy Test")
		, control_(window_)
		, set_(EINA_FALSE)
		, policies_({
			ELM_GLVIEW_RENDER_POLICY_ON_DEMAND
			, ELM_GLVIEW_RENDER_POLICY_ALWAYS
			, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND
			, ELM_GLVIEW_RENDER_POLICY_ALWAYS
		  })
	{
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_GLView_Render_Policy policy, policies_) {
			synchronized(boost::bind(&GLViewRenderPolicyTest::set, boost::ref(*this), policy));
			synchronized(boost::bind(&GLViewRenderPolicyTest::check, boost::ref(*this)));
		}
	}

	void set(const Elm_GLView_Render_Policy policy)
	{
		set_ = elm_glview_render_policy_set(control_, policy);
	}

	void check(void)
	{
		FAIL_UNLESS_EQUAL(set_, EINA_TRUE);
	}

private:
	Window		window_;
	GLView		control_;
	Eina_Bool	set_;
	Policies	policies_;
};

class GLViewModeTest : public ElmTestHarness
{
public:
	GLViewModeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewModeTest", "GLView Mode Test")
		, control_(window_)
		, set_(EINA_FALSE)
	{
		modes_.push_back(ELM_GLVIEW_ALPHA);
		modes_.push_back(ELM_GLVIEW_DEPTH);
		modes_.push_back(ELM_GLVIEW_ALPHA);
		modes_.push_back(ELM_GLVIEW_STENCIL);
		modes_.push_back(ELM_GLVIEW_ALPHA);
		modes_.push_back(ELM_GLVIEW_DIRECT);
		modes_.push_back(ELM_GLVIEW_ALPHA);
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_GLView_Mode mode, modes_) {
			synchronized(boost::bind(&GLViewModeTest::set, boost::ref(*this), mode));
			synchronized(boost::bind(&GLViewModeTest::check, boost::ref(*this)));
		}
	}

	void set(const Elm_GLView_Mode mode)
	{
		set_ = elm_glview_mode_set(control_, mode);
	}

	void check(void)
	{
		FAIL_UNLESS_EQUAL(set_, EINA_TRUE);
	}

private:
	Window				window_;
	GLView				control_;
	Eina_Bool			set_;
	std::vector<Elm_GLView_Mode>	modes_;
};

class GLViewAPITest : public ElmTestHarness
{
public:
	GLViewAPITest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewAPITest", "GLView API Test")
		, control_(window_)
		, api_(NULL)
	{
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		synchronized(boost::bind(&GLViewAPITest::check, boost::ref(*this)));
	}

	void check(void)
	{
		api_ = elm_glview_gl_api_get(control_);

		FAIL_UNLESS(api_ != NULL);

		// check a few GL|ES 2 functions implemented in i915
		FAIL_UNLESS(api_->glActiveTexture != NULL);
		FAIL_UNLESS(api_->glAttachShader != NULL);
		FAIL_UNLESS(api_->glBlendColor != NULL);
		FAIL_UNLESS(api_->glBindFramebuffer != NULL);
		FAIL_UNLESS(api_->glBindRenderbuffer != NULL);
		FAIL_UNLESS(api_->glBindBuffer != NULL);
		FAIL_UNLESS(api_->glCompileShader != NULL);
		FAIL_UNLESS(api_->glLinkProgram != NULL);
	}

private:
	Window		window_;
	GLView		control_;
	Evas_GL_API*	api_;
};


typedef ResizeObjectTest<GLView> GLViewResizeTest;
typedef PositionObjectTest<GLView> GLViewPositionTest;
typedef VisibleObjectTest<GLView> GLViewVisibilityTest;

WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewResizeTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewPositionTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewVisibilityTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewResizePolicyTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewRenderPolicyTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewModeTest)
WFITS_EFL_HARNESS_EGL_TEST_CASE(GLViewAPITest)

} // namespace efl
} // namespace test
} // namespace wfits
