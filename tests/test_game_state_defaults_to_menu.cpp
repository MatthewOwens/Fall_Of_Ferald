#include "catch.hpp"
#include "StateManager.h"

SCENARIO("game state defaults to menu", "[StateManager]")
{
	GIVEN("a state manager without access to the window and input/image managers")
	{
		StateManager sm(NULL, NULL, NULL);
		REQUIRE(sm.getCurrentState() == StateManager::StateEnum::MENU);
	}
}
