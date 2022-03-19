#include "ConsoleManager.h"

#include<iostream>
#include<sstream>
#include<iomanip>

using namespace std;


/// <summary>
/// Helper functions
/// </summary>


static string GetConsoleLineBlocking()
{
	cout << "> ";
	string input;
	getline(cin, input);
	return input;
}

static string ParseLowerStringFromStream(stringstream& stream)
{
	string retVal = "";
	if (stream >> retVal)
	{
		transform(retVal.begin(), retVal.end(), retVal.begin(), tolower);
	}
	return retVal;
}

static int ParseCharNumberFromStream(stringstream& stream)
{
	int retVal = -1;
	string temp = "";
	if (stream >> temp)
	{
		for (auto ch : temp)
		{
			if (ch >= 'a' && ch <= 'z')
			{
				retVal = ch - 'a';
				break;
			}
			if (ch >= 'A' && ch <= 'Z')
			{
				retVal = ch - 'A';
				break;
			}
		}
	}
	return retVal;
}

static int ParseNumberFromStream(stringstream& stream)
{
	int retVal = -1;
	int tempNum;
	if (stream >> tempNum)
	{
		retVal = tempNum;
	}
	return retVal;
}


// ANSI escape code colors
static string MakeBlue(string text)
{
	return "\x1B[36m" + text + "\033[0m";
}

static string MakeDarkBlue(string text)
{
	return "\x1B[34m" + text + "\033[0m";
}

static string MakeRed(string text)
{
	return "\x1B[91m" + text + "\033[0m";
}

static string MakeDarkRed(string text)
{
	return "\x1B[31m" + text + "\033[0m";
}
// end of helper section


void ConsoleManager::ClearConsole()
{
	system("cls");
}

void ConsoleManager::PrintBoard(const HexGame& hexGame)
{
	auto board = hexGame.GetBoardRef();
	Vei2 shift(board.GetShiftX(), board.GetShiftY());
	int sidelen = board.GetSidelen();

	auto shiftLetterLabelFunc = [](int index, int shift, int sidelen) 
	{
		return char('A' + Utils::BindRange(index - shift, 0, sidelen));
	};
	auto shiftNumberLabelFunc = [](int index, int shift, int sidelen) 
	{
		// +1 since indexing starts at 1
		return Utils::BindRange(index - shift, 0, sidelen) + 1;
	};

	for (int i = 0; i < 2; i++)
		cout << " ";
	for (int i = 0; i < sidelen; i++)
		cout << " " << MakeRed({ shiftLetterLabelFunc(i, shift.x, sidelen) }) << " ";
	cout << endl;
	for (int iy = 0; iy < sidelen; iy++)
	{

		for (int i = 0; i < iy; i++)
			cout << " ";
		string rowLabelString = to_string(shiftNumberLabelFunc(iy, shift.y, sidelen));
		if (rowLabelString.length() < 2) cout << " ";
		cout << MakeBlue(rowLabelString) << " ";
		for (int ix = 0; ix < sidelen; ix++)
		{
			bool onWinPath = hexGame.IsOnWinPath(ix, iy);

			cout << " ";
			switch (board.GetCell(ix, iy))
			{
			case TileType::empty:
				cout << "-";
				break;
			case TileType::red:
				if (onWinPath)
					cout << MakeDarkRed("R");
				else
					cout << MakeRed("R");
				break;
			case TileType::blue:
				if (onWinPath)
					cout << MakeDarkBlue("B");
				else
					cout << MakeBlue("B");
				break;
			default:
				break;
			}
			cout << " ";
		}
		cout << " " << MakeBlue(rowLabelString) << " ";
		cout << endl;
	}
	for (int i = 0; i < sidelen + 3; i++)
		cout << " ";
	for (int i = 0; i < sidelen; i++)
		cout << " " << MakeRed({ shiftLetterLabelFunc(i, shift.x, sidelen) }) << " ";
	cout << endl;
}

void ConsoleManager::PrintTurnHeader(const HexGame& hexGame)
{
	string turnString;
	switch (hexGame.GetTurn())
	{
	case TileType::blue:
		turnString = MakeBlue("Blue");
		break;
	case TileType::red:
		turnString = MakeRed("Red");
		break;
	default:
		turnString = "Error!";
		break;
	}

	switch (hexGame.GetWinner())
	{
	case TileType::blue:
		cout << MakeBlue("Blue Wins!") << endl;
		break;
	case TileType::red:
		cout << MakeRed("Red Wins!") << endl;
		break;
	case TileType::empty:
		cout << "It is currently turn " << hexGame.GetTurnNum() << endl;
		cout << "It is " << turnString << "'s turn to move" << endl;
		break;
	}
}

void ConsoleManager::PrintFullTurn(const HexGame& hexGame)
{
	ClearConsole();
	PrintTurnHeader(hexGame);
	PrintBoard(hexGame);
}

ConsoleManager::CommandType ConsoleManager::GetTurnCommandBlocking(const HexGame& hexGame, Vei2& out_coords)
{
	auto board = hexGame.GetBoardRef();
	out_coords = Vei2(-1, -1);

	string commandString = "";

	while (true)
	{
		cout << "Input a command or type \"help\" for instructions" << endl;

		auto stream = stringstream(GetConsoleLineBlocking());
		commandString = ParseLowerStringFromStream(stream);

		// too lazy to make a string switch class or something
		if (commandString == "")
			continue;
		if (commandString[0] == 'p') // for place
		{
			int c_x = ParseCharNumberFromStream(stream);
			// subtract 1 since visible board indexing starts at 1
			int c_y = ParseNumberFromStream(stream) - 1;

			out_coords.x = Utils::BindRange(c_x + board.GetShiftX(), 0, board.GetSidelen());
			out_coords.y = Utils::BindRange(c_y + board.GetShiftY(), 0, board.GetSidelen());

			if (c_x < 0 || c_y < 0 ||
				c_x >= hexGame.GetSideLen() || c_y >= hexGame.GetSideLen() ||
				hexGame.GetBoardRef().GetCell(out_coords) != TileType::empty)
			{
				cout << "Invalid coordinates given, please try again \nenter the coordinates as: <column letter> <row number>" << endl;
				continue;
			}
			else return CommandType::Place;
		}
		if (commandString[0] == 's') // for shift
		{
			string direction = ParseLowerStringFromStream(stream);
			if (direction != "")
			{
				// check first to prevent overlap with right
				if (direction == "reset")
				{
					out_coords = Vei2(-hexGame.GetBoardRef().GetShiftX(), -hexGame.GetBoardRef().GetShiftY());
					return CommandType::Shift;
				}
				if (direction[0] == 'd') // down
				{
					out_coords = Vei2(0, 1);
					return CommandType::Shift;
				}
				if (direction[0] == 'u') // up
				{
					out_coords = Vei2(0, -1);
					return CommandType::Shift;
				}
				if (direction[0] == 'r') // right
				{
					out_coords = Vei2(1, 0);
					return CommandType::Shift;
				}
				if (direction[0] == 'l') // left
				{
					out_coords = Vei2(-1, 0);
					return CommandType::Shift;
				}
			}

			cout << "Invalid direction given, please try again \nshift options are \"U\":Up, \"D\":Down, \"L\":Left, \"R\":Right, and \"Reset\"" << endl;
			continue;
		}
		if (commandString[0] == 'r') // for restart
		{
			return CommandType::Restart;
		}
		if (commandString[0] == 'q') // for quit
		{
			return CommandType::Quit;
		}
	}
}

void ConsoleManager::GetGameStartData(int& out_boardSize, TileType& out_tileType)
{
	string commandString = "";

	while (true)
	{
		cout << "Input board size followed by the player with first turn" << endl;
		cout << "<board size (2 to 26)> <red | blue | random>" << endl;

		auto stream = stringstream(GetConsoleLineBlocking());

		out_boardSize = ParseNumberFromStream(stream);
		string starting = ParseLowerStringFromStream(stream);

		if (out_boardSize >= 2 && out_boardSize <= 26)
		{
			if (starting == "red")
			{
				out_tileType = TileType::red;
				return;
			}
			if (starting == "blue")
			{
				out_tileType = TileType::blue;
				return;
			}
			if (starting == "random")
			{
				out_tileType = (rand() % 2 == 0) ? TileType::red : TileType::blue;
				return;
			}
		}
		cout << "Invalid game starting rules \n\n";
	}
}

