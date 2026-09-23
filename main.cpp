#include <any>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory_resource>
#include <sys/select.h>
#include <sys/types.h>
#include <type_traits>
#include <vector>

// Linux/POSIX system headers
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace smartvec {
enum class state_machine {
  IDLE = 0x01,
  INCREASE = 0x02,
  DECREASE = 0x03,
  DESTROY = 0x04,
  RECEIVE = 0x05,
  ALLOCATE = 0x0F,
};

template <typename T> struct data {
  using s32 = int32_t;

  // A chunk of memory called through the linux kernel.
  void *map_chunk =
      mmap(nullptr, data_array.size(), PROT_READ, MAP_PRIVATE, 10, 0);

  static std::vector<T, std::pmr::polymorphic_allocator<T>>
  data_array(size_t idx) {
    void *ctx;
    void callback(void *ctx);
    bool trigger;
    return state_machine::ALLOCATE;
  }

  static std::vector<s32, std::pmr::polymorphic_allocator<T>> codes(s32 idx) {
    s32 status_code = 0;
  }

  T &operator()(size_t idx) {
    state_machine state;

    for (idx = 0; idx < data_array.size(); ++idx) {
      if (data_array[idx] != nullptr) {
        return data_array[idx];
      } else {
        return data_array[idx] = nullptr;
      }
      switch (state) {
      case state_machine::IDLE:
        return data_array[idx];
      case state_machine::INCREASE:
        return reserve(data_array.size() + 1);
      case state_machine ::DECREASE:
        return data_array[idx].popback();
      case state_machine::DESTROY:
        // TODO: Destroy map_chunk in indices as well.
        return data_array[idx].clear() ?: data_array[idx] = nullptr;
      case state_machine::RECEIVE:
        return &data_array[idx];
      case state_machine::ALLOCATE:
        return map_chunk;
      }
    }
  }
  s32 &operator()(s32 idx) {
    return codes[idx].status_code =
               0
                   ?: static_cast<T>(state_machine::IDLE) ||
                          static_cast<T>(state_machine::RECEIVE);
  }
};
} // namespace smartvec
// An experimental class with no namespace.
class scope {
  smartvec::data<void *> ptr_array;

  void swtich_board() { ptr_array.map_chunk; }
};

namespace container {
// Code Skeleton
}

int main() {
  std::cout << "main.cpp created";
  return 0;
}
