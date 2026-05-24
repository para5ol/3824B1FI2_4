#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "graph.h"
#include "adjacency_list.h"

class GraphApplication {
 public:
  void Run();

 private:
  enum class GraphType {
    kMatrix,
    kList
  };

  enum class HeapType {
    kBinary,
    kBinomial
  };

  GraphType current_graph_type_ = GraphType::kMatrix;
  HeapType current_heap_type_ = HeapType::kBinary;

  std::unique_ptr<Graph> matrix_graph_;
  std::unique_ptr<AdjacencyList> list_graph_;

  int start_vertex_ = -1;
  std::vector<double> last_distances_;
  bool algorithm_run_ = false;

  void ClearGraphs();
  void CreateGraph(int size);
  void GenerateRandomGraph();
  void AddEdge();
  void RunDijkstra();
  void QueryShortestPath();
  void PrintGraph();
  void ShowMenu() const;
};

void GraphApplication::ClearGraphs() {
  matrix_graph_.reset();
  list_graph_.reset();
  algorithm_run_ = false;
  start_vertex_ = -1;
}

void GraphApplication::CreateGraph(int size) {
  if (current_graph_type_ == GraphType::kMatrix) {
    matrix_graph_ = std::make_unique<Graph>(size);
    std::cout << "Created matrix-based graph with " << size << " vertices\n";
  } else {
    list_graph_ = std::make_unique<AdjacencyList>(size);
    std::cout << "Created adjacency list-based graph with " << size
              << " vertices\n";
  }
  algorithm_run_ = false;
}

void GraphApplication::GenerateRandomGraph() {
  int vertices;
  double density;
  double max_weight;

  std::cout << "Enter number of vertices: ";
  std::cin >> vertices;
  std::cout << "Enter edge density (0.0 - 1.0): ";
  std::cin >> density;
  std::cout << "Enter maximum weight: ";
  std::cin >> max_weight;

  if (current_graph_type_ == GraphType::kMatrix) {
    if (!matrix_graph_) {
      matrix_graph_ = std::make_unique<Graph>();
    }
    matrix_graph_->GenerateRandomConnectedGraph(vertices, density, max_weight);
    std::cout << "Random connected graph generated\n";
  } else {
    if (!list_graph_) {
      list_graph_ = std::make_unique<AdjacencyList>();
    }
    list_graph_->GenerateRandomConnectedGraph(vertices, density, max_weight);
    std::cout << "Random connected graph generated\n";
  }
  algorithm_run_ = false;
}

void GraphApplication::AddEdge() {
  int from;
  int to;
  double weight;

  std::cout << "Enter from vertex: ";
  std::cin >> from;
  std::cout << "Enter to vertex: ";
  std::cin >> to;
  std::cout << "Enter weight: ";
  std::cin >> weight;

  try {
    if (current_graph_type_ == GraphType::kMatrix) {
      if (!matrix_graph_) {
        matrix_graph_ = std::make_unique<Graph>();
      }
      matrix_graph_->AddEdge(from, to, weight);
    } else {
      if (!list_graph_) {
        list_graph_ = std::make_unique<AdjacencyList>();
      }
      list_graph_->AddEdge(from - 1, to - 1, weight);
    }
    std::cout << "Edge added successfully\n";
    algorithm_run_ = false;
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << "\n";
  }
}

void GraphApplication::RunDijkstra() {
  if (current_graph_type_ == GraphType::kMatrix && !matrix_graph_) {
    std::cout << "No graph created yet!\n";
    return;
  }
  if (current_graph_type_ == GraphType::kList && !list_graph_) {
    std::cout << "No graph created yet!\n";
    return;
  }

  std::cout << "Enter start vertex: ";
  std::cin >> start_vertex_;
  start_vertex_--;

  int vertex_count = (current_graph_type_ == GraphType::kMatrix)
                         ? matrix_graph_->GetVerticesCount()
                         : list_graph_->GetVerticesCount();

  if (start_vertex_ < 0 || start_vertex_ >= vertex_count) {
    std::cout << "Invalid start vertex!\n";
    return;
  }

  try {
    if (current_heap_type_ == HeapType::kBinary) {
      std::cout << "Running Dijkstra with Binary Heap...\n";
      if (current_graph_type_ == GraphType::kMatrix) {
        last_distances_ = matrix_graph_->Dijkstra(start_vertex_);
      } else {
        last_distances_ = list_graph_->Dijkstra(start_vertex_);
      }
    } else {
      std::cout << "Running Dijkstra with Binomial Heap...\n";
      if (current_graph_type_ == GraphType::kMatrix) {
        last_distances_ = matrix_graph_->DijkstraBinomial(start_vertex_);
      } else {
        last_distances_ = list_graph_->DijkstraBinomial(start_vertex_);
      }
    }
    algorithm_run_ = true;
    std::cout << "Algorithm completed successfully!\n";
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << "\n";
    algorithm_run_ = false;
  }
}

void GraphApplication::QueryShortestPath() {
  if (!algorithm_run_) {
    std::cout << "Please run Dijkstra algorithm first!\n";
    return;
  }

  int target_vertex;
  std::cout << "Enter target vertex: ";
  std::cin >> target_vertex;
  target_vertex--;

  if (target_vertex < 0 ||
      target_vertex >= static_cast<int>(last_distances_.size())) {
    std::cout << "Invalid vertex!\n";
    return;
  }

  if (last_distances_[target_vertex] == std::numeric_limits<double>::max()) {
    std::cout << "No path from vertex " << start_vertex_ + 1
              << " to vertex " << target_vertex + 1 << "\n";
  } else {
    std::cout << "Shortest distance from vertex " << start_vertex_ + 1
              << " to vertex " << target_vertex + 1 << ": "
              << last_distances_[target_vertex] << "\n";
  }
}

void GraphApplication::PrintGraph() {
  std::cout << "\n    Graph Display\n";
  if (current_graph_type_ == GraphType::kMatrix) {
    if (matrix_graph_ && !matrix_graph_->IsEmpty()) {
      std::cout << *matrix_graph_;
    } else {
      std::cout << "Graph is empty\n";
    }
  } else {
    if (list_graph_ && !list_graph_->IsEmpty()) {
      std::cout << *list_graph_;
    } else {
      std::cout << "Graph is empty\n";
    }
  }
}

void GraphApplication::ShowMenu() const {
  std::cout << "\n========================================\n";
  std::cout << "       GRAPH ALGORITHMS MENU            \n";
  std::cout << "========================================\n";
  std::cout << "Current settings:\n";
  std::cout << "  Graph type: "
            << (current_graph_type_ == GraphType::kMatrix ? "Matrix"
                                                          : "Adjacency List")
            << "\n";
  std::cout << "  Heap type:  "
            << (current_heap_type_ == HeapType::kBinary ? "Binary"
                                                        : "Binomial")
            << "\n";
  if (algorithm_run_ && start_vertex_ >= 0) {
    std::cout << "  Last run:   Dijkstra from vertex " << start_vertex_ + 1
              << "\n";
  }
  std::cout << "========================================\n";
  std::cout << "1. Change graph type (Matrix/List)\n";
  std::cout << "2. Change heap type (Binary/Binomial)\n";
  std::cout << "3. Create new empty graph\n";
  std::cout << "4. Generate random connected graph\n";
  std::cout << "5. Add edge\n";
  std::cout << "6. Run Dijkstra algorithm\n";
  std::cout << "7. Query shortest path to vertex\n";
  std::cout << "8. Print graph\n";
  std::cout << "0. Exit\n";
  std::cout << "========================================\n";
  std::cout << "Your choice: ";
}

void GraphApplication::Run() {
  int choice;

  std::cout << "Welcome to Graph Algorithms Application!\n";
  std::cout << "This program demonstrates Dijkstra algorithm on "
            << "different graph representations.\n";

  do {
    ShowMenu();
    std::cin >> choice;

    switch (choice) {
      case 1: {
        std::cout << "Select graph type:\n";
        std::cout << "1. Matrix-based graph\n";
        std::cout << "2. Adjacency list-based graph\n";
        std::cout << "Your choice: ";
        int type;
        std::cin >> type;
        if (type == 1) {
          current_graph_type_ = GraphType::kMatrix;
          ClearGraphs();
          std::cout << "Switched to matrix-based graph\n";
        } else if (type == 2) {
          current_graph_type_ = GraphType::kList;
          ClearGraphs();
          std::cout << "Switched to adjacency list-based graph\n";
        } else {
          std::cout << "Invalid choice!\n";
        }
        break;
      }
      case 2: {
        std::cout << "Select heap type:\n";
        std::cout << "1. Binary heap\n";
        std::cout << "2. Binomial heap\n";
        std::cout << "Your choice: ";
        int type;
        std::cin >> type;
        if (type == 1) {
          current_heap_type_ = HeapType::kBinary;
          std::cout << "Switched to binary heap\n";
        } else if (type == 2) {
          current_heap_type_ = HeapType::kBinomial;
          std::cout << "Switched to binomial heap\n";
        } else {
          std::cout << "Invalid choice!\n";
        }
        algorithm_run_ = false;
        break;
      }
      case 3: {
        int size;
        std::cout << "Enter number of vertices: ";
        std::cin >> size;
        if (size > 0) {
          CreateGraph(size);
        } else {
          std::cout << "Invalid size!\n";
        }
        break;
      }
      case 4:
        GenerateRandomGraph();
        break;
      case 5:
        AddEdge();
        break;
      case 6:
        RunDijkstra();
        break;
      case 7:
        QueryShortestPath();
        break;
      case 8:
        PrintGraph();
        break;
      case 0:
        std::cout << "Thank you for using the application!\n";
        break;
      default:
        std::cout << "Invalid choice! Please try again.\n";
    }
  } while (choice != 0);
}

int main() {
  try {
    GraphApplication app;
    app.Run();
  } catch (const std::exception& e) {
    std::cout << "Fatal error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}