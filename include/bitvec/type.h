#ifndef __BITVEC_TYPE_H__
#define __BITVEC_TYPE_H__
#include <cstddef>
#include <cstdint>
#include <fmt/core.h>
#include <vector>

namespace bv {

// configure when compile
using word_t = size_t;
using width_t = size_t;
using unit_t = uint8_t;

#define ubits (sizeof(unit_t) * 8)
#define BASE_SIZE (sizeof(word_t) / sizeof(unit_t))
#define HALF_OF(type) (((type)1L) << ((sizeof(type) * 8) - 1))

template <typename T> class MyVector : public std::vector<T> {
public:
  MyVector &operator=(const MyVector &that) {
    fmt::print("call MyVector assign const&\n");
    std::vector<T>::operator=(that);
    return *this;
  }
  MyVector &operator=(MyVector &&that) noexcept {
    fmt::print("call MyVector assign &&\n");
    std::vector<T>::operator=(std::move(that));
    return *this;
  };
  MyVector(const MyVector &that) : std::vector<T>(that) {
    fmt::print("call MyVector copy construct of const&\n");
  }
  MyVector(MyVector &&that) noexcept : std::vector<T>(std::move(that)) {
    fmt::print("call MyVector copy construct of &&\n");
  };
  MyVector() : std::vector<T>() {
    fmt::print("call MyVector empty construct\n");
  }
  MyVector(size_t n) : std::vector<T>(n) {
    fmt::print("call MyVector args construct with {}\n", n);
  }
};
using stdvec = MyVector<unit_t>;

}; // namespace bv

#endif // __BITVEC_TYPE_H__