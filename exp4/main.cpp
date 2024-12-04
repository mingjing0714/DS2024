#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>
#include <algorithm>

using namespace std;

class Graph {
public:
    int V;  
    vector<vector<int>> adj;  

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);  
    }

    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int u = s.top();
            s.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    s.push(v);
                }
            }
        }
        cout << endl;
    }

    void dijkstra(int start) {
        vector<int> dist(V, INT_MAX);
        dist[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v : adj[u]) {
                if (dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Shortest paths from node " << start << ":" << endl;
        for (int i = 0; i < V; ++i) {
            cout << "Node " << i << ": " << (dist[i] == INT_MAX ? -1 : dist[i]) << endl;
        }
    }

    void primMST() {
        vector<int> parent(V, -1);
        vector<int> key(V, INT_MAX);
        vector<bool> inMST(V, false);

        key[0] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, 0});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u]) continue;

            inMST[u] = true;

            for (int v : adj[u]) {
                if (!inMST[v] && 1 < key[v]) {
                    key[v] = 1;
                    pq.push({key[v], v});
                    parent[v] = u;
                }
            }
        }

        cout << "Minimum Spanning Tree edges:" << endl;
        for (int i = 1; i < V; ++i) {
            cout << parent[i] << " - " << i << endl;
        }
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    cout << "BFS traversal starting from node 0:" << endl;
    g.BFS(0);

    cout << "DFS traversal starting from node 0:" << endl;
    g.DFS(0);

    g.dijkstra(0);  

    g.primMST();  

    return 0;
}
