#ifndef __WFITS_COMMON_UTIL_H__
#define __WFITS_COMMON_UTIL_H__

#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/foreach.hpp>

boost::lambda::placeholder1_type _1_;
boost::lambda::placeholder2_type _2_;
boost::lambda::placeholder3_type _3_;

#define foreach BOOST_FOREACH
#define foreach_reverse BOOST_REVERSE_FOREACH

struct Geometry
{
	Geometry()
		: x(-1)
		, y(-1)
		, width(-1)
		, height(-1)
	{
		return;
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

	int32_t x;
	int32_t y;
};

#endif
