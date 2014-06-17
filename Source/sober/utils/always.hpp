#ifndef SOBER_UTILS_ALWAYS_HPP_
#define SOBER_UTILS_ALWAYS_HPP_

namespace sober {
namespace utils {

/*
 * @brief Function to suppress `conditional expression is constant` warning
 */
inline bool always() {
  return true;
}

} // namespace utils
} // namespace sober

#endif // SOBER_UTILS_ALWAYS_HPP_
