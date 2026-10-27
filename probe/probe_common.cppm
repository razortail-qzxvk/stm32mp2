module;
#include <cstddef>
#include <cstdint>
#include <bit>
export module probe:common;

/**
 * 作为一个MCU运作的探针程序. 我们首先要为每一种外设描绘出他们需要的三样关键东西: 电源域, 时钟门控和RIF上下文.
 *
 * 对于GPIO:
 *   对于电源域:
 *     文档 an5489-getting-started-with-stm32mp2325xx-mpus-hardware-development-stmicroelectronics.pdf
 *     给出了每一个引脚的所在电源域.
 *     大多数引脚都在VDD电源域, 这个不需要探测, 上电就给电. 其余内容也可以轻松总结得出:
 *     PE[5:0]  -> VDDIO1
 *     PE[15:6] -> VDDIO2
 *     PD[11:0] -> VDDIO3
 *     PB[11:0] -> VDDIO4
 *     PZ[6:0] PC13 PI8 -> VSW
 *     PC[5:3] PF[7:6] PG1 PG3 -> VSW // 注: 若不需要额外功能(TAMP_INx additional function), 则无需VSW供电
 *     可以看出我们只需处理 VDDIO 和 VSW.
 *     VDDIO 是5个部分组成上下文 VMEN RDY SV VRSEL VRSTBY
 */

template <std::size_t Msb, std::size_t Lsb>
constexpr std::uint32_t field(std::uint32_t reg) {
  static_assert(Msb >= Lsb && Msb < 32);
  constexpr std::size_t width = Msb - Lsb + 1;
  constexpr std::uint32_t mask = static_cast<std::uint32_t>(((1ULL << width) - 1) << Lsb); // 1ULL 以避开 width == 32 时的 32 位移位未定义行为
  return (reg & mask) >> Lsb;
}
constexpr std::uint32_t field(std::uint32_t reg, std::uint32_t mask) {
  if (mask == 0) return 0;
  return (reg & mask) >> std::countr_zero(mask);
}

export class probe_result_GPIO {
  /**
   * 电源域
   * STM32MP2-GPIO的电源域
   */

  // 时钟门控

  // RIF上下文
}
