#include "bitvec/debug.h"
#include "bitvec/core.h"
#include "bitvec/utils.h"

void bv::error_impl(const char *msg) {
  std::fputs(msg, stderr);
  std::putc('\n', stderr);
  std::fflush(stderr);
  exit(-1);
}

using namespace bv;
// TODO: not support big end
std::string to_string_impl(uint8_t *arr, width_t width, mark_t mark) {
  auto res = std::stringstream();
  auto nbytes = width / 8;
  if (width % 8) {
    res << bits_string(arr[nbytes], width % 8, mark);
  }
  for (size_t i = 1; i <= nbytes; i++) {
    res << byte_string(arr[nbytes - i], mark);
  }
  return res.str();
}

// format string parse and call impl
std::string BitVec::to_string(const std::string &format) const {
  require(format.length() > 0, " format is empty");
  switch (format[0]) {
  case 'h': {
    return to_string_impl((uint8_t *)data.data(), width(), hex);
  }
  case 'b': {
    return to_string_impl((uint8_t *)data.data(), width(), bin);
  }
  case 'o': {
    return std::to_string(get());
  }
  default:
    return "";
  }
}