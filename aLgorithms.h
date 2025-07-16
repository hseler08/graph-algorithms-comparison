#ifndef SORTER_H
#define SORTER_H

#undef LOG_MORE

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <set>
#include <numeric>
#include <stack>

enum class GraphRepresentation { LIST, MATRIX };
enum class GraphAlgorithmType {
    PRIM,
    KRUSKAL,
    DIJKSTRA,
    FORD_BELLMAN,
    FORD_FULKERSON
};

struct Edge {
    int from, to, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};



struct Graph {
    int vertices;
    bool directed;
    GraphRepresentation representation;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<std::vector<int>> incidenceMatrix;
    std::vector<Edge> edges;

    Graph(int v, bool dir, GraphRepresentation repr = GraphRepresentation::LIST)
        : vertices(v), directed(dir), representation(repr), adjList(v) {
    }

    void addEdge(int u, int v, int w) {
        adjList[u].emplace_back(v, w);
        if (!directed) adjList[v].emplace_back(u, w);
        edges.push_back({ u, v, w });
        if (representation == GraphRepresentation::MATRIX) {
            if (incidenceMatrix.empty()) incidenceMatrix.resize(vertices);
            for (std::vector<int>& row : incidenceMatrix) row.push_back(0);
            int idx = static_cast<int>(edges.size()) - 1;
            incidenceMatrix[u][idx] = -1;
            incidenceMatrix[v][idx] = 1;
        }
    }
};

class GraphAlgorithms {
public:
    static std::vector<Edge> primMST(const Graph& g);
    static std::vector<Edge> kruskalMST(const Graph& g);
    static std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph& g, int start);
    static std::pair<std::vector<int>, std::vector<int>> fordBellman(const Graph& g, int start);
    static int fordFulkerson(Graph& g, int source, int sink);
};

// Zwraca wektor krawêdzi MST
std::vector<Edge> GraphAlgorithms::primMST(const Graph& g) {
    int n = g.vertices;
    std::vector<int> key(n, std::numeric_limits<int>::max());
    std::vector<bool> inMST(n, false);
    std::vector<int> parent(n, -1);
    key[0] = 0;
    std::vector<Edge> mstEdges;

    for (int count = 0; count < n; ++count) {
        int u = -1;
        for (int v = 0; v < n; ++v)
            if (!inMST[v] && (u == -1 || key[v] < key[u]))
                u = v;

        inMST[u] = true;

        if (parent[u] != -1) {
            mstEdges.push_back({ parent[u], u, key[u] });
        }

        for (const std::pair<int, int>& pair : g.adjList[u]) {
            int v = pair.first;
            int weight = pair.second;
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }
    return mstEdges;
}


// Zwraca wektor krawêdzi MST
std::vector<Edge> GraphAlgorithms::kruskalMST(const Graph& g) {
    std::vector<Edge> sortedEdges = g.edges;
    std::sort(sortedEdges.begin(), sortedEdges.end());
    std::vector<int> parent(g.vertices);
    std::iota(parent.begin(), parent.end(), 0);

    std::vector<Edge> mstEdges;

    for (const Edge& e : sortedEdges) {
        int rootU = e.from;
        while (rootU != parent[rootU]) rootU = parent[rootU];

        int rootV = e.to;
        while (rootV != parent[rootV]) rootV = parent[rootV];

        if (rootU != rootV) {
            parent[rootU] = rootV;
            mstEdges.push_back(e);
        }
    }
    return mstEdges;
}

// Zwraca wektor odleg³oœci od startu
std::pair<std::vector<int>, std::vector<int>> GraphAlgorithms::dijkstra(const Graph& g, int start) {
    std::vector<int> dist(g.vertices, std::numeric_limits<int>::max());
    std::vector<int> prev(g.vertices, -1);
    dist[start] = 0;
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.emplace(0, start);

    while (!pq.empty()) {
        P top = pq.top(); pq.pop();
        int d = top.first;
        int u = top.second;
        if (d > dist[u]) continue;
        for (const std::pair<int, int>& p : g.adjList[u]) {
            int v = p.first, w = p.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return { dist, prev };
}

// Zwraca wektor odleg³oœci od startu
std::pair<std::vector<int>, std::vector<int>> GraphAlgorithms::fordBellman(const Graph& g, int start) {
    std::vector<int> dist(g.vertices, std::numeric_limits<int>::max());
    std::vector<int> prev(g.vertices, -1);
    dist[start] = 0;

    for (int i = 1; i < g.vertices; ++i) {
        for (const Edge& e : g.edges) {
            int u = e.from, v = e.to, w = e.weight;
            if (dist[u] != std::numeric_limits<int>::max() && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }
    return { dist, prev };
}

// Funkcja pomocnicza BFS do algorytmu Forda-Fulkersona
bool bfs_ff(int s, int t, std::vector<int>& parent, const std::vector<std::vector<int>>& residual, int V) {
    std::fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    std::queue<std::pair<int, int>> q;
    q.push({ s, INT_MAX });

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next = 0; next < V; ++next) {
            if (parent[next] == -1 && residual[cur][next] > 0) {
                parent[next] = cur;
                int new_flow = std::min(flow, residual[cur][next]);
                if (next == t)
                    return true;
                q.push({ next, new_flow });
            }
        }
    }
    return false;
}

// Zwraca maksymalny przep³yw
int GraphAlgorithms::fordFulkerson(Graph& g, int source, int sink) {
    int V = g.vertices;
    std::vector<std::vector<int>> capacity(V, std::vector<int>(V, 0));
    for (const Edge& edge : g.edges) {
        capacity[edge.from][edge.to] += edge.weight;
    }

    std::vector<std::vector<int>> residual = capacity;
    std::vector<int> parent(V);
    int maxFlow = 0;

    while (bfs_ff(source, sink, parent, residual, V)) {
        int flow = INT_MAX;
        int cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            flow = std::min(flow, residual[prev][cur]);
            cur = prev;
        }

        cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            residual[prev][cur] -= flow;
            residual[cur][prev] += flow;
            cur = prev;
        }

        maxFlow += flow;
    }
    return maxFlow;
}
#endif