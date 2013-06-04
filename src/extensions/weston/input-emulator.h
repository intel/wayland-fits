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

#ifndef __INPUT_EMULATOR_H__
#define __INPUT_EMULATOR_H__

#include <map>
#include "common/util.h"
#include "weston-wfits.h"

namespace wfits {
namespace weston {

class InputEmulator
{
public:
	InputEmulator() { }
	virtual ~InputEmulator() { }

	/**
	* Move the pointer to the desired compositor x,y coordinate.
	**/
	virtual void movePointer(const int32_t x, const int32_t y) const = 0;

	/**
	* Send a key event (mouse button, keyboard, etc.).
	**/
	virtual void keySend(const uint32_t key, const uint32_t state) const = 0;
};

class InputEmulatorFactory
{
public:
	typedef boost::function<InputEmulator* (void)> Creator;
	typedef std::map<std::string, Creator> Creators;

	static bool registerEmulator(const std::string&, Creator);
	static InputEmulator* create(const std::string&);

private:
	static Creators creators_;
};

template <class T>
class Create
{
public:
	T* operator()()
	{
		return new T;
	}
};

} // namespace weston
} // namespace wfits

#endif
