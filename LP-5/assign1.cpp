#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// class Graph{
//     int vertices;
//     vector<vector<int>> adj;
//     Graph(int vertices){
//         this->vertices=vertices;
//         adj.resize(vertices);
//     }
//     void addedge(int src,int dest){
//         adj[src].push_back(dest);
//         adj[dest].push_back(src);
//     }

//     void viewgraph(){
//         for(int i=0;i<vertices;i++){
//             cout<<"Verex"<<i<<"->";
//             for(int j=0;j<adj[i].size();j++){
//                 cout<<adj[i][j]<<" ";
//             }
//             cout<<endl;
//         }
//     }

//     void parallel_bfs(int start){
//         vector<bool>visited(vertices,false);
//         queue<int>q;
//         q.push(start);
//         #pragma omp parallel
//     }



// }
// Graph class representing an undirected graph using adjacency list representation
class Graph {
private:
    int numVertices;          // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

public:
    Graph(int vertices) : numVertices(vertices), adj(vertices) {}

    // Add an edge between two vertices
    void addEdge(int src, int dest) {
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    // View the graph
    void viewGraph() {
        cout << "Graph:\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << " -> ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }


    // Parallel BFS with critical section
    void bfs_parallel(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();
            cout << currentVertex << " ";

            // Parallelize neighbor processing with critical section for shared data
            #pragma omp parallel for
            for (int i = 0; i < adj[currentVertex].size(); i++) {
                int neighbor = adj[currentVertex][i];
                bool doPush = false;
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        doPush = true;
                    }
                }
                if (doPush) {
                    #pragma omp critical
                    q.push(neighbor);
                }
            }
        }
    }

    // Sequential BFS for comparison
    void bfs_sequential(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();
            cout << currentVertex << " ";
            for (int neighbor : adj[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    // Parallel DFS with critical section
    void dfs_parallel(int startVertex) {
        vector<bool> visited(numVertices, false);
        stack<int> s;

        visited[startVertex] = true;
        s.push(startVertex);

        while (!s.empty()) {
            int currentVertex = s.top();
            s.pop();
            cout << currentVertex << " ";

            // Parallelize neighbor processing with critical section for shared data
            #pragma omp parallel for
            for (int i = 0; i < adj[currentVertex].size(); i++) {
                int neighbor = adj[currentVertex][i];
                bool doPush = false;
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        doPush = true;
                    }
                }
                if (doPush) {
                    #pragma omp critical
                    s.push(neighbor);
                }
            }
        }
    }

    // Sequential DFS for comparison
    void dfs_sequential(int startVertex) {
        vector<bool> visited(numVertices, false);
        stack<int> s;

        visited[startVertex] = true;
        s.push(startVertex);

        while (!s.empty()) {
            int currentVertex = s.top();
            s.pop();
            cout << currentVertex << " ";
            for (int neighbor : adj[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    s.push(neighbor);
                }
            }
        }
    }
};

int main() {
    int numVertices;
    cout << "Enter the number of vertices in the graph: ";
    cin >> numVertices;

    // Create a graph with the specified number of vertices
    Graph graph(numVertices);

    int numEdges;
    cout << "Enter the number of edges in the graph: ";
    cin >> numEdges;

    cout << "Enter the edges (source destination):\n";
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    // View the graph
    graph.viewGraph();

    int startVertex;
    cout << "Enter the starting vertex for BFS and DFS: ";
    cin >> startVertex;

    // Compare BFS sequential and parallel
    double start, end;

    cout << "\nSequential BFS: ";
    start = omp_get_wtime();
    graph.bfs_sequential(startVertex);
    end = omp_get_wtime();
    cout << "\nTime (sequential): " << (end - start) << " seconds\n";

    cout << "Parallel BFS (with critical): ";
    start = omp_get_wtime();
    graph.bfs_parallel(startVertex);
    end = omp_get_wtime();
    cout << "\nTime (parallel): " << (end - start) << " seconds\n";

    cout << "\nSequential DFS: ";
    start = omp_get_wtime();
    graph.dfs_sequential(startVertex);
    end = omp_get_wtime();
    cout << "\nTime (sequential): " << (end - start) << " seconds\n";

    cout << "Parallel DFS (with critical): ";
    start = omp_get_wtime();
    graph.dfs_parallel(startVertex);
    end = omp_get_wtime();
    cout << "\nTime (parallel): " << (end - start) << " seconds\n";

    cout << endl;

    return 0;
}