#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <omp.h>
#include<chrono>


using namespace std;
using namespace std::chrono;
// Define the graph structure (undirected graph)
class Graph
{
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V)
    {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    //Breadth First Search
      void BFS(int source)
    {
       vector<bool> visited(adj.size(), false);
        queue<int> q;
        visited[source] = true;
        q.push(source);
        while (!q.empty())
        {
            int u;
            {
                {
                    u = q.front();
                    q.pop();
                }
                if (!(adj[u].size() == 0))
                {
                    for (int i = 0; i <= adj[u].size() - 1; ++i)
                    {
                        if (!visited[adj[u][i]])
                        {
                            {
                                q.push(adj[u][i]);
                                visited[adj[u][i]] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    

    // Parallel Breadth First Search
    // Parallel Breadth First Search
    void parallelBFS(int source)
    {
       vector<bool> visited(adj.size(), false);
        queue<int> q;
        visited[source] = true;
        q.push(source);
        while (!q.empty())
        {
            int u;
#pragma omp parallel shared(q, visited)
            {
#pragma omp single
                {
                    u = q.front();
                    q.pop();
                }
                if (!(adj[u].size() == 0))
                {
#pragma omp for
                    for (int i = 0; i <= adj[u].size() - 1; ++i)
                    {
                        if (!visited[adj[u][i]])
                        {
#pragma omp critical
                            {
                                q.push(adj[u][i]);
                                visited[adj[u][i]] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    //Depth First Search

     void DFSUtil(int v, vector<bool> &visited)
    {
        visited[v] = true;

// Traverse all adjacent vertices
        for (int i = 0; i < adj[v].size(); ++i)
        {
            int u = adj[v][i];
            if (!visited[u])
            {
                DFSUtil(u, visited);
            }
        }
    }

    void DFS(int source)
    {
        vector<bool> visited(V, false);
        DFSUtil(source, visited);
    }

    // Parallel Depth First Search
    // Parallel Depth First Search
    void parallelDFSUtil(int v, vector<bool> &visited)
    {
        visited[v] = true;

// Traverse all adjacent vertices
#pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i)
        {
            int u = adj[v][i];
            if (!visited[u])
            {
                parallelDFSUtil(u, visited);
            }
        }
    }

    void parallelDFS(int source)
    {
        vector<bool> visited(V, false);
        parallelDFSUtil(source, visited);
    }

    void createRandomGraph(int numEdges) {
    set<pair<int, int>> edgeSet;  // To avoid duplicate edges
    int numVertices = getV();

    while (edgeSet.size() < numEdges) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;

        // Avoid self-loops and duplicate edges
        if (u != v && edgeSet.find({u, v}) == edgeSet.end() && edgeSet.find({v, u}) == edgeSet.end()) {
            edgeSet.insert({u, v});
            addEdge(u, v);
        }
    }
}

int getV() const {
        return V;
    }
};

int main()
{

int numNodes = 10000;
    int numEdges = 20000; 
    // Create a graph
    Graph g(numNodes);



    g.createRandomGraph(numEdges);

     auto start = high_resolution_clock::now();

    cout << "Breadth First Search (BFS) starting from vertex 0: ";
    g.BFS(0);
    auto stop = high_resolution_clock::now();

    cout << endl;

    auto bfs_duration = duration_cast<milliseconds>(stop - start);


     start = high_resolution_clock::now();

    cout << "Parallel Breadth First Search (BFS) starting from vertex 0: ";
    g.parallelBFS(0);
     stop = high_resolution_clock::now();

    cout << endl;
    auto pbfs_duration = duration_cast<milliseconds>(stop - start);


     start = high_resolution_clock::now();

    cout << "Depth First Search (DFS) starting from vertex 0: ";
    g.DFS(0);
     stop = high_resolution_clock::now();

    cout << endl;
    auto dfs_duration = duration_cast<milliseconds>(stop - start);


     start = high_resolution_clock::now();

    cout << "Parallel Depth First Search (DFS) starting from vertex 0: ";
    g.parallelDFS(0);
     stop = high_resolution_clock::now();

    cout << endl;
    auto pdfs_duration = duration_cast<milliseconds>(stop - start);

    cout << "Sequential BFS Time: " << bfs_duration.count() << " milliseconds" << endl;
    cout << "Parallel BFS Time: " << pbfs_duration.count() << " milliseconds" << endl;
    cout << "Sequential DFS Time: " << dfs_duration.count() << " milliseconds" << endl;
    cout << "Paralell DFS Time: " << pdfs_duration.count() << " milliseconds" << endl;




    return 0;
}