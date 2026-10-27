#include <atomic>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <bit>

#define RSC_TABLE_SECTION [[gnu::used, gnu::section(".resource_table"), gnu::aligned(4)]]
#define RSC_TRACE_SECTION [[gnu::used, gnu::section(".resource_table.trace"), gnu::aligned(16)]]

// 日志缓存
constexpr std::uint32_t TRACE_BUFFER_SIZE = 4096;
static_assert(std::has_single_bit(TRACE_BUFFER_SIZE));
RSC_TRACE_SECTION static char trace_buffer[TRACE_BUFFER_SIZE]; // OP-TEE 固件校验/加密的原子块大小是 16 字节

// Linux Remoteproc 标准的 Trace 资源表结构
constexpr std::uint32_t RSC_TRACE = 2;
struct fw_rsc_trace {
  std::uint32_t type;
  std::uint32_t da;
  std::uint32_t len;
  std::uint32_t reserved;
  std::uint8_t name[32];
};
struct m33_resource_table {
  std::uint32_t version;
  std::uint32_t num;
  std::uint32_t reserved[2];
  std::uint32_t offset[1];
  fw_rsc_trace trace_entry;
};
static_assert(sizeof(m33_resource_table) == 68);
static_assert(offsetof(m33_resource_table, trace_entry) == 20);

extern "C" {
// clang-format off
RSC_TABLE_SECTION extern const m33_resource_table rsc_table =
{
  .version = 1,
  .num = 1,
  .reserved = {0, 0},
  .offset = {offsetof(m33_resource_table, trace_entry)},
  .trace_entry =
  {
    .type = RSC_TRACE,
    .da = reinterpret_cast<uintptr_t>(trace_buffer),
    .len = TRACE_BUFFER_SIZE,
    .reserved = 0,
    .name = "cm33_trace"
  }
};
// clang-format on

struct __llvm_libc_stdio_cookie {
  void* dummy = nullptr;
} __llvm_libc_stdin_cookie, __llvm_libc_stdout_cookie, __llvm_libc_stderr_cookie;

static std::uint32_t write_idx = 0;
int __llvm_libc_stdio_write([[maybe_unused]] void* cookie, const char* buf, std::size_t size) {
  if (buf == nullptr) return -1;
  if (!std::in_range<int>(size)) return -1;

  for (auto i = 0uz; i < size; ++i) {
    trace_buffer[write_idx] = buf[i];
    write_idx = (write_idx + 1) & (TRACE_BUFFER_SIZE - 1);
  }
  trace_buffer[write_idx] = '\0';

  std::atomic_thread_fence(std::memory_order_release);
  return static_cast<int>(size);
}

int __llvm_libc_stdio_read([[maybe_unused]] void* cookie, [[maybe_unused]] char* buf, [[maybe_unused]] size_t size) { return 0; }
}
