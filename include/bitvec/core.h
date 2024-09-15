#ifndef __BITVEC_CORE_H__
#define __BITVEC_CORE_H__

#include "bitvec/debug.h"
#include "bitvec/type.h"
#include "fmt/ranges.h"
#include <functional>
#include <string>

namespace bv {

class BitVec {
  stdvec data = stdvec();
  width_t len = 0;
  // remove redundent element
  void data_resize() { data.resize((width() - 1) / (sizeof(unit_t))); }
  BitVec(width_t width, stdvec mem) : data(std::move(mem)), len(width) {}
  [[nodiscard]] inline width_t data_size() const { return data.size(); }
  [[nodiscard]] std::string data_check() const {
    return fmt::format("{:02x}", fmt::join(data, ", "));
  }

public:
  [[nodiscard]] std::string to_string(const std::string &format = "h") const;
  // minimal bits for this value
  // BitVec(word_t value, width_t width = 0);
  // BitVec(const std::string &value, width_t width = 0);
  // if width == 0, width will be minimal of value, if value == 0, width = 1
  BitVec(word_t value = 0, width_t width = 0);
  BitVec(const std::string &value, width_t width = 0);
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
  [[nodiscard]] BitVec slice(width_t msb, width_t lsb) const;
  [[nodiscard]] inline width_t width() const { return len; }
  // change
  void sign_extend(width_t start);
  void zero_extend(width_t start);
  void resize(width_t width);

  // set from zero bits
  void set(width_t pos, bool value = true);
  void set(void *value, width_t bits);
  void get(void *value, width_t bits) const;
  // from zero bit to max(64, width())
  [[nodiscard]] uint64_t get() const;
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