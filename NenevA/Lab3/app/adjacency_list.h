#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "vertex_distance.h"
#include "binary_heap.h"
#include "binomial_heap.h"
#include "graph.h"

class AdjacencyList {
 public:
  AdjacencyList() = default;
  explicit AdjacencyList(int vertices_count);
  explicit AdjacencyList(const Graph& matrix_graph);

  void AddEdge(int from, int to, double weight);
  void EraseRecord(int first_vertex_number, int second_vertex_number);
  void EraseVertex(int vertex_number);
  void Clear();
  void MakeConnected(double default_weight);
  void GenerateRandomConnectedGraph(int num_vertices, double density = 0.5,
                                    double max_weight = 100);

  int GetEdgesCount() const;
  int GetVerticesCount() const;
  double GetEdgeWeight(int from, int to) const;
  const std::vector<Edge>& GetNeighbors(int vertex) const;
  
  bool HasEdge(int from, int to) const;
  bool IsEmpty() const;
  bool IsConnected() const;
  bool IsValidForDijkstra() const;

  std::vector<double> Dijkstra(int start_vertex) const;
  std::vector<double> DijkstraBinomial(int start_vertex) const;

  friend std::istream& operator>>(std::istream& in, AdjacencyList& list);
  friend std::ostream& operator<<(std::ostream& out, const AdjacencyList& list);

 private:
  std::vector<std::vector<Edge>> adj_list_;
  int vertices_count_ = 0;
};

AdjacencyList::AdjacencyList(int vertices_count)
    : adj_list_(vertices_count), vertices_count_(vertices_count) {}

AdjacencyList::AdjacencyList(const Graph& matrix_graph) {
  int n = matrix_graph.GetVerticesCount();
  if (n == 0) {
    vertices_count_ = 0;
    return;
  }
  
  adj_list_.assign(n, std::vector<Edge>());
  vertices_count_ = n;

  for (int i = 0; i < n; ++i) {
    auto neighbors = matrix_graph.GetNeighbors(i);
    for (const auto& edge : neighbors) {
      adj_list_[i].push_back(edge);
    }
  }
}

void AdjacencyList::AddEdge(int from, int to, double weight) {
  int max_vertex = std::max(from, to);
  if (max_vertex >= vertices_count_) {
    adj_list_.resize(max_vertex + 1);
    vertices_count_ = max_vertex + 1;
  }

  if (!HasEdge(from, to)) {
    adj_list_[from].push_back({to, weight});
    adj_list_[to].push_back({from, weight});
  }
}

void AdjacencyList::EraseRecord(int first_vertex_number,
                                int second_vertex_number) {
  if (first_vertex_number >= vertices_count_ ||
      second_vertex_number >= vertices_count_) {
    return;
  }

  auto& edges_from_first = adj_list_[first_vertex_number];
  for (size_t i = 0; i < edges_from_first.size(); ++i) {
    if (edges_from_first[i].vertex_number_ == second_vertex_number) {
      edges_from_first.erase(edges_from_first.begin() + i);
      break;
    }
  }

  auto& edges_from_second = adj_list_[second_vertex_number];
  for (size_t i = 0; i < edges_from_second.size(); ++i) {
    if (edges_from_second[i].vertex_number_ == first_vertex_number) {
      edges_from_second.erase(edges_from_second.begin() + i);
      break;
    }
  }
}

void AdjacencyList::EraseVertex(int vertex_number) {
  if (vertex_number >= vertices_count_) {
    return;
  }
  
  for (const auto& edge : adj_list_[vertex_number]) {
    EraseRecord(edge.vertex_number_, vertex_number);
  }
  adj_list_[vertex_number].clear();
}

void AdjacencyList::Clear() {
  adj_list_.clear();
  vertices_count_ = 0;
}

void AdjacencyList::MakeConnected(double default_weight) {
  if (vertices_count_ <= 1) return;

  std::vector<bool> visited(vertices_count_, false);
  std::vector<int> representatives;

  for (int v = 0; v < vertices_count_; ++v) {
    if (!visited[v]) {
      std::vector<int> stack = {v};
      visited[v] = true;
      representatives.push_back(v);

      while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (const auto& edge : adj_list_[u]) {
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
    AddEdge(u, v, default_weight);
  }
}

void AdjacencyList::GenerateRandomConnectedGraph(int num_vertices,
                                                  double density,
                                                  double max_weight) {
  Clear();
  if (num_vertices <= 0) return;

  adj_list_.assign(num_vertices, std::vector<Edge>());
  vertices_count_ = num_vertices;

  std::srand(std::time(nullptr));

  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      double random_value = std::rand() / static_cast<double>(RAND_MAX);
      if (random_value < density) {
        double weight = random_value * max_weight;
        AddEdge(i, j, weight);
      }
    }
  }

  MakeConnected(1.0);
}

int AdjacencyList::GetEdgesCount() const {
  int edges_count = 0;
  for (const auto& edges : adj_list_) {
    edges_count += edges.size();
  }
  return edges_count / 2;
}

int AdjacencyList::GetVerticesCount() const {
  return vertices_count_;
}

double AdjacencyList::GetEdgeWeight(int from, int to) const {
  if (from >= vertices_count_ || to >= vertices_count_ ||
      from < 0 || to < 0) {
    throw std::out_of_range("Vertex index out of range");
  }

  const auto& edges = adj_list_[from];
  for (size_t i = 0; i < edges.size(); ++i) {
    if (edges[i].vertex_number_ == to) {
      return edges[i].distance_;
    }
  }

  throw std::runtime_error("Edge not found");
}

const std::vector<Edge>& AdjacencyList::GetNeighbors(int vertex) const {
  if (vertex >= vertices_count_ || vertex < 0) {
    throw std::out_of_range("Vertex index out of range");
  }
  return adj_list_[vertex];
}

bool AdjacencyList::HasEdge(int from, int to) const {
  if (from >= vertices_count_ || to >= vertices_count_ ||
      from < 0 || to < 0) {
    return false;
  }

  const auto& edges = adj_list_[from];
  for (size_t i = 0; i < edges.size(); ++i) {
    if (edges[i].vertex_number_ == to) {
      return true;
    }
  }
  return false;
}

bool AdjacencyList::IsEmpty() const {
  return vertices_count_ == 0;
}

bool AdjacencyList::IsConnected() const {
  if (vertices_count_ <= 1) return true;

  std::vector<bool> visited(vertices_count_, false);
  std::vector<int> stack;

  stack.push_back(0);
  visited[0] = true;
  int visited_count = 1;

  while (!stack.empty()) {
    int current = stack.back();
    stack.pop_back();

    for (const auto& edge : adj_list_[current]) {
      int neighbor = edge.vertex_number_;
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        ++visited_count;
        stack.push_back(neighbor);
      }
    }
  }

  return visited_count == vertices_count_;
}

bool AdjacencyList::IsValidForDijkstra() const {
  for (int i = 0; i < vertices_count_; ++i) {
    for (const auto& edge : adj_list_[i]) {
      if (edge.distance_ < 0) {
        return false;
      }
    }
  }
  return true;
}

std::vector<double> AdjacencyList::Dijkstra(int start_vertex) const {
  if (start_vertex < 0 || start_vertex >= vertices_count_) {
    throw std::out_of_range("Invalid start vertex");
  }
  
  if (!IsConnected() || !IsValidForDijkstra()) {
    throw std::invalid_argument("Dijkstra algorithm can't be done\n");
  }
  
  std::vector<double> distances(vertices_count_,
                                std::numeric_limits<double>::max());
  distances[start_vertex] = 0;

  BinaryHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(start_vertex, 0));

  std::vector<bool> visited(vertices_count_, false);

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

std::vector<double> AdjacencyList::DijkstraBinomial(int start_vertex) const {
  if (start_vertex < 0 || start_vertex >= vertices_count_) {
    throw std::out_of_range("Invalid start vertex");
  }
  
  if (!IsConnected() || !IsValidForDijkstra()) {
    throw std::invalid_argument("Dijkstra algorithm can't be done\n");
  }
  
  std::vector<double> distances(vertices_count_,
                                std::numeric_limits<double>::max());
  distances[start_vertex] = 0;

  BinomialHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(start_vertex, 0));

  std::vector<bool> visited(vertices_count_, false);

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

std::istream& operator>>(std::istream& in, AdjacencyList& list) {
  int vertices_count, edges_count;
  in >> vertices_count >> edges_count;

  list.Clear();
  list.adj_list_.resize(vertices_count);
  list.vertices_count_ = vertices_count;

  for (int i = 0; i < edges_count; ++i) {
    int from, to;
    double weight;
    in >> from >> to >> weight;
    list.AddEdge(from, to, weight);
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const AdjacencyList& list) {
  out << "Graph on " << list.vertices_count_ << " vertices:\n";

  for (int i = 0; i < list.vertices_count_; ++i) {
    out << "Vertex " << i + 1 << ": ";
    for (const auto& edge : list.adj_list_[i]) {
      out << "(" << edge.vertex_number_ + 1
          << ", weight=" << edge.distance_ << ") ";
    }
    out << "\n";
  }

  return out;
}