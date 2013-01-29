#ifndef __WFITS_CORE_INPUT_H__
#define __WFITS_CORE_INPUT_H__

#include "query.h"

class Input
{
public:
	Input(const Query&);
	virtual ~Input();

	operator wfits_input*() const { return wfits_input_; }
	const Query& query() const { return query_; }

	void moveGlobalPointer(int32_t, int32_t) const;

private:
	const Query& query_;
	wfits_input* wfits_input_;
};

#endif
