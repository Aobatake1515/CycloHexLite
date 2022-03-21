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
        Node(int x, int y, const Board& board, int layer = 0)
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

        /// <summary>
        /// Gets the layer of the node in terms of looping past the grid bounds
        /// </summary>
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

    /// <summary>
    /// graph function for checking whether a win has occured from the starting node
    /// </summary>
    /// <param name="board">board used</param>
    /// <param name="node">current node that connections are being checked for</param>
    /// <param name="start">starting node, used to know if a given node is an ending node</param>
    /// <returns>nodes that are reachable from node</returns>
    std::set<Node> WinGraphFunc(const Board& board, Node node, Node start);

    /// <summary>
    /// graph function for a board
    /// </summary>
    /// <param name="board">board used</param>
    /// <param name="node">current node that connections are being checked for</param>
    /// <param name="endFunc">function to know whether a given node is a valid end node</param>
    /// <returns>nodes that are reachable from node</returns>
    std::set<Node> BoardGraphFunc(const Board& board, Node node, std::function<bool(Node)> endFunc);

    /// <summary>
    /// finds the winning path of a given tiletype on the board if it exists
    /// </summary>
    /// <param name="board">board used</param>
    /// <param name="type">tiletype checked</param>
    /// <returns>one of the shortest winning paths if possible, otherwise empty vector</returns>
    std::vector<int> WinningPath(const Board& board, TileType type);

};

