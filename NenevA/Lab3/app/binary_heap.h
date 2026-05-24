#pragma once
#include <iostream>
#include <vector>
template <typename ValType>
class BinaryHeap {
public:
  BinaryHeap();
  void Insert(const ValType& element);
  void Print();
  ValType ExtractMin();
  void SiftUp(int i);
  void SiftDown(int i);
  bool IsEmpty();
private:
  size_t GetParent(size_t element);
  size_t GetLeftChild(size_t element);
  size_t GetRightChild(size_t element);
  std::vector<ValType> data_;
};

template <typename ValType>
size_t BinaryHeap<ValType>::GetParent(size_t element) {
  return (element - 1) / 2;
}

template <typename ValType>
size_t BinaryHeap<ValType>::GetLeftChild(size_t element) {
  return 2 * element + 1;
}

template <typename ValType>
size_t BinaryHeap<ValType>::GetRightChild(size_t element) {
  return 2 * element + 2;
}

template <typename ValType>
BinaryHeap<ValType>::BinaryHeap() {}

template <typename ValType>
void BinaryHeap<ValType>::Insert(const ValType& element) {
  data_.push_back(element);
  SiftUp(data_.size() - 1);
}

template <typename ValType>
void BinaryHeap<ValType>::SiftUp(int i) {
  while (i > 0 && data_[GetParent(i)] > data_[i]) {
    std::swap(data_[i], data_[GetParent(i)]);
    i = GetParent(i);
  }
}

template <typename ValType>
void BinaryHeap<ValType>::SiftDown(int i) {
  size_t smallest = i;
  size_t size = data_.size();
  size_t left = GetLeftChild(i);
  size_t right = GetRightChild(i);

  if (left < size && data_[left] < data_[smallest]) {
    smallest = left;
  }
  if (right < size && data_[right] < data_[smallest]) {
    smallest = right;
  }

  if (smallest != i) {
    std::swap(data_[i], data_[smallest]);
    SiftDown(smallest);
  }
}

template <typename ValType>
ValType BinaryHeap<ValType>::ExtractMin() {
  if (data_.empty()) throw std::out_of_range("The heap is empty");

  ValType min = data_[0];
  data_[0] = data_.back();
  data_.pop_back();
  if (!data_.empty()) {
    SiftDown(0);
  }
  return min;
}

template <typename ValType>
void BinaryHeap<ValType>::Print() {
  for (auto& element : data_) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}

template <typename ValType>
bool BinaryHeap<ValType>::IsEmpty() {
  return data_.empty();
}
