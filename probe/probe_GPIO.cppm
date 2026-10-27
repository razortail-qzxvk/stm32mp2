/**
 * 待办列表:
 * - 并不是所有的GPIO的电源域都是 PWR->CR1 和 VDDIO3, 这个地方需要修改.
 * - 考虑给出 csv / json-lines / hex帧, 而不是英文字符串, 因为MCU并不擅长渲染, 渲染应该是linux的python脚本做的事情.
 */

module;
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <array>

#include "main.h"
export module probe:GPIO;
import :common;

namespace probe {

void do_GPIO_common(const char* pwr_name, const char* rcc_name, const char* obj_name, GPIO_TypeDef* GPIOx, std::uint32_t PWRREG, std::uint32_t RCCREG) {
  // clang-format off
  // 电源域
  std::printf("[Probe][%s]\n", pwr_name);
  std::printf("  0x%08X | VDDIO3: VMEN=%u SV=%u RDY=%u VRSEL=%u\n",
    PWRREG,
    field(PWRREG, PWR_CR1_VDDIO3VMEN_Msk),
    field(PWRREG, PWR_CR1_VDDIO3SV_Msk),
    field(PWRREG, PWR_CR1_VDDIO3RDY_Msk),
    field(PWRREG, PWR_CR1_VDDIO3VRSEL_Msk));
  // 时钟门控
  std::printf("[Probe][%s]\n", rcc_name);
  std::printf("  0x%08X | EN=%u LPEN=%u RST=%u\n",
    RCCREG,
    field(RCCREG, RCC_GPIOACFGR_GPIOxEN_Msk),
    field(RCCREG, RCC_GPIOACFGR_GPIOxLPEN_Msk),
    field(RCCREG, RCC_GPIOACFGR_GPIOxRST_Msk));
  // clang-format on

  std::printf("[Probe][%s]\n", obj_name);
  std::array<std::uint32_t, 16> CIDCFGR_list = {{GPIOx->CIDCFGR0, GPIOx->CIDCFGR1, GPIOx->CIDCFGR2, GPIOx->CIDCFGR3, GPIOx->CIDCFGR4, GPIOx->CIDCFGR5, GPIOx->CIDCFGR6, GPIOx->CIDCFGR7, GPIOx->CIDCFGR8, GPIOx->CIDCFGR9, GPIOx->CIDCFGR10, GPIOx->CIDCFGR11, GPIOx->CIDCFGR12, GPIOx->CIDCFGR13, GPIOx->CIDCFGR14, GPIOx->CIDCFGR15}};
  std::array<std::uint32_t, 16> SEMCR_list = {{GPIOx->SEMCR0, GPIOx->SEMCR1, GPIOx->SEMCR2, GPIOx->SEMCR3, GPIOx->SEMCR4, GPIOx->SEMCR5, GPIOx->SEMCR6, GPIOx->SEMCR7, GPIOx->SEMCR8, GPIOx->SEMCR9, GPIOx->SEMCR10, GPIOx->SEMCR11, GPIOx->SEMCR12, GPIOx->SEMCR13, GPIOx->SEMCR14, GPIOx->SEMCR15}};
  for (auto pin = 0U; pin < 16U; ++pin) {
    std::printf("  Pin%2u Mode:%03u ODR:%1u IDR:%1u\n", pin, (GPIOx->MODER >> (pin * 2U)) & 3U, (GPIOx->ODR >> pin) & 1U, (GPIOx->IDR >> pin) & 1U);
    std::printf("    Security:%1u\n", (GPIOx->SECCFGR >> pin) & 1U);
    std::printf("    Privilege:%1u\n", (GPIOx->PRIVCFGR >> pin) & 1U);
    std::printf("    Resource Configuration Lock:%1u\n", (GPIOx->RCFGLOCKR >> pin) & 1U);
    std::printf("    CID Filtering Enable:%1u\n", field(CIDCFGR_list[pin], GPIO_CIDCFGR0_CFEN_Msk));
    std::printf("    Semaphore Enable:%1u\n", field(CIDCFGR_list[pin], GPIO_CIDCFGR0_SEM_EN_Msk));
    std::printf("    Static Compartment ID:%3u\n", field(CIDCFGR_list[pin], GPIO_CIDCFGR0_SCID_Msk));
    std::printf("    Semaphore whitelist for compartment:%4u\n", field(CIDCFGR_list[pin], GPIO_CIDCFGR0_SEMWLC0_Msk | GPIO_CIDCFGR0_SEMWLC1_Msk | GPIO_CIDCFGR0_SEMWLC2_Msk | GPIO_CIDCFGR0_SEMWLC3_Msk));
    std::printf("    Semaphore Control Register: 0x%08X\n", SEMCR_list[pin]);
  }
}

export void do_GPIOA() { do_GPIO_common("PWR_CR1", "RCC_GPIOACFGR", "GPIOA", GPIOA, PWR->CR1, RCC->GPIOACFGR); }
export void do_GPIOB() { do_GPIO_common("PWR_CR1", "RCC_GPIOBCFGR", "GPIOB", GPIOB, PWR->CR1, RCC->GPIOBCFGR); }
export void do_GPIOC() { do_GPIO_common("PWR_CR1", "RCC_GPIOCCFGR", "GPIOc", GPIOC, PWR->CR1, RCC->GPIOCCFGR); }
export void do_GPIOD() { do_GPIO_common("PWR_CR1", "RCC_GPIODCFGR", "GPIOD", GPIOD, PWR->CR1, RCC->GPIODCFGR); }
export void do_GPIOE() { do_GPIO_common("PWR_CR1", "RCC_GPIOECFGR", "GPIOE", GPIOE, PWR->CR1, RCC->GPIOECFGR); }
export void do_GPIOF() { do_GPIO_common("PWR_CR1", "RCC_GPIOFCFGR", "GPIOF", GPIOF, PWR->CR1, RCC->GPIOFCFGR); }
export void do_GPIOG() { do_GPIO_common("PWR_CR1", "RCC_GPIOGCFGR", "GPIOG", GPIOG, PWR->CR1, RCC->GPIOGCFGR); }
export void do_GPIOH() { do_GPIO_common("PWR_CR1", "RCC_GPIOHCFGR", "GPIOH", GPIOH, PWR->CR1, RCC->GPIOHCFGR); }
export void do_GPIOI() { do_GPIO_common("PWR_CR1", "RCC_GPIOICFGR", "GPIOI", GPIOI, PWR->CR1, RCC->GPIOICFGR); }
export void do_GPIOJ() { do_GPIO_common("PWR_CR1", "RCC_GPIOJCFGR", "GPIOJ", GPIOJ, PWR->CR1, RCC->GPIOJCFGR); }
export void do_GPIOK() { do_GPIO_common("PWR_CR1", "RCC_GPIOKCFGR", "GPIOK", GPIOK, PWR->CR1, RCC->GPIOKCFGR); }
} // namespace probe
