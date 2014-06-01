################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: nxp_lpcxpresso_11c24_periph_ssp.axf

# Tool invocations
nxp_lpcxpresso_11c24_periph_ssp.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_board_lib/Debug" -L"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/lpc_chip_11cxx_lib/Debug" -Xlinker -Map="nxp_lpcxpresso_11c24_periph_ssp.map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "nxp_lpcxpresso_11c24_periph_ssp_Debug.ld" -o "nxp_lpcxpresso_11c24_periph_ssp.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) nxp_lpcxpresso_11c24_periph_ssp.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "nxp_lpcxpresso_11c24_periph_ssp.axf"; # arm-none-eabi-objcopy -O binary "nxp_lpcxpresso_11c24_periph_ssp.axf" "nxp_lpcxpresso_11c24_periph_ssp.bin" ; checksum -p LPC11C24_301 -d "nxp_lpcxpresso_11c24_periph_ssp.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
