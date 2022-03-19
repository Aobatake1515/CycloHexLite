#pragma once

#include<limits>
#include<sstream>
#include<cmath>

#include "BFS.h"
#include "Board.h"




namespace BoardAnalysis
{
    /// <summary>
    /// Node for hex tiles
    /// </summary>
    struct Node
    {
    public:
        int index;
        int shiftx, shifty;
        int sidelen;

    public:
        Node() = default; // compiler wants this, not sure where its being used
        Node(int index, Board board)
            : index(index)
            , shiftx(board.GetShiftX())
            , shifty(board.GetShiftY())
            , sidelen(board.GetSidelen())
        {}

        // for "layers" where looping in one direction puts you in a different state
        Node(int x, int y, Board board, int layer = 0)
        {
            shiftx = board.GetShiftX();
            shifty = board.GetShiftY();
            sidelen = board.GetSidelen();
            index = GetIndex(x, y);
            index += layer * sidelen * sidelen;
        }

        static Node GoalNode()
        {
            return Node(std::numeric_limits<int>::max(), Board(1));
        }

        /// <summary>
        /// gets the index not counting the layer
        /// </summary>
        /// <returns></returns>
        int GetBoardIndex() const
        {
            return Utils::BindRange(index, 0, sidelen * sidelen);
        }

        int GetLayer() const
        {
            if (index >= 0)
            {
                return index / (sidelen * sidelen);
            }
            else
            {
                return (index / (sidelen * sidelen)) - 1;
            }
        }

        int GetIndex(int x, int y) const
        {
            int newx = Utils::BindRange(x - shiftx, 0, sidelen);
            int newy = Utils::BindRange(y - shifty, 0, sidelen);
            return newx + (newy * sidelen);
        }

        Vei2 GetCoords() const
        {
            int boundIndex = Utils::BindRange(index, 0, sidelen * sidelen);
            int x = Utils::BindRange((boundIndex % sidelen) + shiftx, 0, sidelen);
            int y = Utils::BindRange((boundIndex / sidelen) + shifty, 0, sidelen);
            return { x, y };
        }

        bool operator < (const Node& other) const
        {
            return index < other.index;
        }
        bool operator == (const Node& other) const
        {
            return index == other.index;
        }
        bool operator != (const Node& other) const
        {
            return index != other.index;
        }

        std::string ToString() const
        {
            if (index == std::numeric_limits<int>::max())
            {
                return "Goal Tile";
            }
            else
            {
                auto coords = GetCoords();
                // why cant I have string.format :(
                auto stream = std::stringstream();
                stream << "Tile(" << coords.x << "," << coords.y << ") Layer " << GetLayer();

                return stream.str();
            }
        }
    };

    std::set<Node> WinGraphFunc(const Board& board, Node node, Node start);

    std::set<Node> BoardGraphFunc(const Board& board, Node node, std::function<bool(Node)> endFunc);

    std::vector<int> WinningPath(const Board& board, TileType type);
};

