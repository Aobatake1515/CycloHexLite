#pragma once

#include<string>
#include<cassert>
#include "Utils.h"
#include "Vei2.h"


enum class TileType
{
    empty,
    red,
    blue
};

class Board
{
private:
    TileType* boardArr; // linear array to store 2d of pieces, always square
    int sidelen; // side length of board
    // amount shifted positive x dir
    // original x = percieved x - shiftx
    // 0 <= shiftx < sidelen
    int shiftX;
    int shiftY;

public:
    Board(int sidelen)
        : sidelen(sidelen)
        , boardArr(nullptr)
        , shiftX(0)
        , shiftY(0)
    {
        boardArr = new TileType[sidelen * sidelen];
        for (int i = 0; i < sidelen * sidelen; i++)
        {
            boardArr[i] = TileType::empty;
        }
    }

    Board(const Board& board)
        : sidelen(board.sidelen)
        , shiftX(board.shiftX)
        , shiftY(board.shiftY)
    {
        boardArr = new TileType[sidelen * sidelen];
        for (int i = 0; i < sidelen * sidelen; i++)
        {
            boardArr[i] = board.boardArr[i];
        }
    }

    ~Board()
    {
        if (boardArr)
            delete[] boardArr;
    }

    int GetSidelen() const { return sidelen; }
    int GetShiftX() const { return shiftX; }
    int GetShiftY() const { return shiftY; }
    int GetNumTiles() const { return sidelen * sidelen; }

    /// <summary>
    /// resets board to empty, keeps size
    /// </summary>
    void Reset()
    {
        for (int i = 0; i < GetNumTiles(); i++)
        {
            boardArr[i] = TileType::empty;
        }
        shiftX = 0;
        shiftY = 0;
    }

    /// <summary>
    /// resets board to empty, changes size
    /// </summary>
    void Resize(int sidelen)
    {
        if (boardArr)
            delete[] boardArr;
        this->sidelen = sidelen;
        boardArr = new TileType[sidelen * sidelen];
        for (int i = 0; i < sidelen * sidelen; i++)
        {
            boardArr[i] = TileType::empty;
        }
        shiftX = 0;
        shiftY = 0;
    }

    /// <summary>
    /// translates percieved indicies to index into board
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    int GetIndex(int x, int y) const
    {
        int newx = Utils::BindRange(x - shiftX, 0, sidelen);
        int newy = Utils::BindRange(y - shiftY, 0, sidelen);
        return newx + (newy * sidelen);
    }

    int GetIndex(Vei2 coords) const
    {
        return GetIndex(coords.x, coords.y);
    }

    /// <summary>
    /// translates index into periceved indicies as tuple
    /// </summary>
    /// <param name="index"></param>
    /// <returns></returns>
    Vei2 GetCoords(int index) const
    {
        int x = Utils::BindRange((index % sidelen) + shiftX, 0, sidelen);
        int y = Utils::BindRange((index / sidelen) + shiftY, 0, sidelen);
        return Vei2(x, y);
    }

    /// <summary>
    /// get value of cell in board given percieved indicies
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    TileType GetCell(int x, int y) const
    {
        return boardArr[GetIndex(x, y)];
    }

    TileType GetCell(Vei2 coords) const
    {
        return boardArr[GetIndex(coords)];
    }

    /// <summary>
    /// set value of cell in board given percieved indicies
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="val"></param>
    void SetCell(int x, int y, TileType val)
    {
        boardArr[GetIndex(x, y)] = val;
    }

    /// <summary>
    /// Board is empty
    /// </summary>
    /// <returns></returns>
    bool IsEmpty() const
    {
        for (int i = 0; i < GetNumTiles(); i++)
        {
            if (boardArr[i] != TileType::empty) return false;
        }
        return true;
    }

    /// <summary>
    /// shifts input board coordinates to fit this board's coords
    /// </summary>
    /// <param name="in_x"></param>
    /// <param name="in_y"></param>
    /// <param name="in_board"></param>
    /// <returns></returns>
    Vei2 RelativeShift(int x, int y, Board board) const
    {
        int index = board.GetIndex(x, y);
        return GetCoords(index);
    }

    Vei2 RelativeShift(int in_x, int in_y) const
    {
        return RelativeShift(in_x, in_y, Board(sidelen));
    }

    std::string GetLabel(int x, int y) const
    {
        int shiftedX = Utils::BindRange(x - shiftX, 0, sidelen);
        int shiftedY = Utils::BindRange(y - shiftY, 0, sidelen);

        return std::string(1, shiftedY + 'A') + std::to_string(shiftedX + 1);
    }

    std::string GetLabel(int index) const
    {
        auto coords = GetCoords(index);
        return GetLabel(coords.x, coords.y);
    }

    /// <summary>
    /// Attempts to place a piece in requested cell, returns whether successful
    /// </summary>
    /// <param name="x">periceved x value</param>
    /// <param name="y">percieve y value</param>
    /// <param name="type">piece type, not empty</param>
    /// <returns></returns>
    bool PlaceCell(int x, int y, TileType type)
    {
        if (x >= sidelen || y >= sidelen || type == TileType::empty)
        {
            assert(false && "Error in precondition for PlaceCell");
            return false;
        }
        // check not empty
        if (GetCell(x, y) != TileType::empty)
        {
            return false;
        }
        // set, update whether win occured
        SetCell(x, y, type);
        //IsWinUpdate();
        return true;

    }

    /// <summary>
    /// shifts the board by the amount specified
    /// </summary>
    /// <param name="x">amount to shift in x dir</param>
    /// <param name="y">amount to shift in y dir</param>
    void Shift(int x, int y)
    {
        shiftX = Utils::BindRange(shiftX + x, 0, sidelen);
        shiftY = Utils::BindRange(shiftY + y, 0, sidelen);
    }
};

