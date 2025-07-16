#ifndef MODES_H
#define MODES_H

#include <string>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "algorithms.h"
#include "timer.h"

Graph loadGraphFromFile(const std::string& filename, bool directed, GraphRepresentation repr) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Cannot open input file");
    int edges, vertices;
    in >> edges >> vertices;
    Graph g(vertices, directed, repr);
    for (int i = 0; i < edges; ++i) {
        int u, v, w;
        in >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    return g;
}

Graph generateRandomGraph(int vertices, double density, bool directed, GraphRepresentation repr) {
    int maxEdges = directed ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
    int edgeCount = static_cast<int>(maxEdges * density);

    Graph g(vertices, directed, repr);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> weightDist(1, 100);

    std::vector<std::pair<int, int>> possibleEdges;
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && (directed || i < j)) {
                possibleEdges.emplace_back(i, j);
            }
        }
    }
    std::shuffle(possibleEdges.begin(), possibleEdges.end(), rng);
    for (int i = 0; i < edgeCount && i < static_cast<int>(possibleEdges.size()); ++i) {
        int u = possibleEdges[i].first;
        int v = possibleEdges[i].second;
        int w = weightDist(rng);
        g.addEdge(u, v, w);
    }
    return g;
}

std::string formatGraphOutput(const Graph& g, const std::string& format = "") {
    std::ostringstream out;
    out << "Wierzcholki: " << g.vertices << "\n";
    out << "Krawedzie: " << g.edges.size() << "\n";
    for (const Edge& edge : g.edges) {
        out << edge.from << " -> " << edge.to << " [" << edge.weight << "]\n";
    }

    if ((format == "matrix" || (format.empty() && g.representation == GraphRepresentation::MATRIX)) && !g.incidenceMatrix.empty()) {
        out << "\nMacierz:\n";
        for (const std::vector<int>& row : g.incidenceMatrix) {
            for (int val : row)
                out << val << " ";
            out << "\n";
        }
    }
    else if (format == "list" || (format.empty() && g.representation == GraphRepresentation::LIST)) {
        out << "\n Lista:\n";
        for (int i = 0; i < g.vertices; ++i) {
            out << i << ": ";
            for (const std::pair<int, int>& p : g.adjList[i]) {
                out << "(" << p.first << ", " << p.second << ") ";
            }
            out << "\n";
        }
    }
    return out.str();
}

void runAlgorithm(const Graph& g, GraphAlgorithmType alg, const std::string& outputFile = "") {
    std::ostringstream results;
    switch (alg) {
    case GraphAlgorithmType::PRIM:
        GraphAlgorithms::primMST(g);
    case GraphAlgorithmType::KRUSKAL:
        GraphAlgorithms::kruskalMST(g);
    case GraphAlgorithmType::DIJKSTRA:
        GraphAlgorithms::dijkstra(g, 0);
    case GraphAlgorithmType::FORD_BELLMAN:
        GraphAlgorithms::fordBellman(g, 0);
    case GraphAlgorithmType::FORD_FULKERSON:
        GraphAlgorithms::fordFulkerson(const_cast<Graph&>(g), 0, g.vertices - 1);
    }

    if (!outputFile.empty()) {
        std::ofstream out(outputFile);
        if (!out) throw std::runtime_error("Cannot write to output file");
        out << formatGraphOutput(g);
        out << "\nResult:\n";
        out << results.str() << "\n";
    }
}

#endif // MODES_H
