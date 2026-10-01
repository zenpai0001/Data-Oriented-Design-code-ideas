#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <sys/types.h>
#include <type_traits>

#include <sys/mman.h>

namespace smart_vec {

template <typename Ref_T>
concept Reference = std::is_reference_v<Ref_T>;

template <typename Cache_T>
concept CacheAligned = (alignof(Cache_T) >= 4);

template <typename T>
  requires CacheAligned<T>
struct data_span {
  void *addr = nullptr;
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  int fd = -1;
  int offset = 0;

  T *items = nullptr;
  size_t idx = 1024;
  size_t len = 0;
  void *page;

  data_span() {
    len = idx * sizeof(T);
    page = mmap(addr, len, prot, flags, fd, offset);

    if (page == MAP_FAILED) {
      page = nullptr;
      items = nullptr;
      return;
    }
    items = static_cast<T *>(page);
  }

  ~data_span() {
    if (page && page != MAP_FAILED) {
      munmap(page, len);
    }
  }

  [[nodiscard("Only you can prevent segfaults")]] constexpr std::span<T>
  as_span() const noexcept {
    return std::span<T>(items, idx);
  }
};

template <typename T>
  requires Reference<T>
struct data_ref {};

} // namespace smart_vec

class switch_board {
private:
  // Must be strictly aliigned with a 4 byte(32 bit) boundary.
  smart_vec::data_span<uint32_t> data_stream;

public:
  void initialize_span() { (void)data_stream.as_span(); }
};
