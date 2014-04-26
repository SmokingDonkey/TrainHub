################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IEC60335_B_ClockTest.c \
../src/IEC60335_B_Interrupts.c \
../src/IEC60335_B_ProgramCounterTest.c \
../src/IEC60335_B_RAMTestsBIST.c 

ASM_SRCS += \
../src/IEC60335_B_CPUregTestBIST_GNU.asm \
../src/IEC60335_B_CPUregTestPOST_GNU.asm \
../src/IEC60335_B_FLASHTest_GNU.asm \
../src/IEC60335_B_RAMTestPOST_GNU.asm 

OBJS += \
./src/IEC60335_B_CPUregTestBIST_GNU.o \
./src/IEC60335_B_CPUregTestPOST_GNU.o \
./src/IEC60335_B_ClockTest.o \
./src/IEC60335_B_FLASHTest_GNU.o \
./src/IEC60335_B_Interrupts.o \
./src/IEC60335_B_ProgramCounterTest.o \
./src/IEC60335_B_RAMTestPOST_GNU.o \
./src/IEC60335_B_RAMTestsBIST.o 

C_DEPS += \
./src/IEC60335_B_ClockTest.d \
./src/IEC60335_B_Interrupts.d \
./src/IEC60335_B_ProgramCounterTest.d \
./src/IEC60335_B_RAMTestsBIST.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__  -DDEBUG -D__CODE_RED -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\IEC60335_B_libs\inc" -g3 -mcpu=cortex-m0 -mthumb -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\RAMTestBIST_example\src" -I"C:\Users\nxp28536\sw\codebundle\libraries\cortex_m0\IEC60335_Class_B\v1.2\LPCXpresso\IEC60335_B_libs\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


