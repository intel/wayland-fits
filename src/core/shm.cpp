#include <sys/mman.h>
#include "shm.h"

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
	wl_shm_pool *pool;
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

	ASSERT(pool != NULL);

	wl_buffer_ = wl_shm_pool_create_buffer(
		pool, 0, width_, height_, stride_, WL_SHM_FORMAT_ARGB8888);

	ASSERT(wl_buffer_ != NULL);

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

TEST(SharedMemory, "Core/Wrapper")
{
	Display display;
	SharedMemory shm(display);

	FAIL_UNLESS_EQUAL(&shm.display(), &display);
	FAIL_IF((wl_shm*)shm == NULL);
	FAIL_UNLESS_EQUAL(wl_shm_get_user_data(shm), &shm);
}

TEST(SharedMemoryBuffer, "Core/Wrapper")
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

