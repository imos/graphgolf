#ifndef GRAPHGOLF_SLICE_H_
#define GRAPHGOLF_SLICE_H_
#include <type_traits>

template<class T>
class Slice {
 public:
  Slice(T begin, T end) : begin_(begin), end_(end) {}
  ~Slice() {}

  T begin() { return begin_; }
  T end() { return end_; }

  size_t size() const { return end_ - begin_; }

  typename std::add_lvalue_reference<
      typename std::remove_pointer<T>::type>::type
      operator[](int index) const {
    DCHECK_LE(0, index);
    DCHECK_GT(size(), index);
    return *(begin_ + index);
  }

 private:
  T begin_;
  T end_;
};

#endif  // GRAPHGOLF_SLICE_H_
