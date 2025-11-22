################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include simplicity_sdk_2024.6.2/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/device_init/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/emlib/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/common/src/subdir.mk
-include simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2024.6.2/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := lab1
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: lab1.axf

# Tool invocations
lab1.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "D:\IOT\Thuc hanh\SiliconLab\lab1\autogen\linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="lab1.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o lab1.axf -Wl,--start-group "./app.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./simplicity_sdk_2024.6.2/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2024.6.2/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2024.6.2/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/startup_efr32mg21.o" "./simplicity_sdk_2024.6.2/platform/Device/SiliconLabs/EFR32MG21/Source/system_efr32mg21.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.6.2/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.6.2/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_core.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_emu.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_msc.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_system.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_timer.o" "./simplicity_sdk_2024.6.2/platform/emlib/src/em_usart.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2024.6.2/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2024.6.2/platform/service/device_init/src/sl_device_init_emu_s2.o" "./simplicity_sdk_2024.6.2/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2024.6.2/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2024.6.2/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2024.6.2/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.6.2/platform/service/system/src/sl_system_process_action.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: lab1.out'
	arm-none-eabi-objcopy "lab1.axf" "lab1.out"
	@echo ' '

	@echo 'Building hex file: lab1.hex'
	arm-none-eabi-objcopy -O ihex "lab1.axf" "lab1.hex"
	@echo ' '

	@echo 'Building bin file: lab1.bin'
	arm-none-eabi-objcopy -O binary "lab1.axf" "lab1.bin"
	@echo ' '

	@echo 'Building s37 file: lab1.s37'
	arm-none-eabi-objcopy -O srec "lab1.axf" "lab1.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "lab1.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) lab1.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
