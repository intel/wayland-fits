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

#ifndef __WFITS_COMMON_UTIL_H__
#define __WFITS_COMMON_UTIL_H__

#include <iostream>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/current_function.hpp>
#include <boost/thread.hpp>

boost::lambda::placeholder1_type _1_;
boost::lambda::placeholder2_type _2_;
boost::lambda::placeholder3_type _3_;

#define foreach BOOST_FOREACH
#define foreach_reverse BOOST_REVERSE_FOREACH

struct Geometry
{
	Geometry(
		const int32_t ix = -1,
		const int32_t iy = -1,
		const int32_t iw = -1,
		const int32_t ih = -1
	)
		: x(ix)
		, y(iy)
		, width(iw)
		, height(ih)
	{
		return;
	}

	friend std::ostream& operator<<(std::ostream& os, const Geometry& g)
	{
		os << g.x << "," << g.y << " " << g.width << "x" << g.height;
		return os;
	}

	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

struct Position
{
	Position()
		: x(-1)
		, y(-1)
	{
		return;
	}

	friend std::ostream& operator<<(std::ostream& os, const Position& p)
	{
		os << p.x << "," << p.y;
		return os;
	}

	int32_t x;
	int32_t y;
};

#endif
