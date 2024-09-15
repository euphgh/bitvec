#ifndef __BITVEC_DEBUG_H__
#define __BITVEC_DEBUG_H__

#define BV_LOG_LEVEL 0
#define BV_LOG_INFO 1
#define BV_LOG_WARN 2
#define BV_LOG_ERROR 3

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

#define BV_LOG(cond, fmt_str, ...)                                             \
  do {                                                                         \
    if (BV_LOG_LEVEL >= cond)                                                  \
      fmt::println(fmt_str, ##__VA_ARGS__);                                    \
  } while (0)

#define BV_INFO(fmt_str, ...) BV_LOG(BV_LOG_INFO, fmt_str, __VA_ARGS__)
#define BV_WARN(fmt_str, ...) BV_LOG(BV_LOG_WARN, fmt_str, __VA_ARGS__)
#define BV_ERROR(fmt_str, ...) BV_LOG(BV_LOG_ERROR, fmt_str, __VA_ARGS__)

};     // namespace bv
#endif // __BITVEC_DEBUG_H__