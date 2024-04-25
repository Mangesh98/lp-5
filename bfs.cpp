#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph
{
private:
    int numVertices;         // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int vertices) : numVertices(vertices), adj(numVertices) {}

    void addEdge(int src, int dest)
    {
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    void viewGraph()
    {
        cout << "Graph : " << endl;
        for (int i = 0; i < numVertices; i++)
        {
            cout << "Vertices :" << i << " ->";
            for (int neighbour : adj[i])
            {
                cout << neighbour << " ";
            }
            cout << endl;
        }
    }

    void parallelBFS(int startVertex)
    {
        vector<bool> visited(numVertices, false);
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty())
        {
            int level_size = q.size(); // Get the size of the current level for parallel processing

#pragma omp parallel
            {
                vector<bool> local_visited(numVertices, false);
#pragma omp for
                for (int i = 0; i < level_size; ++i)
                {
                    int current;
#pragma omp critical
                    {
                        current = q.front();
                        q.pop();
                    }
                    cout << current << " ";
                    for (int neighbour : adj[current])
                    {
                        if (!local_visited[neighbour])
                        {
                            local_visited[neighbour] = true;
#pragma omp critical
                            {
                                if (!visited[neighbour])
                                {
                                    visited[neighbour] = true;
                                    q.push(neighbour);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

int main()
{
    // Predefined graph with 6 vertices and 5 edges
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 5);

    // View the graph
    graph.viewGraph();

    // Start BFS from vertex 0
    int startVertex = 0;
    cout << "Parallel Breadth First Search (BFS) starting from vertex " << startVertex << ": ";
    graph.parallelBFS(startVertex);
    cout << endl;

    return 0;
}
