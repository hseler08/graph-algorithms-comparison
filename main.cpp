#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "timer.h"
#include "algorithms.h"
#include "modes.h"

void showHelp() {
    std::cout << "PROGRAM GRAFOWY - AiZO Projekt 2\n";
    std::cout << "=================================\n";
    std::cout << "Tryby u¿ycia:\n\n";

    std::cout << "  --file <problem> <algorithm> <plik_wejsciowy> [plik_wyjsciowy] <typ wyjœcia> [start] [end]\n";
    std::cout << "    problem:  0 - MST (drzewo rozpinaj¹ce)\n";
    std::cout << "              1 - najkrótsza œcie¿ka\n";
    std::cout << "              2 - maksymalny przep³yw\n";
    std::cout << "    algorithm: dla MST:\n";
    std::cout << "                 0 - Prim\n";
    std::cout << "                 1 - Kruskal\n";
    std::cout << "               dla najkrótszej œcie¿ki:\n";
    std::cout << "                 2 - Dijkstra\n";
    std::cout << "                 3 - Bellman-Ford\n";
    std::cout << "               dla maksymalnego przep³ywu:\n";
    std::cout << "                 4 - Ford-Fulkerson\n";
    std::cout << "    plik_wejsciowy: nazwa pliku z grafem (format zgodny z PDF)\n";
    std::cout << "    plik_wyjsciowy: (opcjonalnie) plik z wynikami\n";
	std::cout << "    typ wyjœcia: (opcjonalnie) 'matrix' lub 'list' (domyœlnie 'list')\n";
    std::cout << "    start, end:  wierzcho³ki pocz¹tkowy i koñcowy dla œcie¿ki/przep³ywu(dla problemu 1 i 2)\n";

    std::cout << "  --test <problem> <algorithm> <liczba_wierzcholkow> <gestosc[%]> <plik_wyjsciowy>\n";
    std::cout << "    problem:  0 - MST(drzewo rozpinaj¹ce)\n";
    std::cout << "              1 - najkrótsza œcie¿ka\n";
    std::cout << "              2 - maksymalny przep³yw\n";
    std::cout << "    algorithm: dla MST:\n";
    std::cout << "                 0 - Prim\n";
    std::cout << "                 1 - Kruskal\n";
    std::cout << "               dla najkrótszej œcie¿ki:\n";
    std::cout << "                 2 - Dijkstra\n";
    std::cout << "                 3 - Bellman-Ford\n";
    std::cout << "               dla maksymalnego przep³ywu:\n";
    std::cout << "                 4 - Ford-Fulkerson\n";
	std::cout << "    liczba_wierzcholkow: liczba wierzcho³ków w grafie\n";
	std::cout << "    gestosc[%]: procent krawêdzi w grafie (0-100)\n";
    std::cout << "    plik_wyjsciowy: (opcjonalnie) plik z wynikami\n";

    std::cout << "  --help\n";
    std::cout << "    Wyœwietla tê pomoc.\n";

}

GraphAlgorithmType parseAlgorithm(int alg) {
    switch (alg) {
    case 0: return GraphAlgorithmType::PRIM;
    case 1: return GraphAlgorithmType::KRUSKAL;
    case 2: return GraphAlgorithmType::DIJKSTRA;
    case 3: return GraphAlgorithmType::FORD_BELLMAN;
    case 4: return GraphAlgorithmType::FORD_FULKERSON;
    default: throw std::invalid_argument("Unknown algorithm id");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp();
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "--help") {
            showHelp();
        }
        else if (mode == "--file"){
            int problem = std::stoi(argv[2]);
            GraphAlgorithmType alg = parseAlgorithm(std::stoi(argv[3]));
            std::string inputFile = argv[4];
            std::string outputFile = (argc >= 6) ? argv[5] : "";
            GraphRepresentation repr = (argc == 7 && std::string(argv[6]) == "matrix")
                ? GraphRepresentation::MATRIX
                : GraphRepresentation::LIST;
            
            bool directed = (problem != 0);
            Graph g = loadGraphFromFile(inputFile, directed, repr);

            int start = 0, end = 0;
            if (problem == 1 || problem == 2) {
                if (argc < 9) {
                    std::cerr << "Podaj start i end dla tego problemu!\n";
                    return 1;
                }
                start = std::stoi(argv[7]);
                end = std::stoi(argv[8]);
                if (start < 0 || start >= g.vertices || end < 0 || end >= g.vertices) {
                    std::cerr << "B³¹d: start lub end poza zakresem wierzcho³ków grafu!\n";
                    return 1;
                }
            }
            if (!outputFile.empty()) {
                std::ofstream out(outputFile);

                if (problem == 0) { // MST
                    if (alg == GraphAlgorithmType::PRIM) {
                        std::vector<Edge> mst = GraphAlgorithms::primMST(g);
                        int totalWeight = 0;
                        for (const Edge& edge : mst) totalWeight += edge.weight;
                        out << "MST (Prim):\n";
                        out << "Total weight: " << totalWeight << "\n";
                        for (const Edge& edge : mst) {
                            out << edge.from << " " << edge.to << " " << edge.weight << "\n";
                        }
                    }
                    else if (alg == GraphAlgorithmType::KRUSKAL) {
                        std::vector<Edge> mst = GraphAlgorithms::kruskalMST(g);
                        int totalWeight = 0;
                        for (const Edge& edge : mst) totalWeight += edge.weight;
                        out << "MST (Kruskal):\n";
                        out << "Total weight: " << totalWeight << "\n";
                        for (const Edge& edge : mst) {
                            out << edge.from << " " << edge.to << " " << edge.weight << "\n";
                        }
                    }
                    /*if (alg == GraphAlgorithmType::KRUSKAL) { //dla 50 razy do testu datasetu
                        const int runs = 50;
                        double totalTime = 0.0;
                        std::vector<Edge> lastMST;

                        for (int i = 0; i < runs; ++i) {
                            Timer timer;
                            timer.start();

                            // Jeœli runAlgorithm wykonuje Kruskala i zwraca MST, 
                            // to u¿yj tego, lub jeœli musisz, wywo³aj bezpoœrednio:
                            auto mst = GraphAlgorithms::kruskalMST(g);

                            timer.stop();
                            totalTime += timer.result();

                            if (i == runs - 1) { // na ostatnim przebiegu zachowaj wynik
                                lastMST = mst;
                            }
                        }

                        int totalWeight = 0;
                        for (const auto& edge : lastMST) totalWeight += edge.weight;

                        out << "MST (Kruskal) – œredni czas z " << runs << " uruchomieñ: "
                            << (totalTime / runs) << " ms\n";
                        out << "Total weight: " << totalWeight << "\n";
                        for (const auto& edge : lastMST) {
                            out << edge.from << " " << edge.to << " " << edge.weight << "\n";
                        }
                        }
                        */
                    
                }
                else if (problem == 1) { // Najkrótsza œcie¿ka
                    std::pair<std::vector<int>, std::vector<int>> result;
                    if (alg == GraphAlgorithmType::DIJKSTRA)
                        result = GraphAlgorithms::dijkstra(g, start);
                    else if (alg == GraphAlgorithmType::FORD_BELLMAN)
                        result = GraphAlgorithms::fordBellman(g, start);

                    out << "Najkrotsza sciezka od " << start << " do " << end << ": ";
                    if (result.first[end] == std::numeric_limits<int>::max()) {
                        out << "brak œcie¿ki\n";
                    }
                    else {
                        out << "koszt = " << result.first[end] << " | sciezka: ";
                        // Funkcja wypisuj¹ca œcie¿kê
                        std::vector<int> path;
                        for (int v = end; v != -1; v = result.second[v])
                            path.push_back(v);
                        std::reverse(path.begin(), path.end());
                        for (size_t i = 0; i < path.size(); ++i) {
                            out << path[i];
                            if (i + 1 < path.size()) out << " -> ";
                        }
                        out << "\n";
                    }
                }
                else if (problem == 2) { // Maksymalny przep³yw
                    int maxFlow = GraphAlgorithms::fordFulkerson(g, start, end);
                    out << "Maksymalny przeplyw: " << maxFlow << "\n";
                }
            }
        }
        else if (mode == "--test" && argc == 7) {
            int problem = std::stoi(argv[2]);
            int algorithm = std::stoi(argv[3]);
            int vertices = std::stoi(argv[4]);
            double density = std::stod(argv[5]) / 100.0;
            std::string outputFile = argv[6];
            const int count = 50;
            int start = 0;
            int end = vertices - 1;
            std::ofstream out(outputFile);
            if (!out) throw std::runtime_error("Cannot open output file");

            // wybór algorytmów
            std::vector<GraphAlgorithmType> algs;
            if (algorithm == 5) {
                algs = {
                    GraphAlgorithmType::PRIM,
                    GraphAlgorithmType::KRUSKAL,
                    GraphAlgorithmType::DIJKSTRA,
                    GraphAlgorithmType::FORD_BELLMAN,
                    GraphAlgorithmType::FORD_FULKERSON
                };
            }
            else {
                algs = { parseAlgorithm(algorithm) };
            }
            
            std::vector<std::vector<std::vector<int>>> allTimes_repr(2);
            for (int r = 0; r < 2; ++r)
                allTimes_repr[r].resize(algs.size());

            for (int i = 0; i < count; ++i) {
                for (int repr_id = 0; repr_id < 2; ++repr_id) {
                    GraphRepresentation repr = (repr_id == 0) ? GraphRepresentation::LIST : GraphRepresentation::MATRIX;
                    Graph g = generateRandomGraph(vertices, density, true, repr);
                    std::cout << "Test: " << i << " | " << (repr == GraphRepresentation::LIST ? "LIST" : "MATRIX") << " representation\n";

                    for (size_t j = 0; j < algs.size(); ++j) {
                        Graph g_copy = g;

                        Timer timer;
                        timer.start();
                        runAlgorithm(g_copy, algs[j]);
                        timer.stop();

                        allTimes_repr[repr_id][j].push_back(timer.result());
                    }
                }
            }

            // ZAPIS WYNIKÓW
            out << "LIST:\n";
            for (int i = 0; i < count; ++i) {
                for (size_t j = 0; j < allTimes_repr[0].size(); ++j) {
                    out << allTimes_repr[0][j][i];
                    if (j != allTimes_repr[0].size() - 1) out << "\t";
                }
                out << "\n";
            }

            out << "\nMATRIX:\n";
            for (int i = 0; i < count; ++i) {
                for (size_t j = 0; j < allTimes_repr[1].size(); ++j) {
                    out << allTimes_repr[1][j][i];
                    if (j != allTimes_repr[1].size() - 1) out << "\t";
                }
                out << "\n";
            }

            out.close();
        }
        else {
            std::cerr << "Invalid arguments\n";
            showHelp();
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}