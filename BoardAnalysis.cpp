#include "BoardAnalysis.h"


namespace BoardAnalysis
{
    std::set<Node> WinGraphFunc(const Board& board, Node node, Node start)
    {
        auto endFunc = [start = start](Node in_node)
        {
            int layer = in_node.GetLayer();
            return in_node.GetCoords() == start.GetCoords() && layer > 0;
        };
        return BoardGraphFunc(board, node, endFunc);
    }

    std::set<Node> BoardGraphFunc(const Board& board, Node node, std::function<bool(Node)> endFunc)
    {
        auto coords = node.GetCoords();
        int layer = node.GetLayer();
        auto color = board.GetCell(coords);
        auto retVal = std::set<Node>();
        if (color == TileType::empty) return retVal;

        // prevent it from going further than necessary on board
        if (std::abs(layer) > board.GetSidelen() / 2)
        {
            return retVal;
        }
        // this node is a valid end, so set that it connects to the "goal node" value
        if (endFunc(node))
        {
            retVal.insert(Node::GoalNode());
        }

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == dy) continue; // corners and center arent adjacent
                if (board.GetCell(coords.x + dx, coords.y + dy) != color) continue; // not player

                // check for whether the adjacent tile would loop forward or backward in respect to that color's goal direction
                bool isLoopBack = (color == TileType::blue && coords.y + dy < 0) ||
                    (color == TileType::red && coords.x + dx < 0);
                bool isLoopFrwd = (color == TileType::blue && coords.y + dy >= board.GetSidelen()) ||
                    (color == TileType::red && coords.x + dx >= board.GetSidelen());

                bool isLoop = isLoopBack || isLoopFrwd;

                // increment or decrement based on loop direction if looping
                if (isLoopBack)
                {
                    retVal.emplace(coords.x + dx, coords.y + dy, board, layer - 1);
                }
                else if (isLoopFrwd)
                {
                    retVal.emplace(coords.x + dx, coords.y + dy, board, layer + 1);
                }
                else
                {
                    retVal.emplace(coords.x + dx, coords.y + dy, board, layer);
                }
            }
        }
        return retVal;
    }

    std::vector<int> WinningPath(const Board& board, TileType type)
    {
        assert(type != TileType::empty);
        if (type == TileType::empty) return std::vector<int>();

        auto allWinpaths = std::vector<std::vector<int>>();
        for (int i = 0; i < board.GetSidelen(); i++)
        {
            auto startNode = type == TileType::blue ? Node(i, 0, board, 0) : Node(0, i, board, 0);
            auto targetNode = type == TileType::blue ? Node(i, 0, board, 1) : Node(0, i, board, 1);

            if (board.GetCell(startNode.GetCoords()) != type) continue;
            auto endFunc = [&](Node node) {return WinGraphFunc(board, node, startNode); };
            auto parentMap = BFS<Node>::LazyGetParentMap(startNode, Node::GoalNode(), endFunc);
            auto path = BFS<Node>::ShortestPath(parentMap, Node::GoalNode());

            if (path.size() == 0) continue;

            auto thisWinpath = std::vector<int>();
            // ignore last 2 since it's the end node (same coords as start) and the goal indicator node
            for (int j = 0; j < (int)path.size() - 2; j++)
            {
                thisWinpath.emplace_back(path[j].GetBoardIndex());
            }

            // path is shorter
            if (allWinpaths.size() == 0 || allWinpaths[0].size() > thisWinpath.size())
            {
                allWinpaths.clear();
                allWinpaths.emplace_back(thisWinpath);
            }
            // equal size to other paths
            else if (allWinpaths[0].size() == thisWinpath.size())
            {
                allWinpaths.emplace_back(thisWinpath);
            }
        }
        return (allWinpaths.size() == 0) ? std::vector<int>() : allWinpaths[0];
    }
}