#include "input.h"

Input::Input(const Query& query)
	: query_(query)
	, wfits_input_(query.display().bind<wfits_input>(
		"wfits_input", &wfits_input_interface))
{
	ASSERT(wfits_input_ != NULL);

	wfits_input_set_user_data(*this, this);
}

/*virtual*/ Input::~Input()
{
	wfits_input_destroy(*this);
}

void Input::moveGlobalPointer(int32_t x, int32_t y) const
{
	wfits_input_move_pointer(*this, x, y);
	query().expectGlobalPointerPosition(x, y);
}

TEST(InputConstruct, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Input input(query);

	FAIL_UNLESS_EQUAL(&input.query(), &query);
	FAIL_IF((wfits_input*)input == NULL);
	FAIL_UNLESS_EQUAL(wfits_input_get_user_data(input), &input);
}

TEST(InputMoveGlobalPointer, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Input input(query);

	for (unsigned y(0); y < 480; y+= 80) {
		for (unsigned x(0); x < 640; x+=80) {
			input.moveGlobalPointer(x, y);
		}
	}
}
