################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc11xx.c \
../src/crp.c \
../src/main.c \
../src/sysinit.c 

OBJS += \
./src/cr_startup_lpc11xx.o \
./src/crp.o \
./src/main.o \
./src/sysinit.o 

C_DEPS += \
./src/cr_startup_lpc11xx.d \
./src/crp.d \
./src/main.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M0 -D__USE_LPCOPEN -D__LPC11XX__ -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_board_lib/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/lpc_chip_11cxx_lib/inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


