# graph-algorithms-comparison
This repository contains implementations and performance tests of selected graph algorithms, including both shortest-path and spanning-tree algorithms, as well as max-flow computation. Each algorithm is tested using two equivalent input formats: adjacency matrix and adjacency list. This allows fair comparisons in terms of both algorithm design and data structure performance.

Implemented Algorithms
Dijkstra - shortest path (non-negative weights)
Bellman-Ford - shortest path (supports negative weights)
Ford-Fulkerson - maximum flow in a flow network
Prim - minimum spanning tree (MST)
Kruskal - MST using edge sorting and union-find

Features
Execution time comparison between algorithms
Two interchangeable graph representations:
  Adjacency matrix
  Adjacency list
