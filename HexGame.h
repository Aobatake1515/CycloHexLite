#pragma once

#include"BoardAnalysis.h"
#include"Board.h"


class HexGame
{
private:
    Board board;
    TileType turn; // player whose turn it is
    TileType winner; // player who won if game has been won, otherwise empty
    std::vector<int> winpath; // indicies to go through for path to win game, empty if not yet won
    int turnNum;

public:
    const Board& GetBoardRef() const { return board; }
    Board GetBoardCopy() const { return Board(board); }
    int GetSideLen() const { return board.GetSidelen(); }
    TileType GetTurn() const { return turn; }
    TileType GetWinner() const { return winner; }
    int GetTurnNum() const { return turnNum; }


public:
    HexGame(int sideLen, TileType firstMove = TileType::red)
        : turn(firstMove)
        , board(sideLen)
        , turnNum(0)
    {}

    /// <summary>
    /// Do move with specified piece at the specified coordinates, return true if valid
    /// </summary>
    /// <param name="celltype"></param>
    /// <param name="target"></param>
    /// <returns></returns>
    bool DoMove(int targetX, int targetY, TileType typetype);

    // like normal do move but uses the current turn player and changes turn after
    void DoMove(int x, int y);

    void SwapTurn()
    {
        turn = (turn == TileType::blue) ? TileType::red : TileType::blue;
    }

    /// <summary>
    /// shift the board in the direction of shift
    /// </summary>
    /// <param name="shift"></param>
    void ShiftBoard(int shiftX, int shiftY)
    {
        board.Shift(shiftX, shiftY);
    }

    /// <summary>
    /// Shifts board to absolute coordinates given
    /// </summary>
    /// <param name="shiftX"></param>
    /// <param name="shiftY"></param>
    void ShiftBoardAbs(int shiftX, int shiftY) { ShiftBoard(shiftX - board.GetShiftX(), shiftY - board.GetShiftY()); }

    /// <summary>
    /// reset shift in the board
    /// </summary>
    void ResetShift() { ShiftBoardAbs(0, 0); }

    /// <summary>
    /// check whether the game is won
    /// </summary>
    /// <returns></returns>
    bool CheckWin();

    /// <summary>
    /// checks whether the given coordinates are on the winning path if one exists
    /// does not actually check for whether a win occured, must check before using
    /// uses percieved coordinates, not true coordinates
    /// </summary>
    bool IsOnWinPath(int x, int y) const;

    /// <summary>
    /// resets the game to its default state, resizable
    /// </summary>
    /// <param name="firstMove">who moves first on reset</param>
    void Reset(int sideLength, TileType firstMove = TileType::red);

    // overload for no resize
    void Reset(TileType firstMove = TileType::red);
};

