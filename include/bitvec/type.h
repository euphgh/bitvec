#ifndef __BITVEC_TYPE_H__
#define __BITVEC_TYPE_H__
#include <cstddef>
#include <cstdint>
#include <vector>

#ifndef word_t
using word_t = size_t;
#endif // word_t
#ifndef width_t
using width_t = size_t;
#endif // width_t
#ifndef unit_t
using unit_t = uint8_t;
#endif // unit_t

#define ubits (sizeof(unit_t) * 8)
#define BASE_SIZE (sizeof(word_t) / sizeof(unit_t))
#define HALF_OF(type) (((type)1L) << ((sizeof(type) * 8) - 1))


#ifndef stdvec
#include <fmt/core.h>
template <typename T> class MyVector : public std::vector<T> {
public:
  MyVector &operator=(const MyVector &that) {
    fmt::println("call MyVector assign const&");
    std::vector<T>::operator=(that);
    return *this;
  }
  MyVector &operator=(MyVector &&that) noexcept {
    fmt::println("call MyVector assign &&");
    std::vector<T>::operator=(std::move(that));
    return *this;
  };
  MyVector(const MyVector &that) : std::vector<T>(that) {
    fmt::println("call MyVector copy construct of const&");
  }
  MyVector(MyVector &&that) noexcept : std::vector<T>(std::move(that)) {
    fmt::println("call MyVector copy construct of &&");
  };
  MyVector() : std::vector<T>() {
    fmt::println("call MyVector empty construct");
  }
  MyVector(size_t n) : std::vector<T>(n) {
    fmt::println("call MyVector args construct with {}", n);
  }
};
using stdvec = MyVector<unit_t>;
#endif

#endif // __BITVEC_TYPE_H__