################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/src/FreeRTOSCommonHooks.c \
../freertos/src/croutine.c \
../freertos/src/heap_1.c \
../freertos/src/list.c \
../freertos/src/port.c \
../freertos/src/queue.c \
../freertos/src/tasks.c \
../freertos/src/timers.c 

OBJS += \
./freertos/src/FreeRTOSCommonHooks.o \
./freertos/src/croutine.o \
./freertos/src/heap_1.o \
./freertos/src/list.o \
./freertos/src/port.o \
./freertos/src/queue.o \
./freertos/src/tasks.o \
./freertos/src/timers.o 

C_DEPS += \
./freertos/src/FreeRTOSCommonHooks.d \
./freertos/src/croutine.d \
./freertos/src/heap_1.d \
./freertos/src/list.d \
./freertos/src/port.d \
./freertos/src/queue.d \
./freertos/src/tasks.d \
./freertos/src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/src/%.o: ../freertos/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M0 -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_freertos_blinky/example/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_freertos_blinky/freertos/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/lpc_chip_11cxx_lib/inc" -I"/Users/keller_reto/_work/TrainHub/TrainTracker/_workspace_LpcXpresso/nxp_lpcxpresso_11c24_board_lib/inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


