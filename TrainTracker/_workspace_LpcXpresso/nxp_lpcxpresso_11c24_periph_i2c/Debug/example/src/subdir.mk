################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_startup_lpc11xx.c \
../example/src/i2c.c \
../example/src/sysinit.c 

OBJS += \
./example/src/cr_startup_lpc11xx.o \
./example/src/i2c.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/cr_startup_lpc11xx.d \
./example/src/i2c.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/cr_startup_lpc11xx.o: ../example/src/cr_startup_lpc11xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M0 -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_periph_i2c/example/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/lpc_chip_11cxx_lib/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_board_lib/inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"example/src/cr_startup_lpc11xx.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M0 -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_periph_i2c/example/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/lpc_chip_11cxx_lib/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_board_lib/inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


