#pragma once
//from one of the previous labs almost unchanged
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

const int kMaxVectorSize = 100000000;
const int kMaxMatrixSize = 1000000;

template <typename ValType>
class TVector {
 public:
  TVector(int size = 10, int start_index = 0);
  TVector(const TVector& other);
  ~TVector();

  int GetSize() const { return size_; }
  int GetStartIndex() const { return start_index_; }

  ValType& operator[](int pos);
  const ValType& operator[](int pos) const;

  bool operator==(const TVector& other) const;
  bool operator!=(const TVector& other) const;
  TVector& operator=(const TVector& other);

  template <typename T>
  friend std::istream& operator>>(std::istream& in, TVector<T>& v);

  template <typename T>
  friend std::ostream& operator<<(std::ostream& out, const TVector<T>& v);

 private:
  int size_;
  int start_index_;
  ValType* data_;
};

template <typename ValType>
class TMatrix : public TVector<TVector<ValType>> {
 public:
  explicit TMatrix(int size = 10);
  TMatrix(const TMatrix& other);
  explicit TMatrix(const TVector<TVector<ValType>>& other);

  bool operator==(const TMatrix& other) const;
  bool operator!=(const TMatrix& other) const;
  TMatrix& operator=(const TMatrix& other);

  template <typename T>
  friend std::istream& operator>>(std::istream& in, TMatrix<T>& mt);

  template <typename T>
  friend std::ostream& operator<<(std::ostream& out, const TMatrix<T>& mt);
};

template <typename ValType>
TVector<ValType>::TVector(int size, int start_index)
    : size_(size), start_index_(start_index), data_(nullptr) {
  if (size > kMaxVectorSize || size < 0 || start_index > kMaxVectorSize ||
      start_index < 0 || size < start_index) {
    throw std::out_of_range("Incorrect vector size or start index");
  }
  data_ = new ValType[size - start_index]();
}

template <typename ValType>
TVector<ValType>::TVector(const TVector& other)
    : size_(other.size_),
      start_index_(other.start_index_),
      data_(new ValType[other.size_ - other.start_index_]) {
  for (int i = 0; i < size_ - start_index_; ++i) {
    data_[i] = other.data_[i];
  }
}

template <typename ValType>
TVector<ValType>::~TVector() {
  delete[] data_;
  data_ = nullptr;
}

template <typename ValType>
ValType& TVector<ValType>::operator[](int pos) {
  if (pos < start_index_ || pos >= size_) {
    throw std::out_of_range("Index is out of range");
  }
  return data_[pos - start_index_];
}

template <typename ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
  if (pos < start_index_ || pos >= size_) {
    throw std::out_of_range("Index is out of range");
  }
  return data_[pos - start_index_];
}

template <typename ValType>
bool TVector<ValType>::operator==(const TVector& other) const {
  if (size_ != other.size_ || start_index_ != other.start_index_) {
    return false;
  }
  for (int i = 0; i < size_ - start_index_; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }
  return true;
}

template <typename ValType>
bool TVector<ValType>::operator!=(const TVector& other) const {
  return !(*this == other);
}

template <typename ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& other) {
  if (this == &other) {
    return *this;
  }
  TVector tmp(other);
  std::swap(size_, tmp.size_);
  std::swap(start_index_, tmp.start_index_);
  std::swap(data_, tmp.data_);
  return *this;
}

template <typename ValType>
std::istream& operator>>(std::istream& in, TVector<ValType>& v) {
  for (int i = 0; i < v.size_ - v.start_index_; ++i) {
    in >> v.data_[i];
  }
  return in;
}

template <typename ValType>
std::ostream& operator<<(std::ostream& out, const TVector<ValType>& v) {
  for (int i = 0; i < v.start_index_; ++i) {
    out << "0 ";
  }
  for (int i = 0; i < v.size_ - v.start_index_; ++i) {
    out << v.data_[i] << " ";
  }
  return out;
}

template <typename ValType>
TMatrix<ValType>::TMatrix(int size)
    : TVector<TVector<ValType>>(size, 0) {
  if (size < 0) {
    throw std::out_of_range("Size must be positive");
  }
  if (size > kMaxMatrixSize) {
    throw std::out_of_range("Incorrect matrix size provided");
  }

  for (int i = 0; i < size; ++i) {
    TVector<ValType> row(size, 0);
    for (int j = 0; j < size; ++j) {
      row[j] = ValType();
    }
    (*this)[i] = row;
  }
}

template <typename ValType>
TMatrix<ValType>::TMatrix(const TMatrix& other)
    : TVector<TVector<ValType>>(other) {}

template <typename ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& other)
    : TVector<TVector<ValType>>(other) {}

template <typename ValType>
bool TMatrix<ValType>::operator==(const TMatrix& other) const {
  return TVector<TVector<ValType>>::operator==(other);
}

template <typename ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& other) const {
  return !(*this == other);
}

template <typename ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix& other) {
  TVector<TVector<ValType>>::operator=(other);
  return *this;
}

template <typename ValType>
std::istream& operator>>(std::istream& in, TMatrix<ValType>& mt) {
  for (int i = 0; i < mt.GetSize(); ++i) {
    in >> mt[i];
  }
  return in;
}

template <typename ValType>
std::ostream& operator<<(std::ostream& out, const TMatrix<ValType>& mt) {
  for (int i = 0; i < mt.GetSize(); ++i) {
    out << mt[i] << "\n";
  }
  return out;
}