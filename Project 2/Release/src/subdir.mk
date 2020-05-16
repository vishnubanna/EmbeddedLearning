################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/initial-main.c \
../src/midi.c \
../src/stairway-to-heaven.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/initial-main.o \
./src/midi.o \
./src/stairway-to-heaven.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/initial-main.d \
./src/midi.d \
./src/stairway-to-heaven.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/vishnubanna/ECE362CODE/Project 2/Utilities" -I"/Users/vishnubanna/ECE362CODE/Project 2/StdPeriph_Driver/inc" -I"/Users/vishnubanna/ECE362CODE/Project 2/inc" -I"/Users/vishnubanna/ECE362CODE/Project 2/CMSIS/device" -I"/Users/vishnubanna/ECE362CODE/Project 2/CMSIS/core" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


