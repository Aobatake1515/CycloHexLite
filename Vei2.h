#pragma once

/// <summary>
/// Simple wrapper for 2 ints
/// </summary>
struct Vei2
{
	int x;
	int y;

	Vei2(int x, int y)
		: x(x)
		, y(y)
	{}

	bool operator==(const Vei2& other) const
	{
		return x == other.x && y == other.y;
	}
};