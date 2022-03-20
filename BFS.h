#pragma once
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<functional>

// T_Node must have working <, ==, and != operators
template<typename T_Node>
struct BFS
{
    struct Graph
    {
        std::map<T_Node, std::set<T_Node>> edges;
    };
    
    /// <summary>
    /// Performs BFS to make a tree with the shortest path from the start node
    /// Sets the parent of the starting node as itself
    /// !!!Cannot have self loops!!! (may want to change later)
    /// </summary>
    /// <param name="start">starting node</param>
    /// <param name="graph">graph to search</param>
    /// <returns>map with keys: reachable nodes and vals: parent in tree</returns>
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

    /// <summary>
    /// Gets a set of all nodes reachable from the starting node
    /// </summary>
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

    /// <summary>
    /// Shortest path to the target node if it exists
    /// *A bit slower than necessary since parentMap goes to all nodes rather than stopping at a target
    /// </summary>
    /// <param name="parentMap">tree of shortest paths</param>
    /// <param name="target">target node</param>
    /// <returns>shortest path, empty if no path</returns>
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

    // number of nodes reachable
    static int AccessableSize(std::map<T_Node, T_Node> parentMap)
    {
        return parentMap.size();
    }

    /// <summary>
    /// Like GetParentMap, but uses a function to represent the graph rather than a graph object
    /// Terminates at an ending node since the graph could be infinite
    /// </summary>
    /// <param name="start">starting node</param>
    /// <param name="end">ending node, must be reachable if the graph is infinite</param>
    /// <param name="graphFunc">function for the graph, takes a node and returns a set of connected nodes</param>
    /// <returns>map with keys: reachable nodes and vals: parent in tree</returns>
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


