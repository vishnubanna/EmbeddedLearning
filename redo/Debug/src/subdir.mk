################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/hw5.s 

C_SRCS += \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/test.c 

OBJS += \
./src/hw5.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/test.o 

C_DEPS += \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"/Users/vishnubanna/ECE362CODE/hw5true/Utilities" -I"/Users/vishnubanna/ECE362CODE/hw5true/StdPeriph_Driver/inc" -I"/Users/vishnubanna/ECE362CODE/hw5true/inc" -I"/Users/vishnubanna/ECE362CODE/hw5true/CMSIS/device" -I"/Users/vishnubanna/ECE362CODE/hw5true/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/vishnubanna/ECE362CODE/hw5true/Utilities" -I"/Users/vishnubanna/ECE362CODE/hw5true/StdPeriph_Driver/inc" -I"/Users/vishnubanna/ECE362CODE/hw5true/inc" -I"/Users/vishnubanna/ECE362CODE/hw5true/CMSIS/device" -I"/Users/vishnubanna/ECE362CODE/hw5true/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


