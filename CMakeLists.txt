# clear; cmake --fresh -B build -G Ninja --toolchain ./.cross.cmake; cmake --build build -j
cmake_minimum_required(VERSION 3.28)
project(stm32mp257fak3-m33 ASM C CXX)
add_executable(${PROJECT_NAME})
set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/CM33/refactoring/STM32MP257FAKX_RAM.ld")
set(global_marcos USE_HAL_DRIVER USE_FULL_LL_DRIVER CORE_CM33 STM32MP257Fxx)

target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
    retarget_io.cpp
)

file(GLOB_RECURSE ALL_CXX_MODULES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cppm"
)

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET CXX_MODULES FILES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${ALL_CXX_MODULES}
)

target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE
    STM32CubeMX/CM33/Core/Inc
    STM32CubeMX/Drivers/STM32MP2xx_HAL_Driver/Inc
    STM32CubeMX/Drivers/STM32MP2xx_HAL_Driver/Inc/Legacy
    STM32CubeMX/Utilities/CoproSync
    STM32CubeMX/Drivers/CMSIS/Device/ST/STM32MP2xx/Include
    STM32CubeMX/Drivers/CMSIS/Include
)

file(GLOB_RECURSE CUBEMX_C_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/CM33/Core/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/Drivers/STM32MP2xx_HAL_Driver/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/Utilities/CoproSync/copro_sync.c"
)
list(REMOVE_ITEM CUBEMX_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/CM33/Core/Src/sysmem.c")
list(REMOVE_ITEM CUBEMX_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/CM33/Core/Src/syscalls.c")

target_sources(${PROJECT_NAME}
  PRIVATE
    STM32CubeMX/CM33/refactoring/startup_stm32mp257fakx.S
    STM32CubeMX/Common/System/system_stm32mp2xx_m33.c
    ${CUBEMX_C_SOURCES}
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -gdwarf-4

    $<$<COMPILE_LANGUAGE:C,CXX>:
      -O0
      -flto
      -Wall -Wextra -Wpedantic
      # -Werror
      -ffunction-sections -fdata-sections
      -fstack-usage
      -funsigned-char
      -Wno-packed
      -Wno-reserved-id-macro
      -Wno-unused-macros
      -Wno-documentation-unknown-command
      -Wno-documentation
      -Wno-parentheses-equality
      -Wno-reserved-identifier
    >

    $<$<COMPILE_LANGUAGE:C>:
      -Wno-strict-prototypes
    >

    $<$<COMPILE_LANGUAGE:CXX>:
      -fno-exceptions -fno-rtti
      -fno-threadsafe-statics
      -fno-use-cxa-atexit
    >
)

# -Wall -Wextra -Wpedantic -Werror 警告
# -flto 跨模块链接
# -fno-exceptions -fno-rtti 禁用异常
# -ffunction-sections -fdata-sections 配合 -Wl,--gc-sections 剔除未使用的函数和数据
# -fstack-usage 生成 .su 文件, 记录栈占用.
# -funsigned-char ARM特色
# -gdwarf-4 调试兼容选项
# -Wno* CMSIS和ST非标准代码需要
# -fno-threadsafe-statics 局部static变量不再线程安全
# -fno-use-cxa-atexit 移除程序退出时的析构代码


target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    $<$<COMPILE_LANGUAGE:C,CXX>:
      ${global_marcos}
      $<$<CONFIG:Debug>:DEBUG>
    >
)

# target_compile_features(${PROJECT_NAME} PRIVATE c_std_11 cxx_std_26)
set_target_properties(${PROJECT_NAME} PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS OFF
  CXX_STANDARD 26
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
)

target_link_options(${PROJECT_NAME}
  PRIVATE
    "-T${linker_script}"
    "-nostartfiles"
    "-Wl,-Map=${PROJECT_NAME}.map"
    "-Wl,--cref"
    "-Wl,--print-memory-usage"
    "-Wl,--gc-sections"
    "-Wl,--build-id=none"
    "-Wl,-z,noexecstack"
)

# -nostartfiles 可以不写. 不使用默认启动文件(比如crt0.o), 但CMake可以不写.
# -Wl,--cref -Wl,-Map=${PROJECT_NAME}.map 配合 生成交叉引用表
# -Wl,--print-memory-usage 链接后打印内存使用情况
# -Wl,--build-id=none 禁用构建标识符, 嵌入式不需要
# -Wl,-z,noexecstack 现代LLVM需要

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}> > ${PROJECT_NAME}.lst
  # COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  VERBATIM
)

# .lst 全景反汇编列表
# --all-headers 打印所有的 ELF 段（Section）头信息
# --demangle C++ 符号解混淆
# --disassemble 反汇编代码段

# CMAKE_SIZE
# 可以不写
# --format=berkeley 输出格式

# VERBATIM 确保命令字符串按字面意思处理, 不进行 CMake 变量替换等




set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeMX/Utilities/CoproSync/copro_sync.c
  PROPERTIES
    COMPILE_OPTIONS "-Wno-unused-parameter"
)
