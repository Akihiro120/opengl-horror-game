#pragma once
#include "../item.h"

namespace ItemCollection
{
	static Item ITEM_none()
	{
		return {
			"None",
			"None"
		};
	}

	static Item ITEM_test()
	{
		return {
			"Test",
			"This is for testing"
		};
	}

	static Item ITEM_cube()
	{
		return {
			"Cube",
			"This box is a cube"
		};
	}

	static Item ITEM_doorKey()
	{
		return {
			"Door Key",
			"This key opens a door"
		};
	}
}