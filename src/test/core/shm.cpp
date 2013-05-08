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

#include <sys/mman.h>
#include "shm.h"

namespace wfits {
namespace test {
namespace core {

SharedMemory::SharedMemory(const Display& display)
	: display_(display)
	, wl_shm_(display.bind<wl_shm>("wl_shm", &wl_shm_interface))
	, argb_(false)
{
	ASSERT(wl_shm_ != NULL);

	wl_shm_set_user_data(*this, this);

	static const wl_shm_listener listener = {format};

	wl_shm_add_listener(*this, &listener, this);

	display.roundtrip();

	ASSERT(argb_);
}

/*virtual*/ SharedMemory::~SharedMemory()
{
	wl_shm_destroy(*this);
}

/*static*/ void SharedMemory::format(void *data, wl_shm *wl_shm, uint32_t format)
{
	SharedMemory* shm = static_cast<SharedMemory*>(data);

	ASSERT(wl_shm == *shm);

	if (format == WL_SHM_FORMAT_ARGB8888) {
		shm->argb_ = true;
	}
}

SharedMemoryBuffer::SharedMemoryBuffer(const SharedMemory& shm, int width, int height)
	: shm_(shm)
	, wl_buffer_(NULL)
	, data_(NULL)
	, width_(width)
	, height_(height)
	, stride_(width_ * 4)
	, size_(stride_ * height_)
{
	wl_shm_pool *pool(NULL);
	char tmp[] = "/tmp/wfits_shm-XXXXXX";
	int fd = mkstemp(tmp);

	ASSERT(fd >= 0);

	if (ftruncate(fd, size_) < 0) {
		unlink(tmp);
		close(fd);
		ASSERT(0);
	}

	data_ = mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	unlink(tmp);

	if (data_ == MAP_FAILED) {
		close(fd);
		ASSERT(data_ != MAP_FAILED);
	}

	pool = wl_shm_create_pool(shm_, fd, size_);
	if (not pool) {
		close(fd);
		ASSERT(pool != NULL);
	}

	wl_shm_pool_set_user_data(pool, this);
	if (wl_shm_pool_get_user_data(pool) != this) {
		close(fd);
		ASSERT(wl_shm_pool_get_user_data(pool) == this);
	}

	wl_buffer_ = wl_shm_pool_create_buffer(
		pool, 0, width_, height_, stride_, WL_SHM_FORMAT_ARGB8888);
	if (not wl_buffer_) {
		close(fd);
		ASSERT(wl_buffer_ != NULL);
	}

	wl_shm_pool_destroy(pool);
	close(fd);

	memset(data_, 0xff, size_);

	wl_buffer_set_user_data(*this, this);
}

/*virtual*/ SharedMemoryBuffer::~SharedMemoryBuffer()
{
	munmap(data_, size_);
	wl_buffer_destroy(*this);
}

namespace wrapper {

	TEST(SharedMemory)
	{
		Display display;
		SharedMemory shm(display);

		FAIL_UNLESS_EQUAL(&shm.display(), &display);
		FAIL_IF((wl_shm*)shm == NULL);
		FAIL_UNLESS_EQUAL(wl_shm_get_user_data(shm), &shm);
	}

	TEST(SharedMemoryBuffer)
	{
		Display display;
		SharedMemory shm(display);
		SharedMemoryBuffer buffer(shm, 24, 13);

		FAIL_UNLESS_EQUAL(&buffer.shm(), &shm);
		FAIL_IF((wl_buffer*)buffer == NULL);
		FAIL_UNLESS_EQUAL(wl_buffer_get_user_data(buffer), &buffer);

		FAIL_UNLESS_EQUAL(buffer.width(), 24);
		FAIL_UNLESS_EQUAL(buffer.height(), 13);
		FAIL_UNLESS_EQUAL(buffer.stride(), 4*24);
		FAIL_UNLESS_EQUAL(buffer.size(), 4*24*13);
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
