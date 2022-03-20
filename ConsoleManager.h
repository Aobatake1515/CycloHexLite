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


	void PrintBoard(const HexGame& hexGame);
	// print winner or turn count and current player
	void PrintTurnHeader(const HexGame& hexGame);
	// print header, and print board. 
	void PrintFullTurn(const HexGame& hexGame);

	/// <summary>
	/// Gets the turn input from the player. Handles error checking
	/// </summary>
	/// <param name="hexGame">hex game to be checked against</param>
	/// <param name="out_coords">outut reference for coordinates of shifts and placements</param>
	/// <returns>what command was given (no return for "help" command)</returns>
	CommandType GetTurnCommandBlocking(const HexGame& hexGame, Vei2& out_coords);

	/// <summary>
	/// Gets the input needed to start the game. Handles error checking
	/// </summary>
	/// <param name="out_boardSize">output reference for board size</param>
	/// <param name="out_tileType">output reference for the starting player</param>
	void GetGameStartData(int& out_boardSize, TileType& out_startingTurn);
};

