#include "display.h"
#include "compositor.h"
#include "pointer.h"
#include "seat.h"
#include "surface.h"
#include "shell.h"
#include "shell_surface.h"
#include "shm.h"
#include "input.h"
#include "query.h"

class Client
{
public:
	Client(uint32_t width, uint32_t height)
		: display()
		, compositor(display)
		, shell(display)
		, seat(display)
		, pointer(seat)
		, surface(compositor)
		, shellSurface(shell, surface)
		, shm(display)
		, buffer(shm, width, height)
		, query(display)
		, input(query)
	{
		wl_surface_attach(surface, buffer, 0, 0);
		wl_surface_damage(surface, 0, 0, buffer.width(),
				  buffer.height());
		surface.commit();
	}

	void movePointer(int32_t x, int32_t y)
	{
		Query::Geometry geometry = query.getSurfaceGeometry(surface);
		input.moveGlobalPointer(geometry.x + x, geometry.y + y);
	}

	void checkFocus(bool focus)
	{
		display.yield();
		for(unsigned i(0); i < 20 && (pointer.hasFocus(&surface) != focus); ++i) {
			display.yield(i*0.001*1e6);
		}
		FAIL_UNLESS_EQUAL(pointer.hasFocus(&surface), focus);
	}

	void checkPointer(uint32_t x, uint32_t y)
	{
		display.yield();
		for(unsigned i(0); i < 20 && (pointer.x() != x || pointer.y() != y); ++i) {
			display.yield(i*0.001*1e6);
		}
		FAIL_UNLESS_EQUAL(pointer.x(), x);
		FAIL_UNLESS_EQUAL(pointer.y(), y);
	}

	Display display;
	Compositor compositor;
	Shell shell;
	Seat seat;
	Pointer pointer;
	Surface surface;
	ShellSurface shellSurface;
	SharedMemory shm;
	SharedMemoryBuffer buffer;
	Query query;
	Input input;
};

TEST(SurfacePointer, "Core/Input")
{
	Client client(120, 75);

	for (unsigned x(0); x < 120; x+=10) {
		for (unsigned y(0); y < 75; y+=10) {
			client.movePointer(x, y);
			client.checkFocus(true);
			client.checkPointer(x, y);
		}
	}
	client.movePointer(-1, -1);
	client.checkFocus(false);
}
