#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;


class Graph {
private:
    int numVertices;
    vector<vector<int>> adjMatrix;

public:
    Graph(int vertices) : numVertices(vertices) {
        adjMatrix.resize(vertices, vector<int>(vertices, 0));
    }

    void addEdge(int src, int dest, int weight = 1) {
        adjMatrix[src][dest] = weight;
        adjMatrix[dest][src] = weight;  
    }

    int getNumVertices() const {
        return numVertices;
    }

    const vector<vector<int>>& getAdjMatrix() const {
        return adjMatrix;
    }
};


void BFS(const Graph& graph, int startVertex) {
    int numVertices = graph.getNumVertices();
    vector<bool> visited(numVertices, false);
    queue<int> q;

    visited[startVertex] = true;
    q.push(startVertex);

    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();
        cout << currentVertex << " ";

        const vector<int>& neighbors = graph.getAdjMatrix()[currentVertex];
        for (int i = 0; i < numVertices; ++i) {
            if (neighbors[i] > 0 &&!visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    cout << endl;
}


void DFSUtil(const Graph& graph, int vertex, vector<bool>& visited) {
    visited[vertex] = true;
    cout << vertex << " ";

    const vector<int>& neighbors = graph.getAdjMatrix()[vertex];
    for (int i = 0; i < graph.getNumVertices(); ++i) {
        if (neighbors[i] > 0 &&!visited[i]) {
            DFSUtil(graph, i, visited);
        }
    }
}


void DFS(const Graph& graph, int startVertex) {
    vector<bool> visited(graph.getNumVertices(), false);
    DFSUtil(graph, startVertex, visited);
    cout << endl;
}

vector<int> dijkstra(const Graph& graph, int startVertex) {
    int numVertices = graph.getNumVertices();
    vector<int> dist(numVertices, INT_MAX);
    vector<bool> visited(numVertices, false);

    dist[startVertex] = 0;

    for (int count = 0; count < numVertices - 1; ++count) {
        int minDist = INT_MAX;
        int minIndex = -1;

        for (int v = 0; v < numVertices; ++v) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }

        visited[minIndex] = true;

        const vector<int>& neighbors = graph.getAdjMatrix()[minIndex];
        for (int i = 0; i < numVertices; ++i) {
            if (neighbors[i] > 0 &&!visited[i] && dist[minIndex]!= INT_MAX &&
                dist[minIndex] + neighbors[i] < dist[i]) {
                dist[i] = dist[minIndex] + neighbors[i];
            }
        }
    }

    return dist;
}

int prim(const Graph& graph) {
    int numVertices = graph.getNumVertices();
    vector<bool> inMST(numVertices, false);
    vector<int> key(numVertices, INT_MAX);
    vector<int> parent(numVertices, -1);

    key[0] = 0;

    for (int count = 0; count < numVertices - 1; ++count) {
        int minKey = INT_MAX;
        int minIndex = -1;

        for (int v = 0; v < numVertices; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                minIndex = v;
            }
        }

        inMST[minIndex] = true;

        const vector<int>& neighbors = graph.getAdjMatrix()[minIndex];
        for (int i = 0; i < numVertices; ++i) {
            if (neighbors[i] > 0 &&!inMST[i] && neighbors[i] < key[i]) {
                key[i] = neighbors[i];
                parent[i] = minIndex;
            }
        }
    }

    int sum = 0;
    for (int i = 1; i < numVertices; ++i) {
        sum += graph.getAdjMatrix()[parent[i]][i];
    }
    return sum;
}

void testBFS() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    cout << "BFS starting from vertex 0: ";
    BFS(g, 0);
}

void testDFS() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    cout << "DFS starting from vertex 0: ";
    DFS(g, 0);
}

void testDijkstra() {
    Graph g(5);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 1, 1);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 10);
    g.addEdge(3, 4, 2);

    vector<int> distances = dijkstra(g, 0);
    cout << "Shortest distances from vertex 0: ";
    for (int i = 0; i < distances.size(); ++i) {
        cout << distances[i] << " ";
    }
    cout << endl;
}

void testPrim() {
    Graph g(5);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 3, 6);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 8);
    g.addEdge(1, 4, 5);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 4, 9);

    int minCost = prim(g);
    cout << "Cost of minimum spanning tree: " << minCost << endl;
}

int main() {
    testBFS();
    testDFS();
    testDijkstra();
    testPrim();
    return 0;
}