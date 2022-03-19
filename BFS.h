#pragma once
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<stack>
#include<functional>

// T_Node must have working <, ==, and != operators
template<typename T_Node>
struct BFS
{
    struct Graph
    {
        std::map<T_Node, std::set<T_Node>> edges;
    };

    static std::map<T_Node, T_Node> GetParentMap(T_Node start, Graph graph)
    {
        auto parentMap = std::map<T_Node, T_Node>();

        if (graph.edges.count(start) == 0) return parentMap;

        auto queue = std::queue<T_Node>();
        queue.push(start);
        // base case: key == value
        // assume no one node cycles
        // TODO: maybe make into pointer to be nullable later?
        parentMap.insert({ start, start });

        while (queue.size() > 0)
        {
            T_Node current = queue.back();
            queue.pop();

            if (graph.edges.count(current) == 0) continue;
            for (T_Node n : graph.edges[current])
            {
                if (parentMap.count(n) == 0)
                {
                    parentMap.insert({ n, current });
                    queue.push(n);
                }
            }
        }

        return parentMap;
    }

    static std::set<T_Node> GetConnectedNodes(T_Node start, Graph graph)
    {
        auto retVal = std::set<T_Node>();

        auto parentMap = GetParentMap(start, graph);

        // add all keys
        for(auto node : parentMap)
        {
            retVal.insert(node.first);
        }
        retVal.insert(start);

        return retVal;
    }

    static std::vector<std::set<T_Node>> GetNodeDepths(std::map<T_Node, T_Node> parentMap)
    {
        std::vector<std::set<T_Node>> nodeDepths = std::vector<std::set<T_Node>>();

        for(auto pair : parentMap)
        {
            auto child = pair.first;
            auto nodeStack = std::stack<T_Node>();
            T_Node tempNode = child;
            // root node has value = key
            while (tempNode != parentMap[tempNode])
            {
                nodeStack.push(tempNode);
                tempNode = parentMap[tempNode];
            }
            for (int i = 0; nodeStack.size() != 0; i++)
            {
                if (nodeDepths.size() <= i)
                {
                    nodeDepths.emplace_back();
                }
                nodeDepths[i].insert(nodeStack.top());
                nodeStack.pop();
            }
        }
        return nodeDepths;
    }

    static std::vector<T_Node> ShortestPath(std::map<T_Node, T_Node> parentMap, T_Node target)
    {
        auto retVal = std::vector<T_Node>();
        if (parentMap.count(target) != 0)
        {
            T_Node next = target;
            while (next != parentMap[next])
            {
                retVal.emplace_back(next);
                next = parentMap[next];
            }
            retVal.emplace_back(next);
        }
        std::reverse(retVal.begin(), retVal.end());
        return retVal;
    }

    static int AccessableSize(std::map<T_Node, T_Node> parentMap)
    {
        return parentMap.size();
    }

    static std::map<T_Node, T_Node> LazyGetParentMap(T_Node start, T_Node end, std::function<std::set<T_Node>(T_Node)> graphFunc)
    {
        auto parentMap = std::map<T_Node, T_Node>();

        auto queue = std::queue<T_Node>();
        queue.push(start);
        parentMap.insert({ start, start });

        while (queue.size() > 0)
        {
            T_Node current = queue.front();
            queue.pop();
            if (current == end) break;

            auto canReachSet = graphFunc(current);
            for(T_Node n : canReachSet)
            {
                if (parentMap.count(n) == 0)
                {
                    parentMap.insert({ n, current });
                    queue.push(n);
                }
            }
        }

        return parentMap;
    }
};


