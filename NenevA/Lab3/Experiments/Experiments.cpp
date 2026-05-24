#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <memory>
#include "graph.h"
#include "adjacency_list.h"
int main() {
  std::cout << "Dijkstra Algorithm Performance Comparison\n";

  struct TestParams {
    int vertices; 
    double density;
  };

  std::vector<TestParams> tests = {
    {100, 0.5},
    {250, 0.75},
    {500, 0.35},
    {1000, 0.2}
  };

  std::vector<std::vector<double>> results;

  for (const auto& test : tests) {
    std::cout << "Testing graph: " << test.vertices << " vertices, " << test.density * 100 << "% density\n";

    Graph matrixGraph;
    matrixGraph.GenerateRandomConnectedGraph(test.vertices, test.density, 100);

    AdjacencyList listGraph(matrixGraph);

    int start = rand() % test.vertices;

    std::vector<double> times(4);

    auto t1 = std::chrono::high_resolution_clock::now();
    matrixGraph.Dijkstra(start);
    auto t2 = std::chrono::high_resolution_clock::now();
    times[0] = std::chrono::duration<double, std::milli>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    matrixGraph.DijkstraBinomial(start);
    t2 = std::chrono::high_resolution_clock::now();
    times[1] = std::chrono::duration<double, std::milli>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    listGraph.Dijkstra(start);
    t2 = std::chrono::high_resolution_clock::now();
    times[2] = std::chrono::duration<double, std::milli>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    listGraph.DijkstraBinomial(start);
    t2 = std::chrono::high_resolution_clock::now();
    times[3] = std::chrono::duration<double, std::milli>(t2 - t1).count();

    results.push_back(times);

    std::cout << "  Matrix + Binary:   " << times[0] << " ms\n";
    std::cout << "  Matrix + Binomial: " << times[1] << " ms\n";
    std::cout << "  List + Binary:     " << times[2] << " ms\n";
    std::cout << "  List + Binomial:   " << times[3] << " ms\n\n";
  }
  return 0;
}