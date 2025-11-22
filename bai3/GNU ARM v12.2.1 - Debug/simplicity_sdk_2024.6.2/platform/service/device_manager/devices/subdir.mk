################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.c 

OBJS += \
./simplicity_sdk_2024.6.2/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.o 

C_DEPS += \
./simplicity_sdk_2024.6.2/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.d 


# Each subdirectory must supply rules for building sources it contributes
simplicity_sdk_2024.6.2/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.o: D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.c simplicity_sdk_2024.6.2/platform/service/device_manager/devices/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c18 '-DDEBUG=1' '-DDEBUG_EFM=1' '-DEFR32MG21A020F1024IM32=1' '-DSL_BOARD_NAME="BRD4180B"' '-DSL_BOARD_REV="A03"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=38400000' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DCMSIS_NVIC_VIRTUAL=1' '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' '-DSL_CODE_COMPONENT_CORE=core' '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' -I"D:\IOT\Thuc hanh\SiliconLab\bai3\config" -I"D:\IOT\Thuc hanh\SiliconLab\bai3\autogen" -I"D:\IOT\Thuc hanh\SiliconLab\bai3" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/Device/SiliconLabs/EFR32MG21/Include" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/common/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//hardware/board/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/clock_manager/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/clock_manager/src" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/CMSIS/Core/Include" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//hardware/driver/configuration_over_swo/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/driver/debug/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/device_manager/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/device_init/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/emlib/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/interrupt_manager/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/interrupt_manager/inc/arm" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/memory_manager/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/common/toolchain/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/system/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/sleeptimer/inc" -I"D:/SiliconLab/simplicity_sdk_suite_v2024_6_2-001/simplicity_sdk_5//platform/service/udelay/inc" -Os -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"simplicity_sdk_2024.6.2/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg21.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


