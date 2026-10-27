#include <cstdint>
#include <cstdio>

#include "adc.h"
#include "i2c.h"
#include "ipcc.h"
#include "rtc.h"
#include "usart.h"
#include "ucpd.h"
#include "gpio.h"

extern "C" {
#include "copro_sync.h"
}

extern "C" void SystemClock_Config(void);

namespace {

void ProbeRccResource(const char* name, std::uint32_t res_id, volatile std::uint32_t& cfgr_reg) {
  const std::uint32_t reg_idx = res_id / 32U;
  const std::uint32_t bit_pos = res_id % 32U;
  const bool is_sec = ((RCC->SECCFGR[reg_idx] >> bit_pos) & 1U) != 0U;
  const bool is_priv = ((RCC->PRIVCFGR[reg_idx] >> bit_pos) & 1U) != 0U;
  const std::uint32_t cidcfgr = RCC->R[res_id].CIDCFGR;
  const std::uint32_t semcr = RCC->R[res_id].SEMCR;
  const std::uint32_t cfgr_val = cfgr_reg;

  const bool cfen = (cidcfgr & 0x1U) != 0U;
  const bool semen = (cidcfgr & 0x2U) != 0U;
  const std::uint32_t scid = (cidcfgr >> 4U) & 0x7U;
  const std::uint32_t semwlc = (cidcfgr >> 16U) & 0xFFU;

  const bool m33_can_write = !is_sec && (!cfen || (!semen && scid == 2U) || (semen && ((semwlc & (1U << 2U)) != 0U)));

  std::printf("[RCC R%-3u %-5s] %s/%s CIDCFGR=0x%08X (CFEN=%u SEM=%u SCID=%u WL=0x%X) SEMCR=0x%08X | CFGR=0x%08X | M33_WRITE=%s\n",
              res_id, name, is_sec ? "SEC" : "NS ", is_priv ? "PRV" : "UPR",
              cidcfgr, cfen ? 1U : 0U, semen ? 1U : 0U, scid, semwlc, semcr, cfgr_val,
              m33_can_write ? "ALLOW" : "DENY(IAC!)");
}

void ProbeGpioPinRif(const char* port_name, GPIO_TypeDef* port, std::uint32_t pin) {
  const bool is_sec = ((port->SECCFGR >> pin) & 1U) != 0U;
  const bool is_priv = ((port->PRIVCFGR >> pin) & 1U) != 0U;
  const volatile std::uint32_t* cid_base = &port->CIDCFGR0;
  const volatile std::uint32_t* sem_base = &port->SEMCR0;
  const std::uint32_t cidcfgr = cid_base[pin * 2U];
  const std::uint32_t semcr = sem_base[pin * 2U];

  const bool cfen = (cidcfgr & GPIO_CIDCFGR0_CFEN_Msk) != 0U;
  const bool semen = (cidcfgr & GPIO_CIDCFGR0_SEM_EN_Msk) != 0U;
  const std::uint32_t scid = (cidcfgr & GPIO_CIDCFGR0_SCID_Msk) >> GPIO_CIDCFGR0_SCID_Pos;
  const std::uint32_t semwlc = (cidcfgr >> GPIO_CIDCFGR0_SEMWLC0_Pos) & 0xFU;
  const bool m33_can_write = !is_sec && (!cfen || (!semen && scid == 2U) || (semen && ((semwlc & (1U << 2U)) != 0U)));

  const std::uint32_t mode = (port->MODER >> (pin * 2U)) & 0x3U;
  const std::uint32_t afr = (pin < 8U) ? ((port->AFR[0] >> (pin * 4U)) & 0xFU) : ((port->AFR[1] >> ((pin - 8U) * 4U)) & 0xFU);

  std::printf("[GPIO %s%-2u]     %s/%s CIDCFGR=0x%08X (CFEN=%u SEM=%u SCID=%u WL=0x%X) SEMCR=0x%08X | MODE=%u AF=%u | M33_WRITE=%s\n",
              port_name, pin, is_sec ? "SEC" : "NS ", is_priv ? "PRV" : "UPR",
              cidcfgr, cfen ? 1U : 0U, semen ? 1U : 0U, scid, semwlc, semcr, mode, afr,
              m33_can_write ? "ALLOW" : "DENY(IAC!)");
}

void ProbeI2C1RifChain() {
  std::printf("\n=== [M33 RIF Probe] MX_I2C1_Init Dependency Chain ===\n");

  // 1. RIFSC (RISC) I2C1 (ID = 41)
  constexpr std::uint32_t kI2c1Id = 41U;
  const bool i2c1_sec = ((RISC->SECCFGR[kI2c1Id / 32U] >> (kI2c1Id % 32U)) & 1U) != 0U;
  const bool i2c1_priv = ((RISC->PRIVCFGR[kI2c1Id / 32U] >> (kI2c1Id % 32U)) & 1U) != 0U;
  const std::uint32_t i2c1_cid = RISC->PER[kI2c1Id].CIDCFGR;
  const std::uint32_t i2c1_sem = RISC->PER[kI2c1Id].SEMCR;
  std::printf("[RIFSC ID=41 I2C1] %s/%s CIDCFGR=0x%08X SEMCR=0x%08X | RCC_I2C1CFGR=0x%08X\n",
              i2c1_sec ? "SEC" : "NS ", i2c1_priv ? "PRV" : "UPR", i2c1_cid, i2c1_sem,
              static_cast<std::uint32_t>(RCC->I2C1CFGR));

  // 2. RCC GPIO Clock Resources (95=GPIOF, 96=GPIOG, 97=GPIOH, 98=GPIOI)
  ProbeRccResource("GPIOF", 95U, RCC->GPIOFCFGR);
  ProbeRccResource("GPIOG", 96U, RCC->GPIOGCFGR);
  ProbeRccResource("GPIOH", 97U, RCC->GPIOHCFGR);
  ProbeRccResource("GPIOI", 98U, RCC->GPIOICFGR);

  // 3. GPIO Pin RIF for I2C1_SDA (PI1) and I2C1_SCL (PG13)
  ProbeGpioPinRif("PI", GPIOI, 1U);
  ProbeGpioPinRif("PG", GPIOG, 13U);
  std::printf("=====================================================\n\n");
}

} // namespace

int main() {
  HAL_Init();

  if (IS_DEVELOPER_BOOT_MODE()) {
    SystemClock_Config();
  } else {
    SystemCoreClockUpdate();
  }

  if (!IS_DEVELOPER_BOOT_MODE()) {
    MX_IPCC1_Init();
    CoproSync_Init();
  }

  MX_GPIO_Init();
  MX_ADC3_Init();
  ProbeI2C1RifChain();

  std::printf("[M33] Calling MX_I2C1_Init()...\n");
  MX_I2C1_Init();
  std::printf("[M33] MX_I2C1_Init() completed! RCC_I2C1CFGR=0x%08X I2C1_CR1=0x%08X TIMINGR=0x%08X ISR=0x%08X\n",
              static_cast<std::uint32_t>(RCC->I2C1CFGR),
              static_cast<std::uint32_t>(I2C1->CR1),
              static_cast<std::uint32_t>(I2C1->TIMINGR),
              static_cast<std::uint32_t>(I2C1->ISR));
  ProbeGpioPinRif("PI", GPIOI, 1U);
  ProbeGpioPinRif("PG", GPIOG, 13U);

  while (true) {
    std::printf("Hello World.\n");
    HAL_Delay(500);
  }
}
