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
public:
	GLViewResizePolicyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewResizePolicyTest", "GLView Resize Policy Test")
		, control_(window_)
		, set_(EINA_FALSE)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		policies_.push_back(ELM_GLVIEW_RESIZE_POLICY_RECREATE);
		policies_.push_back(ELM_GLVIEW_RESIZE_POLICY_SCALE);
		policies_.push_back(ELM_GLVIEW_RESIZE_POLICY_RECREATE);
		policies_.push_back(ELM_GLVIEW_RESIZE_POLICY_SCALE);
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (const Elm_GLView_Resize_Policy policy, policies_) {
			queueStep(boost::bind(&GLViewResizePolicyTest::set, boost::ref(*this), policy));
			queueStep(boost::bind(&GLViewResizePolicyTest::check, boost::ref(*this)));
		}
	}

	void set(const Elm_GLView_Resize_Policy policy)
	{
		set_ = elm_glview_resize_policy_set(control_, policy);
	}

	void check(void)
	{
		FAIL_UNLESS_EQUAL(set_, EINA_TRUE);
	}

private:
	Window					window_;
	GLView					control_;
	Eina_Bool				set_;
	std::vector<Elm_GLView_Resize_Policy>	policies_;
};

class GLViewRenderPolicyTest : public ElmTestHarness
{
public:
	GLViewRenderPolicyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("GLViewRenderPolicyTest", "GLView Render Policy Test")
		, control_(window_)
		, set_(EINA_FALSE)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		policies_.push_back(ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
		policies_.push_back(ELM_GLVIEW_RENDER_POLICY_ALWAYS);
		policies_.push_back(ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
		policies_.push_back(ELM_GLVIEW_RENDER_POLICY_ALWAYS);
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (const Elm_GLView_Render_Policy policy, policies_) {
			queueStep(boost::bind(&GLViewRenderPolicyTest::set, boost::ref(*this), policy));
			queueStep(boost::bind(&GLViewRenderPolicyTest::check, boost::ref(*this)));
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
	Window					window_;
	GLView					control_;
	Eina_Bool				set_;
	std::vector<Elm_GLView_Render_Policy>	policies_;
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
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

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
		window_.show();
		control_.show();

		foreach (const Elm_GLView_Mode mode, modes_) {
			queueStep(boost::bind(&GLViewModeTest::set, boost::ref(*this), mode));
			queueStep(boost::bind(&GLViewModeTest::check, boost::ref(*this)));
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
		control_.setSize(200, 100);
		control_.setPosition(50, 10);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueStep(boost::bind(&GLViewAPITest::check, boost::ref(*this)));
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

WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewResizeTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewPositionTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewVisibilityTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewResizePolicyTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewRenderPolicyTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewModeTest, "GLView")
WAYLAND_ELM_HARNESS_EGL_TEST_CASE(GLViewAPITest, "GLView")

