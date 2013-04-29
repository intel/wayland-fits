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
