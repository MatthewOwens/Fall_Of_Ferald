#include "catch.hpp"
#include "StateManager.h"
#include "InputManager.h"
#include "ImageManager.h"

SCENARIO("game state defaults to menu", "[StateManager]")
{
	GIVEN("a state manager is created")
	{
		InputManager in;
		ImageManager im;

		StateManager sm(&in,&im,NULL);
		REQUIRE(sm.getCurrentState() == sm.StateEnum::MENU);
	}
}
