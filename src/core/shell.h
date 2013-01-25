#ifndef __WFITS_CORE_SHELL_H__
#define __WFITS_CORE_SHELL_H__

#include "display.h"

class Shell
{
public:
	Shell(const Display&);

	virtual ~Shell();

	operator wl_shell*() const { return wl_shell_; }
	const Display& display() const { return display_; }

private:
	const Display&	display_;
	wl_shell	*wl_shell_;
};

#endif
