################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include config/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RAMTestPOST_example.axf

# Tool invocations
RAMTestPOST_example.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\CMSISv1p30_LPC11xx\Debug" -L"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\IEC60335_B_libs\Debug" -Xlinker -Map=RAMTestPOST_example.map -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "RAMTestPOST_example_Debug.ld" -o"RAMTestPOST_example.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) RAMTestPOST_example.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size RAMTestPOST_example.axf; # arm-none-eabi-objcopy -O binary RAMTestPOST_example.axf RAMTestPOST_example.bin ; checksum -p LPC1114_302 -d RAMTestPOST_example.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
