#pragma once


#include "Board.h"
#include "HexGame.h"


namespace ConsoleManager
{
	enum class CommandType
	{
		Place,
		Shift,
		Quit,
		Restart
	};


	void ClearConsole();
	void PrintBoard(const HexGame& hexGame);
	void PrintTurnHeader(const HexGame& hexGame);
	void PrintFullTurn(const HexGame& hexGame);

	CommandType GetTurnCommandBlocking(const HexGame& hexGame, Vei2& out_coords);

	void GetGameStartData(int& out_boardSize, TileType& out_tileType);
};

