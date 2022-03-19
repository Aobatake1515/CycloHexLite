

#include "ConsoleManager.h"

int main()
{
	int boardsize;
	TileType starting;
	ConsoleManager::GetGameStartData(boardsize, starting);
	HexGame hexGame = HexGame(boardsize, starting);
	while (true)
	{
		ConsoleManager::PrintFullTurn(hexGame);

		Vei2 coords = Vei2(0, 0);
		switch (ConsoleManager::GetTurnCommandBlocking(hexGame, coords))
		{
		case ConsoleManager::CommandType::Place:
			hexGame.DoMove(coords.x, coords.y);
			break;
		case ConsoleManager::CommandType::Shift:
			hexGame.ShiftBoard(coords.x, coords.y);
			break;
		case ConsoleManager::CommandType::Restart:
			ConsoleManager::ClearConsole();
			ConsoleManager::GetGameStartData(boardsize, starting);
			hexGame.Reset(boardsize, starting);
			break;
		case ConsoleManager::CommandType::Quit:
			return 0;
		}
	}
}
