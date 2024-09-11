#include "bitvec/debug.h"
#include "bitvec/core.h"
#include "bitvec/type.h"
#include <cstddef>
#include <fmt/core.h>
#include <sstream>
#include <stack>
#include <string>

void error_impl(const char *msg) {
  std::fputs(msg, stderr);
  std::putc('\n', stderr);
  std::fflush(stderr);
  exit(-1);
}

namespace {

char hex_char(size_t data) {
  if (data < 10) {
    auto res = (data & 0xf) + '0';
    return *(char *)&res;
  }
  if (data < 16) {
    auto res = ((data - 10) & 0xf) + 'a';
    return *(char *)&res;
  }
  require(false, "call hex_char with {}", data);
}

#define bin_char(bit) ((char)(((bit) & 0x1) + '0'))

using mark_t = enum { hex, bin, otx };

std::string byte_string(uint8_t data, mark_t mark) {
  std::string res;
  switch (mark) {
  case hex:
    return fmt::format("{:02x}", data);
  case bin:
    return fmt::format("{:08b}", data);
  default:
    require(false, "not expect exec to here");
  }
}

// from 0 to len - 1
std::string bits_string(uint8_t data, width_t len, mark_t mark) {
  auto res = std::stringstream();
  if (mark == hex) {
    auto buf = std::stack<uint8_t>();
    for (size_t i = 0; i < len / 4; i++) {
      buf.push((data & 0xf));
      data >>= 4;
    }
    if (len % 4) {
      buf.push(data);
      res << "(";
      for (size_t i = len % 4; i < 4; i++) {
        res << "x";
      }
      for (size_t i = len % 4 - 1; i >= 0; i--) {
        res << (char)(((data >> i) & 0x1) + '0');
      }
      res << ")";
    }

    while (!buf.empty()) {
      res << hex_char(buf.top());
      buf.pop();
    }
    return res.str();
  }
  if (mark == bin) {
    for (size_t i = len - 1; i >= 0; i--) {
      res << bin_char(data >> i);
    }
    return res.str();
  }
  require(false, "fail to bits_string with not support format");
}

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

}; // namespace

std::string BitVec::to_string(const std::string &format) const {
  require(format.length() > 0, " format is empty");
  auto res = std::stringstream();
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