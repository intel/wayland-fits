#ifndef __WFITS_CORE_SHM_H__
#define __WFITS_CORE_SHM_H__

#include "display.h"

class SharedMemory
{
public:
	SharedMemory(const Display&);

	virtual ~SharedMemory();

	operator wl_shm*() const { return wl_shm_; }
	const Display& display() const { return display_; }

private:
	static void format(void*, wl_shm*, uint32_t);

	const Display&	display_;
	wl_shm		*wl_shm_;
	bool		argb_;
};

class SharedMemoryBuffer {
public:
	SharedMemoryBuffer(const SharedMemory&, int width, int height);
	virtual ~SharedMemoryBuffer();

	int size() const { return size_; }
	int width() const { return width_; }
	int height() const { return height_; }
	int stride() const { return stride_; }

	operator wl_buffer*() const { return wl_buffer_; }
	const SharedMemory& shm() const { return shm_; }

private:
	const SharedMemory&	shm_;
	wl_buffer		*wl_buffer_;
	void			*data_;
	int			width_;
	int			height_;
	int			stride_;
	int			size_;

};

#endif
