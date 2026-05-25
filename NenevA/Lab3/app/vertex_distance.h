#pragma once
struct VertexDistance 
{ // In an separate file as it's used both in graph.cpp and adjacency_list.cpp 
  // Struct for Dijkstra algorithm
  int vertex;
  double distance;
  VertexDistance() : vertex(-1), distance(std::numeric_limits<double>::max()) {}
  VertexDistance(int start_vertex, double dist) : vertex(start_vertex), distance(dist) {}
  bool operator<(const VertexDistance& other) const { return distance < other.distance; }
  bool operator<=(const VertexDistance& other) const { return distance <= other.distance; }
  bool operator>(const VertexDistance& other) const { return distance > other.distance; }
  bool operator>=(const VertexDistance& other) const { return distance >= other.distance; }
};

struct Edge 
{
  int vertex_number_;
  double distance_;
};