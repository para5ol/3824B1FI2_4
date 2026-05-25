#pragma once

#include <iostream>
#include <limits>
#include <stdexcept>

template <typename ValueType>
struct Node 
{
  Node() : key(ValueType()), parent(nullptr), left_child(nullptr),
           sibling(nullptr), degree(0) {}
  
  explicit Node(ValueType key) : key(key), parent(nullptr), left_child(nullptr),
                                  sibling(nullptr), degree(0) {}
  
  ValueType key;
  Node* parent;
  Node* left_child;
  Node* sibling;
  int degree;
};

template <typename ValueType>
class BinomialHeap 
{
 public:
  BinomialHeap();
  explicit BinomialHeap(ValueType key);
  BinomialHeap(const BinomialHeap& other);
  BinomialHeap& operator=(const BinomialHeap& other);
  ~BinomialHeap();

  void Merge(BinomialHeap& other);
  // Merges with a binomial heap containing only one element
  void Insert(ValueType key);
  void DecreaseKey(Node<ValueType>* node, ValueType key);
  void Delete(Node<ValueType>* node);

  bool IsEmpty() const;
  ValueType ExtractMin();
  Node<ValueType>* GetMin() const;

 private:
  Node<ValueType>* head_;

  // In order to ease Merge
  void LinkTrees(Node<ValueType>* parent, Node<ValueType>* child);
  void Clear();
  Node<ValueType>* CopyTree(Node<ValueType>* node, Node<ValueType>* parent);
};

// ==================== Constructor and Destructor ====================

template <typename ValueType>
BinomialHeap<ValueType>::BinomialHeap() : head_(nullptr) {}

template <typename ValueType>
BinomialHeap<ValueType>::BinomialHeap(ValueType key) : head_(new Node<ValueType>(key)) {}

template <typename ValueType>
BinomialHeap<ValueType>::BinomialHeap(const BinomialHeap& other) : 
    head_(nullptr) 
{
  if (!other.IsEmpty()) 
  {
    head_ = CopyTree(other.head_, nullptr);
  }
}

template <typename ValueType>
BinomialHeap<ValueType>& BinomialHeap<ValueType>::operator=(const BinomialHeap& other) 
{
  if (this != &other) 
  {
    Clear();
    if (!other.IsEmpty()) 
    {
      head_ = CopyTree(other.head_, nullptr);
    }
  }
  return *this;
}

template <typename ValueType>
BinomialHeap<ValueType>::~BinomialHeap() 
{
  Clear();
}

// ==================== Private Methods ====================

template <typename ValueType>
void BinomialHeap<ValueType>::Clear() 
{
  while (!IsEmpty()) 
  {
    ExtractMin();
  }
}

template <typename ValueType>
Node<ValueType>* BinomialHeap<ValueType>::CopyTree(Node<ValueType>* node,
                                                    Node<ValueType>* parent) 
{
  if (node == nullptr) return nullptr;

  Node<ValueType>* new_node = new Node<ValueType>(node->key);
  new_node->degree = node->degree;
  new_node->parent = parent;
  new_node->left_child = CopyTree(node->left_child, new_node);
  new_node->sibling = CopyTree(node->sibling, parent);
  
  return new_node;
}

template <typename ValueType>
void BinomialHeap<ValueType>::LinkTrees(Node<ValueType>* parent,
                                         Node<ValueType>* child) 
{
  child->parent = parent;
  child->sibling = parent->left_child;
  parent->left_child = child;
  parent->degree++;
}

// ==================== Public Methods ====================

template <typename ValueType>
void BinomialHeap<ValueType>::Merge(BinomialHeap& other) 
{
  if (other.IsEmpty())
  {
    return;
  }
  if (IsEmpty()) 
  {
    head_ = other.head_;
    other.head_ = nullptr;
    return;
  }

  Node<ValueType> temp;
  Node<ValueType>* tail = &temp;

  Node<ValueType>* a = head_;
  Node<ValueType>* b = other.head_;

  // Merge two root lists by degree
  while (a != nullptr && b != nullptr) 
  {
    if (a->degree <= b->degree) 
    {
      tail->sibling = a;
      a = a->sibling;
    } 
    else 
    {
      tail->sibling = b;
      b = b->sibling;
    }
    tail = tail->sibling;
  }

  tail->sibling = (a != nullptr) ? a : b;

  head_ = temp.sibling;
  other.head_ = nullptr;

  if (head_ == nullptr)
  {
    return;
  }
  // Consolidate trees with the same degree
  Node<ValueType>* prev = nullptr;
  Node<ValueType>* current = head_;
  Node<ValueType>* next = current->sibling;

  while (next != nullptr) 
  {
    if (current->degree != next->degree ||
        (next->sibling != nullptr && next->sibling->degree == current->degree)) 
    {
      prev = current;
      current = next;
    } 
    else 
    {
      if (current->key <= next->key) 
      {
        current->sibling = next->sibling;
        LinkTrees(current, next);
      } 
      else 
      {
        if (prev == nullptr) 
        {
          head_ = next;
        } 
        else 
        {
          prev->sibling = next;
        }
        LinkTrees(next, current);
        current = next;
      }
    }
    next = current->sibling;
  }
}

template <typename ValueType>
void BinomialHeap<ValueType>::Insert(ValueType key) 
{
  BinomialHeap temp(key);
  Merge(temp);
}

template <typename ValueType>
void BinomialHeap<ValueType>::DecreaseKey(Node<ValueType>* node, ValueType key) 
{
  if (key > node->key) 
  {
    throw std::runtime_error("New key is greater than current key");
  }
  
  node->key = key;
  Node<ValueType>* y = node;
  Node<ValueType>* z = y->parent;
  
  while (z != nullptr && y->key < z->key) 
  {
    std::swap(y->key, z->key);
    y = z;
    z = y->parent;
  }
}

template <typename ValueType>
void BinomialHeap<ValueType>::Delete(Node<ValueType>* node) 
{
  if (node == nullptr) return;
  DecreaseKey(node, std::numeric_limits<ValueType>::min());
  ExtractMin();
}

template <typename ValueType>
bool BinomialHeap<ValueType>::IsEmpty() const 
{
  return head_ == nullptr;
}

template <typename ValueType>
ValueType BinomialHeap<ValueType>::ExtractMin() 
{
  if (IsEmpty()) 
  {
    throw std::runtime_error("Heap is empty");
  }

  // Find minimum node and its predecessor
  Node<ValueType>* min_prev = nullptr;
  Node<ValueType>* min_node = head_;
  Node<ValueType>* prev = nullptr;
  Node<ValueType>* current = head_;

  while (current->sibling != nullptr)
  {
    if (current->sibling->key < min_node->key)
    {
      min_prev = current;
      min_node = current->sibling;
    }
    current = current->sibling;
  }

  // Remove min_node from root list
  if (min_prev == nullptr) 
  {
    head_ = min_node->sibling;
  } 
  else
  {
    min_prev->sibling = min_node->sibling;
  }

  // Create new heap from children of min_node
  BinomialHeap child_heap;
  Node<ValueType>* child = min_node->left_child;
  while (child != nullptr) 
  {
    Node<ValueType>* next = child->sibling;
    child->sibling = child_heap.head_;
    child->parent = nullptr;
    child_heap.head_ = child;
    child = next;
  }

  Merge(child_heap);

  ValueType min_key = min_node->key;
  delete min_node;
  return min_key;
}

template <typename ValueType>
Node<ValueType>* BinomialHeap<ValueType>::GetMin() const 
{
  if (IsEmpty()) return nullptr;

  Node<ValueType>* min_node = head_;
  Node<ValueType>* current = head_->sibling;
  
  while (current != nullptr) 
  {
    if (current->key < min_node->key) 
    {
      min_node = current;
    }
    current = current->sibling;
  }
  
  return min_node;
}