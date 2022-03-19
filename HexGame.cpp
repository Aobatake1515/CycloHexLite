#include "HexGame.h"

/// <summary>
/// Do move with specified piece at the specified coordinates, return true if valid
/// </summary>
/// <param name="celltype"></param>
/// <param name="target"></param>
/// <returns></returns>

bool HexGame::DoMove(int targetX, int targetY, TileType typetype)
{
    assert(typetype != TileType::empty);

    if (winner != TileType::empty) return false;

    if (board.PlaceCell(targetX, targetY, typetype))
    {
        // check for win, if win, set all winning tiles to win state
        if (CheckWin())
        {
            // TODO: any events on win
        }
        turnNum++;
        return true;
    }
    else return false;
}

// like normal do move but uses the current turn player and changes turn after

void HexGame::DoMove(int x, int y)
{
    assert(turn != TileType::empty);
    DoMove(x, y, turn);
    SwapTurn();
}

/// <summary>
/// check whether the game is won
/// </summary>
/// <returns></returns>

bool HexGame::CheckWin()
{
    for (TileType type : { TileType::red, TileType::blue })
    {
        winpath = BoardAnalysis::WinningPath(board, type);
        if (winpath.size() != 0)
        {
            winner = type;
            return true;
        }
    }
    return false;
}

bool HexGame::IsOnWinPath(int x, int y) const
{
    if (winner == TileType::empty) return false;
    int tileIndex = board.GetIndex(x, y);
    for (int i : winpath)
    {
        if (i == tileIndex) return true;
    }
    return false;
}

/// <summary>
/// resets the game to its default state
/// </summary>
/// <param name="firstMove">who moves first on reset</param>

void HexGame::Reset(int sideLength, TileType firstMove)
{
    turn = firstMove;
    board.Resize(sideLength);
    turnNum = 0;
    winner = TileType::empty;
    winpath.clear();

}

void HexGame::Reset(TileType firstMove)
{
    Reset(0, firstMove);
}
