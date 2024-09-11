#ifndef __BITVEC_DEBUG_H__
#define __BITVEC_DEBUG_H__

namespace bv {

// impliment in cc file, print and exit(-1)
[[noreturn]] void error_impl(const char *msg);

#ifdef RELEASE
#define require(cond, fmt_str, ...)
#else
#define require(cond, fmt_str, ...)                                            \
  do {                                                                         \
    if (!(cond))                                                               \
      error_impl(fmt::format(fmt_str, ##__VA_ARGS__).c_str());                 \
  } while (0)
#endif
};     // namespace bv
#endif // __BITVEC_DEBUG_H__