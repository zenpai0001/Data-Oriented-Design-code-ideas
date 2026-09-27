#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <sys/mman.h>
#include <sys/select.h>
#include <tuple>
#include <type_traits>

// Linux kernel headers
#include <fcntl.h>
#include <span>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace smartvecv2 {

template <typename T> struct data {
  size_t idx;
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  int fd = -1;

  void *page = mmap(nullptr, idx * sizeof(T), prot, flags, fd, 0);

  enum class state : uint8_t { RAM = 1, DISK = 2, COUNT };
  const char *state_lookup[state::COUNT]{
      static_cast<const char *>(state::RAM),
      static_cast<const char *>(state::DISK)};

  [[nodiscard]] inline T *get() const noexcept {
    return static_cast<T *>(page);
  }
  [[nodiscard]] inline T *create() const noexcept {}
};
} // namespace smartvecv2
