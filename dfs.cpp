#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int node)
{
    stack<int> s;
    s.push(node);
    while (!s.empty())
    {
        int curr_node = s.top();
        s.pop();
        if (!visited[curr_node])
        {
            visited[curr_node] = true;
            cout << curr_node << " "; // Print the visited node
#pragma omp parallel for
            for (int i = 0; i < graph[curr_node].size(); i++)
            {
                int adj_node = graph[curr_node][i];
                if (!visited[adj_node])
                {
                    s.push(adj_node);
                }
            }
        }
    }
}

int main()
{
    int n = 6;          // Number of nodes
    int m = 5;          // Number of edges
    int start_node = 2; // Starting node
    cout << "Parallel Depth First Search (DFS) starting from vertex " << start_node << " : ";

    graph[1].push_back(2);
    graph[2].push_back(1);
    graph[1].push_back(3);
    graph[3].push_back(1);
    graph[2].push_back(4);
    graph[4].push_back(2);
    graph[2].push_back(5);
    graph[5].push_back(2);
    graph[3].push_back(6);
    graph[6].push_back(3);

#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    dfs(start_node);

    return 0;
}
