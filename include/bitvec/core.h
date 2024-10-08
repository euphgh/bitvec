#ifndef __BITVEC_CORE_H__
#define __BITVEC_CORE_H__

#include "bitvec/debug.h"
#include "bitvec/type.h"
#include "bitvec/utils.h"
#include "fmt/ranges.h"
#include <functional>
#include <string>

namespace bv {

// copy from bitset reference
class BitRef;
class BitVec {
  stdvec data = stdvec();
  width_t len = 0;
  // remove redundent element
  void data_resize() {
    const auto new_size = ((width() - 1) / ubits) + 1;
    BV_INFO("call data_resize {}", new_size);
    data.resize(new_size);
  }
  BitVec(width_t width, stdvec mem) : data(std::move(mem)), len(width) {}
  [[nodiscard]] inline width_t data_size() const { return data.size(); }

public:
  [[nodiscard]] std::string data_check() const {
    return fmt::format("{:02x}", fmt::join(data, ", "));
  }
  [[nodiscard]] std::string to_string(const std::string &format = "h") const;
  // minimal bits for this value
  // BitVec(word_t value, width_t width = 0);
  // BitVec(const std::string &value, width_t width = 0);
  // if width == 0, width will be minimal of value, if value == 0, width = 1
  BitVec(word_t value = 0, width_t width = 0);
  BitVec(const std::string &value, width_t width = 0);
  static BitVec random(width_t width) {
    auto ret = BitVec(0, width);
    for (size_t i = 0; i < ret.data_size() - 1; i++) {
      ret.data[i] = rand_unit();
    }
    ret.data[ret.data_size() - 1] = rand_unit() & low_bit_mask(width % ubits);
    return ret;
  };
  // interface for BitRef constructor
  inline unit_t &ref(width_t pos) { return data[pos / ubits]; }
  [[nodiscard]] BitRef operator[](width_t pos);
  [[nodiscard]] bool operator[](width_t pos) const;
  [[nodiscard]] bool operator==(const BitVec &rhs) const;
  [[nodiscard]] bool operator!=(const BitVec &rhs) const;
  [[nodiscard]] bool operator>(const BitVec &rhs) const;
  [[nodiscard]] bool operator<(const BitVec &rhs) const;
  [[nodiscard]] bool operator>=(const BitVec &rhs) const;
  [[nodiscard]] bool operator<=(const BitVec &rhs) const;
  // copy and new, data will be copy
  [[nodiscard]] BitVec operator<<(width_t pos) const;
  [[nodiscard]] BitVec operator>>(width_t pos) const;
  [[nodiscard]] BitVec operator&(const BitVec &rhs) const;
  [[nodiscard]] BitVec operator|(const BitVec &rhs) const;
  [[nodiscard]] BitVec operator^(const BitVec &rhs) const;
  [[nodiscard]] BitVec slice(width_t msb, width_t lsb) const;
  [[nodiscard]] inline width_t width() const {
    require(len > 0, "len == 0");
    return len;
  }
  // change
  void sign_extend(width_t start);
  void zero_extend(width_t start);
  void resize(width_t width);

  // set from zero bits, fast version of bitvec[pos] = true;
  void set(width_t pos, bool value = true);
  void clear(width_t pos) { set(pos, false); }
  void set(void *value, width_t bits);
  void get(void *value, width_t bits) const;
  // from zero bit to max(64, width())
  [[nodiscard]] size_t get() const;
};

class BitRef {
  unit_t &data;
  size_t pos;

public:
  BitRef() = delete;
  BitRef(BitVec &bv, size_t bv_pos)
      : data(bv.ref(bv_pos)), pos(bv_pos % ubits) {}
  BitRef(const BitRef &that) = delete;
  BitRef(BitRef &&that) = delete;

  // For b[i] = __x;
  BitRef &operator=(bool value) {
    if (value) {
      data |= 1 << pos;
    } else {
      data &= ~(1 << pos);
    }
    return *this;
  }

  // For b[i] = b[__j];
  BitRef &operator=(const BitRef &that) {
    *this = bool(that);
    return *this;
  }

  BitRef &operator=(BitRef &&that) noexcept {
    *this = bool(that);
    return *this;
  };

  // Flips the bit
  bool operator~() const { return !bool(*this); }
  bool operator!() const { return !bool(*this); }

  // For __x = b[i];
  operator bool() const { return data >> (pos) & 0x1; }

  // For b[i].flip();
  BitRef &flip() {
    *this = ~(*this);
    return *this;
  }
};

struct Direct {
  bool is_output;
  [[nodiscard]] Direct flipped() const { return {!is_output}; }
  static Direct input() { return {false}; }
  static Direct output() { return {true}; };

private:
  Direct(bool output) : is_output(output) {}
};

struct Signal {
  std::string name;
  Direct direct;
  BitVec data;
  Signal(const std::string &name, Direct direct, size_t width);
  [[nodiscard]] bool prsent() const { return data.width() > 0; }
};

struct DecoupledIO {
  Signal valid;
  Signal ready;

  DecoupledIO(Direct direct)
      : valid("valid", direct, 1), ready("ready", direct.flipped(), 1) {}
  DecoupledIO() : DecoupledIO(Direct::output()) {}
  [[nodiscard]] bool is_master() const { return valid.direct.is_output; }
  virtual void foreach (const std::function<void(Signal)> &cb) const {
    cb(valid);
    cb(ready);
  }
};
};     // namespace bv
#endif // __BITVEC_CORE_H__