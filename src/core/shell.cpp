#include "shell.h"

Shell::Shell(const Display& display)
	: display_(display)
	, wl_shell_(
		display.bind<wl_shell>(
			"wl_shell", &wl_shell_interface))
{
	ASSERT(wl_shell_ != NULL);

	wl_shell_set_user_data(*this, this);

	ASSERT(wl_shell_get_user_data(*this) == this);
}

/*virtual*/ Shell::~Shell()
{
	wl_shell_destroy(*this);
}

TEST(Shell, "Core/Wrapper")
{
	Display display;
	Shell shell(display);

	FAIL_UNLESS_EQUAL(&shell.display(), &display);
	FAIL_IF((wl_shell*)shell == NULL);
	FAIL_UNLESS_EQUAL(wl_shell_get_user_data(shell), &shell);
}
