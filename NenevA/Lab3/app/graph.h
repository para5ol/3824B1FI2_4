#pragma once
//TMatrix is from one of the previous labs
#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "triangle_matrix.h"
#include "vertex_distance.h"
#include "binary_heap.h"
#include "binomial_heap.h"

class Graph {
 public:
  Graph();
  explicit Graph(int vertices_number);

  void AddEdge(int from, int to, double weight);
  void EraseRecord(int first_vertex_number, int second_vertex_number);
  void EraseVertex(int vertex_number);
  void Resize(int new_size);
  void Clear();
  void MakeConnected(double default_weight = 1.0);
  void GenerateRandomConnectedGraph(int num_vertices, double density = 0.5,
                                    double max_weight = 100);

  int GetVerticesCount() const;
  int GetEdgesCount() const;
  double GetEdgeWeight(int from, int to) const;
  std::vector<Edge> GetNeighbors(int vertex) const;
  
  bool HasEdge(int from, int to) const;
  bool IsEmpty() const;
  bool IsConnected() const;
  bool IsValidForDijkstra() const;

  std::vector<double> Dijkstra(int start_vertex) const;
  std::vector<double> DijkstraBinomial(int start_vertex) const;

  friend std::istream& operator>>(std::istream& in, Graph& graph);
  friend std::ostream& operator<<(std::ostream& out, const Graph& graph);

 private:
  static void Normalize(int& a, int& b) {
    if (a > b) std::swap(a, b);
  }

  TMatrix<double> data_;
};

Graph::Graph() : data_(0) {}

Graph::Graph(int vertices_number) {
  if (vertices_number < 0) {
    throw std::invalid_argument("Negative vertex count");
  }
  
  data_ = TMatrix<double>(vertices_number);
  for (int i = 0; i < data_.GetSize(); ++i) {
    for (int j = i; j < data_.GetSize(); ++j) {
      data_[i][j] = -1.0;
    }
  }
}

void Graph::AddEdge(int from, int to, double weight) {
  if (weight < 0) {
    throw std::runtime_error("Weights below zero are not allowed");
  }

  int u = from - 1;
  int v = to - 1;
  Normalize(u, v);

  if (v >= data_.GetSize()) {
    Resize(v + 1);
  }

  data_[u][v] = weight;
}

void Graph::EraseRecord(int first_vertex_number, int second_vertex_number) {
  int u = first_vertex_number - 1;
  int v = second_vertex_number - 1;
  Normalize(u, v);
  
  if (u < 0 || v >= data_.GetSize()) {
    throw std::out_of_range("Vertex index out of range");
  }
  data_[u][v] = -1.0;
}

void Graph::EraseVertex(int vertex_number) {
  for (int i = 0; i < data_.GetSize(); ++i) {
    for (int j = i; j < data_.GetSize(); ++j) {
      if (vertex_number - 1 == i || vertex_number - 1 == j) {
        data_[i][j] = -1.0;
      }
    }
  }
}

void Graph::Resize(int new_size) {
  if (new_size <= data_.GetSize()) return;
  
  TMatrix<double> new_data(new_size);
  int old = data_.GetSize();

  for (int i = 0; i < old; ++i) {
    for (int j = i; j < old; ++j) {
      new_data[i][j] = data_[i][j];
    }
  }

  for (int i = 0; i < new_size; ++i) {
    for (int j = i; j < new_size; ++j) {
      if (i >= old || j >= old) {
        new_data[i][j] = -1.0;
      }
    }
  }
  
  data_ = std::move(new_data);
}

void Graph::Clear() {
  data_ = TMatrix<double>(0);
}

void Graph::MakeConnected(double default_weight) {
  int n = data_.GetSize();
  if (n <= 1) return;

  std::vector<bool> visited(n, false);
  std::vector<int> representatives;

  for (int v = 0; v < n; ++v) {
    if (!visited[v]) {
      std::vector<int> stack = {v};
      visited[v] = true;
      representatives.push_back(v);

      while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (const auto& edge : GetNeighbors(u)) {
          int neighbor = edge.vertex_number_;
          if (!visited[neighbor]) {
            visited[neighbor] = true;
            stack.push_back(neighbor);
          }
        }
      }
    }
  }

  for (size_t i = 0; i + 1 < representatives.size(); ++i) {
    int u = representatives[i];
    int v = representatives[i + 1];
    AddEdge(u + 1, v + 1, default_weight);
  }
}

void Graph::GenerateRandomConnectedGraph(int num_vertices, double density,
                                          double max_weight) {
  if (num_vertices <= 0) return;

  TMatrix<double> temp(num_vertices);

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i; j < num_vertices; ++j) {
      temp[i][j] = -1.0;
    }
  }

  data_ = temp;

  std::srand(std::time(nullptr));

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      double random_value = std::rand() / static_cast<double>(RAND_MAX);
      if (random_value < density) {
        double weight = random_value * max_weight;
        AddEdge(i + 1, j + 1, weight);
      }
    }
  }

  MakeConnected(1.0);
}

int Graph::GetVerticesCount() const {
  return data_.GetSize();
}

int Graph::GetEdgesCount() const {
  int n = data_.GetSize();
  int count = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      if (data_[i][j] != -1.0) {
        ++count;
      }
    }
  }
  return count;
}

double Graph::GetEdgeWeight(int from, int to) const {
  int u = from - 1;
  int v = to - 1;
  Normalize(u, v);
  
  if (v >= data_.GetSize()) {
    throw std::out_of_range("Vertex index out of range");
  }
  return data_[u][v];
}

std::vector<Edge> Graph::GetNeighbors(int vertex) const {
  if (vertex < 0 || vertex >= data_.GetSize()) {
    throw std::out_of_range("Vertex index out of range");
  }

  std::vector<Edge> neighbors;
  int n = data_.GetSize();

  for (int j = vertex; j < n; ++j) {
    if (data_[vertex][j] != -1.0) {
      neighbors.push_back({j, data_[vertex][j]});
    }
  }

  for (int i = 0; i < vertex; ++i) {
    if (data_[i][vertex] != -1.0) {
      neighbors.push_back({i, data_[i][vertex]});
    }
  }
  
  return neighbors;
}

bool Graph::HasEdge(int from, int to) const {
  int u = from - 1;
  int v = to - 1;
  Normalize(u, v);
  
  if (u < 0 || v >= data_.GetSize()) {
    throw std::out_of_range("Vertex index out of range");
  }
  return data_[u][v] != -1.0;
}

bool Graph::IsEmpty() const {
  for (int i = 0; i < data_.GetSize(); ++i) {
    for (int j = i; j < data_.GetSize(); ++j) {
      if (data_[i][j] != -1.0) {
        return false;
      }
    }
  }
  return true;
}

bool Graph::IsConnected() const {
  int n = GetVerticesCount();
  if (n <= 1) return true;

  std::vector<bool> visited(n, false);
  std::vector<int> stack;

  stack.push_back(0);
  visited[0] = true;
  int visited_count = 1;

  while (!stack.empty()) {
    int current = stack.back();
    stack.pop_back();

    auto neighbors = GetNeighbors(current);
    for (const auto& edge : neighbors) {
      int neighbor = edge.vertex_number_;
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        ++visited_count;
        stack.push_back(neighbor);
      }
    }
  }

  return visited_count == n;
}

bool Graph::IsValidForDijkstra() const {
  int n = GetVerticesCount();

  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      double weight = data_[i][j];
      if (weight != -1.0 && weight < 0) {
        return false;
      }
    }
  }

  return true;
}

std::vector<double> Graph::Dijkstra(int start_vertex) const {
  if (start_vertex < 0 || start_vertex >= GetVerticesCount()) {
    throw std::out_of_range("Invalid start vertex");
  }
  
  if (!IsConnected() || !IsValidForDijkstra()) {
    throw std::invalid_argument("Dijkstra algorithm can't be done\n");
  }

  std::vector<double> distances(GetVerticesCount(),
                                std::numeric_limits<double>::max());
  distances[start_vertex] = 0;

  BinaryHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(start_vertex, 0));

  std::vector<bool> visited(GetVerticesCount(), false);

  while (!heap.IsEmpty()) {
    VertexDistance current = heap.ExtractMin();
    if (visited[current.vertex]) continue;
    visited[current.vertex] = true;

    for (const auto& edge : GetNeighbors(current.vertex)) {
      int neighbor = edge.vertex_number_;
      double weight = edge.distance_;
      
      if (!visited[neighbor] &&
          distances[current.vertex] + weight < distances[neighbor]) {
        distances[neighbor] = distances[current.vertex] + weight;
        heap.Insert(VertexDistance(neighbor, distances[neighbor]));
      }
    }
  }
  
  return distances;
}

std::vector<double> Graph::DijkstraBinomial(int start_vertex) const {
  if (start_vertex < 0 || start_vertex >= GetVerticesCount()) {
    throw std::out_of_range("Invalid start vertex");
  }
  
  if (!IsConnected() || !IsValidForDijkstra()) {
    throw std::invalid_argument("Dijkstra algorithm can't be done\n");
  }
  
  std::vector<double> distances(GetVerticesCount(),
                                std::numeric_limits<double>::max());
  distances[start_vertex] = 0;

  BinomialHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(start_vertex, 0));

  std::vector<bool> visited(GetVerticesCount(), false);

  while (!heap.IsEmpty()) {
    VertexDistance current = heap.ExtractMin();

    if (visited[current.vertex]) {
      continue;
    }
    visited[current.vertex] = true;

    const auto& neighbors = GetNeighbors(current.vertex);
    for (const auto& edge : neighbors) {
      int neighbor = edge.vertex_number_;
      double weight = edge.distance_;

      if (!visited[neighbor] &&
          distances[current.vertex] + weight < distances[neighbor]) {
        distances[neighbor] = distances[current.vertex] + weight;
        heap.Insert(VertexDistance(neighbor, distances[neighbor]));
      }
    }
  }
  
  return distances;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
  int vertices_count, edges_count;
  in >> vertices_count >> edges_count;

  graph.Clear();
  graph.data_ = TMatrix<double>(vertices_count);

  for (int i = 0; i < vertices_count; ++i) {
    for (int j = i; j < vertices_count; ++j) {
      graph.data_[i][j] = -1.0;
    }
  }

  for (int i = 0; i < edges_count; ++i) {
    int from, to;
    double weight;
    in >> from >> to >> weight;
    graph.AddEdge(from, to, weight);
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  int n = graph.GetVerticesCount();
  out << "Graph on " << n << " vertices:\n";

  for (int v = 0; v < n; ++v) {
    out << "Vertex " << (v + 1) << ": ";
    auto neighbors = graph.GetNeighbors(v);
    for (const auto& edge : neighbors) {
      out << "(" << (edge.vertex_number_ + 1)
          << ", weight=" << edge.distance_ << ") ";
    }
    out << "\n";
  }

  return out;
}