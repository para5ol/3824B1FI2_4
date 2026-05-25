#include "pch.h"

#include <vector>
#include <string>

#include "adjacency_list.h"
#include "graph.h"
#include "binary_heap.h"
#include "binomial_heap.h"

//AdjacencyList Tests

TEST(AdjacencyListTest, DijkstraConsistency) 
{
  AdjacencyList graph;
  graph.GenerateRandomConnectedGraph(15, 0.4, 100);
  int n = graph.GetVerticesCount();
  
  for (int start = 0; start < n; ++start) 
  {
    auto distances_binary = graph.Dijkstra(start);
    auto distances_binomial = graph.DijkstraBinomial(start);
    
    ASSERT_EQ(distances_binary.size(), distances_binomial.size());
    for (int i = 0; i < n; ++i) 
    {
      EXPECT_DOUBLE_EQ(distances_binary[i], distances_binomial[i]);
    }
  }
}

TEST(AdjacencyListTest, FixedGraph) 
{
  AdjacencyList graph(4);
  graph.AddEdge(0, 1, 10.0);
  graph.AddEdge(1, 2, 20.0);
  graph.AddEdge(2, 3, 30.0);
  graph.AddEdge(0, 3, 100.0);
  graph.AddEdge(1, 3, 5.0);

  auto distances_binary = graph.Dijkstra(0);
  EXPECT_DOUBLE_EQ(distances_binary[0], 0.0);
  EXPECT_DOUBLE_EQ(distances_binary[1], 10.0);
  EXPECT_DOUBLE_EQ(distances_binary[2], 30.0);
  EXPECT_DOUBLE_EQ(distances_binary[3], 15.0);

  auto distances_binomial = graph.DijkstraBinomial(0);
  EXPECT_DOUBLE_EQ(distances_binomial[0], 0.0);
  EXPECT_DOUBLE_EQ(distances_binomial[1], 10.0);
  EXPECT_DOUBLE_EQ(distances_binomial[2], 30.0);
  EXPECT_DOUBLE_EQ(distances_binomial[3], 15.0);
}

TEST(AdjacencyListTest, SingleVertex) 
{
  AdjacencyList graph(1);
  
  auto distances_binary = graph.Dijkstra(0);
  ASSERT_EQ(distances_binary.size(), 1);
  EXPECT_DOUBLE_EQ(distances_binary[0], 0.0);
  
  auto distances_binomial = graph.DijkstraBinomial(0);
  EXPECT_DOUBLE_EQ(distances_binomial[0], 0.0);
}

TEST(AdjacencyListTest, EmptyGraph) 
{
  AdjacencyList graph;
  EXPECT_TRUE(graph.IsEmpty());
  EXPECT_EQ(graph.GetVerticesCount(), 0);
  EXPECT_EQ(graph.GetEdgesCount(), 0);
}

TEST(AdjacencyListTest, AddAndRemoveEdges) 
{
  AdjacencyList graph(3);
  graph.AddEdge(0, 1, 5.0);
  graph.AddEdge(1, 2, 10.0);
  
  EXPECT_TRUE(graph.HasEdge(0, 1));
  EXPECT_TRUE(graph.HasEdge(1, 2));
  EXPECT_FALSE(graph.HasEdge(0, 2));
  EXPECT_EQ(graph.GetEdgesCount(), 2);
  
  graph.EraseRecord(0, 1);
  EXPECT_FALSE(graph.HasEdge(0, 1));
  EXPECT_EQ(graph.GetEdgesCount(), 1);
}

//Graph (Matrix) Tests

TEST(GraphTest, DijkstraConsistency)
{
  Graph graph;
  graph.GenerateRandomConnectedGraph(15, 0.4, 100);
  int n = graph.GetVerticesCount();
  
  for (int start = 0; start < n; ++start)
  {
    auto distances_binary = graph.Dijkstra(start);
    auto distances_binomial = graph.DijkstraBinomial(start);
    
    ASSERT_EQ(distances_binary.size(), distances_binomial.size());
    for (int i = 0; i < n; ++i) 
    {
      EXPECT_DOUBLE_EQ(distances_binary[i], distances_binomial[i]);
    }
  }
}

TEST(GraphTest, FixedGraph) 
{
  Graph graph(4);
  graph.AddEdge(1, 2, 10.0);
  graph.AddEdge(2, 3, 20.0);
  graph.AddEdge(3, 4, 30.0);
  graph.AddEdge(1, 4, 100.0);
  graph.AddEdge(2, 4, 5.0);

  auto distances_binary = graph.Dijkstra(0);
  EXPECT_DOUBLE_EQ(distances_binary[0], 0.0);
  EXPECT_DOUBLE_EQ(distances_binary[1], 10.0);
  EXPECT_DOUBLE_EQ(distances_binary[2], 30.0);
  EXPECT_DOUBLE_EQ(distances_binary[3], 15.0);

  auto distances_binomial = graph.DijkstraBinomial(0);
  EXPECT_DOUBLE_EQ(distances_binomial[0], 0.0);
  EXPECT_DOUBLE_EQ(distances_binomial[1], 10.0);
  EXPECT_DOUBLE_EQ(distances_binomial[2], 30.0);
  EXPECT_DOUBLE_EQ(distances_binomial[3], 15.0);
}

TEST(GraphTest, SingleVertex)
{
  Graph graph(1);
  
  auto distances_binary = graph.Dijkstra(0);
  ASSERT_EQ(distances_binary.size(), 1);
  EXPECT_DOUBLE_EQ(distances_binary[0], 0.0);
  
  auto distances_binomial = graph.DijkstraBinomial(0);
  EXPECT_DOUBLE_EQ(distances_binomial[0], 0.0);
}

TEST(GraphTest, EmptyGraph)
{
  Graph graph;
  EXPECT_TRUE(graph.IsEmpty());
  EXPECT_EQ(graph.GetVerticesCount(), 0);
  EXPECT_EQ(graph.GetEdgesCount(), 0);
}

TEST(GraphTest, AddAndRemoveEdges)
{
  Graph graph(3);
  graph.AddEdge(1, 2, 5.0);
  graph.AddEdge(2, 3, 10.0);
  
  EXPECT_TRUE(graph.HasEdge(1, 2));
  EXPECT_TRUE(graph.HasEdge(2, 3));
  EXPECT_FALSE(graph.HasEdge(1, 3));
  EXPECT_EQ(graph.GetEdgesCount(), 2);
  
  graph.EraseRecord(1, 2);
  EXPECT_FALSE(graph.HasEdge(1, 2));
  EXPECT_EQ(graph.GetEdgesCount(), 1);
}

//Binary Heap Tests

TEST(BinaryHeapTest, InsertAndExtractMin)
{
  BinaryHeap<int> heap;
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(8);
  heap.Insert(1);
  heap.Insert(4);
  
  EXPECT_EQ(heap.ExtractMin(), 1);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 4);
  EXPECT_EQ(heap.ExtractMin(), 5);
  EXPECT_EQ(heap.ExtractMin(), 8);
  EXPECT_TRUE(heap.IsEmpty());
}

TEST(BinaryHeapTest, InsertDuplicateValues)
{
  BinaryHeap<int> heap;
  heap.Insert(5);
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(3);
  heap.Insert(1);
  
  EXPECT_EQ(heap.ExtractMin(), 1);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 5);
  EXPECT_EQ(heap.ExtractMin(), 5);
}

TEST(BinaryHeapTest, WithVertexDistance)
{
  BinaryHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(0, 10.0));
  heap.Insert(VertexDistance(1, 5.0));
  heap.Insert(VertexDistance(2, 15.0));
  heap.Insert(VertexDistance(3, 3.0));
  
  EXPECT_EQ(heap.ExtractMin().distance, 3.0);
  EXPECT_EQ(heap.ExtractMin().distance, 5.0);
  EXPECT_EQ(heap.ExtractMin().distance, 10.0);
  EXPECT_EQ(heap.ExtractMin().distance, 15.0);
}
//Binomial Heap Tests

TEST(BinomialHeapTest, InsertAndExtractMin)
{
  BinomialHeap<int> heap;
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(8);
  heap.Insert(1);
  heap.Insert(4);
  
  EXPECT_EQ(heap.ExtractMin(), 1);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 4);
  EXPECT_EQ(heap.ExtractMin(), 5);
  EXPECT_EQ(heap.ExtractMin(), 8);
  EXPECT_TRUE(heap.IsEmpty());
}
TEST(BinomialHeapTest, InsertDuplicateValues)
{
  BinomialHeap<int> heap;
  heap.Insert(5);
  heap.Insert(5);
  heap.Insert(3);
  heap.Insert(3);
  heap.Insert(1);
  
  EXPECT_EQ(heap.ExtractMin(), 1);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 3);
  EXPECT_EQ(heap.ExtractMin(), 5);
  EXPECT_EQ(heap.ExtractMin(), 5);
}

TEST(BinomialHeapTest, WithVertexDistance)
{
  BinomialHeap<VertexDistance> heap;
  heap.Insert(VertexDistance(0, 10.0));
  heap.Insert(VertexDistance(1, 5.0));
  heap.Insert(VertexDistance(2, 15.0));
  heap.Insert(VertexDistance(3, 3.0));
  
  EXPECT_EQ(heap.ExtractMin().distance, 3.0);
  EXPECT_EQ(heap.ExtractMin().distance, 5.0);
  EXPECT_EQ(heap.ExtractMin().distance, 10.0);
  EXPECT_EQ(heap.ExtractMin().distance, 15.0);
}

TEST(BinomialHeapTest, MergeHeaps)
{
  BinomialHeap<int> heap1;
  heap1.Insert(5);
  heap1.Insert(3);
  heap1.Insert(8);
  
  BinomialHeap<int> heap2;
  heap2.Insert(1);
  heap2.Insert(4);
  heap2.Insert(7);
  
  heap1.Merge(heap2);
  
  EXPECT_EQ(heap1.ExtractMin(), 1);
  EXPECT_EQ(heap1.ExtractMin(), 3);
  EXPECT_EQ(heap1.ExtractMin(), 4);
  EXPECT_EQ(heap1.ExtractMin(), 5);
  EXPECT_EQ(heap1.ExtractMin(), 7);
  EXPECT_EQ(heap1.ExtractMin(), 8);
}

// Cross-Validation Tests

TEST(CrossValidationTest, MatrixVsListConsistency)
{
  const int kNumVertices = 20;
  const double kDensity = 0.3;
  const double kMaxWeight = 100;
  
  Graph matrix_graph;
  matrix_graph.GenerateRandomConnectedGraph(kNumVertices, kDensity, kMaxWeight);
  
  AdjacencyList list_graph(matrix_graph);
  
  // Random start vertex
  int start = rand() % kNumVertices;
  
  auto matrix_distances = matrix_graph.Dijkstra(start);
  auto list_distances = list_graph.Dijkstra(start);
  
  ASSERT_EQ(matrix_distances.size(), list_distances.size());
  for (int i = 0; i < kNumVertices; ++i) {
    EXPECT_DOUBLE_EQ(matrix_distances[i], list_distances[i]);
  }
}

TEST(CrossValidationTest, BinaryVsBinomialHeapOnSameGraph) 
{
  const int kNumVertices = 30;
  const double kDensity = 0.4;
  const double kMaxWeight = 100;
  
  Graph graph;
  graph.GenerateRandomConnectedGraph(kNumVertices, kDensity, kMaxWeight);
  
  int start = rand() % kNumVertices;
  
  auto binary_distances = graph.Dijkstra(start);
  auto binomial_distances = graph.DijkstraBinomial(start);
  
  for (int i = 0; i < kNumVertices; ++i) 
  {
    EXPECT_DOUBLE_EQ(binary_distances[i], binomial_distances[i]);
  }
}

//Edge Case Tests 

TEST(EdgeCaseTest, GraphWithZeroWeightEdges)
{
  Graph graph(3);
  graph.AddEdge(1, 2, 0.0);
  graph.AddEdge(2, 3, 5.0);
  
  auto distances = graph.Dijkstra(0);
  EXPECT_DOUBLE_EQ(distances[0], 0.0);
  EXPECT_DOUBLE_EQ(distances[1], 0.0);
  EXPECT_DOUBLE_EQ(distances[2], 5.0);
}

TEST(EdgeCaseTest, DisconnectedGraphThrowsException)
{
  Graph graph(3);
  graph.AddEdge(1, 2, 5.0);
  // Vertex 3 is isolated
  
  EXPECT_ANY_THROW(graph.Dijkstra(0));
  EXPECT_ANY_THROW(graph.DijkstraBinomial(0));
}

TEST(EdgeCaseTest, InvalidStartVertexThrowsException) 
{
  Graph graph(3);
  graph.GenerateRandomConnectedGraph(3, 0.5, 100);
  
  EXPECT_THROW(graph.Dijkstra(-1), std::out_of_range);
  EXPECT_THROW(graph.Dijkstra(10), std::out_of_range);
  EXPECT_THROW(graph.DijkstraBinomial(-1), std::out_of_range);
  EXPECT_THROW(graph.DijkstraBinomial(10), std::out_of_range);
}