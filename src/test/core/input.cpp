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

#include "input.h"

Input::Input(const Query& query)
	: query_(query)
	, wfits_input_(query.display().bind<wfits_input>(
		"wfits_input", &wfits_input_interface))
{
	ASSERT(wfits_input_ != NULL);

	wfits_input_set_user_data(*this, this);
}

/*virtual*/ Input::~Input()
{
	wfits_input_destroy(*this);
}

void Input::moveGlobalPointer(int32_t x, int32_t y) const
{
	wfits_input_move_pointer(*this, x, y);
	query().expectGlobalPointerPosition(x, y);
}

TEST(InputConstruct, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Input input(query);

	FAIL_UNLESS_EQUAL(&input.query(), &query);
	FAIL_IF((wfits_input*)input == NULL);
	FAIL_UNLESS_EQUAL(wfits_input_get_user_data(input), &input);
}

TEST(InputMoveGlobalPointer, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Input input(query);

	for (unsigned y(0); y < 480; y+= 80) {
		for (unsigned x(0); x < 640; x+=80) {
			input.moveGlobalPointer(x, y);
		}
	}
}
