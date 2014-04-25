################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CT32B0.c \
../src/cr_startup_lpc11.c \
../src/lpc11xx_led.c \
../src/main.c 

OBJS += \
./src/CT32B0.o \
./src/cr_startup_lpc11.o \
./src/lpc11xx_led.o \
./src/main.o 

C_DEPS += \
./src/CT32B0.d \
./src/cr_startup_lpc11.d \
./src/lpc11xx_led.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSISv1p30_LPC11xx -DLPC1114 -DDEBUG -D__CODE_RED -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\CMSISv1p30_LPC11xx\inc" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\InterruptTest_example\src" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\InterruptTest_example\config" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\IEC60335_B_libs\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc11.o: ../src/cr_startup_lpc11.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSISv1p30_LPC11xx -DLPC1114 -DDEBUG -D__CODE_RED -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\CMSISv1p30_LPC11xx\inc" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\InterruptTest_example\src" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\InterruptTest_example\config" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\IEC60335_B_libs\inc" -O0 -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc11.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


