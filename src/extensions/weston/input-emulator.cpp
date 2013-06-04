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

#include "input-emulator.h"

namespace wfits {
namespace weston {

/*static*/ InputEmulatorFactory::Creators InputEmulatorFactory::creators_;

/*static*/ bool InputEmulatorFactory::registerEmulator(const std::string& name, Creator creator)
{
	std::pair<Creators::iterator, bool> result;
	result = creators_.insert(std::make_pair(name, creator));
	return result.second;
}

/*static*/ InputEmulator* InputEmulatorFactory::create(const std::string& name)
{
	const Creators::const_iterator creator(creators_.find(name));
	if (creator == creators_.end())
	{
		weston_log("weston-wfits: warning, no input emulator named '%s' found\n", name.c_str());
		return NULL;
	}

	return creator->second();
}

} // namespace weston
} // namespace wfits
