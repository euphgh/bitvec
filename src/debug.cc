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

std::string unit_string(unit_t data, mark_t mark) {
  std::string res;
  switch (mark) {
  case hex:
    res = fmt::format("{:h}", data);
  case bin:
    res = fmt::format("{:b}", data);
  case otx:
    res = fmt::format("{:l}", data);
  }
  return res;
}

// from 0 to len - 1
std::string bits_string(unit_t data, width_t len, mark_t mark) {
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

std::string BitVec::to_string(const std::string &format) const {
  require(format.length() > 0, " format is empty");
  auto res = std::stringstream();
  switch (format[0]) {
  case 'h': {
    res << bits_string(data[data_size() - 1], width() % ubits, hex);
    if (data_size() > 1) {
      for (size_t i = data_size() - 2; i >= 0; i--) {
        res << unit_string(data[i], hex);
      }
    }
    return res.str();
  }
  case 'b': {
    res << bits_string(data[data_size() - 1], width() % ubits, bin);
    if (data_size() > 1) {
      for (size_t i = data_size() - 2; i >= 0; i--) {
        res << unit_string(data[i], bin);
      }
    }
    return res.str();
  }
  case 'o': {
    return std::to_string(get());
  }
  default:
    return "";
  }
}