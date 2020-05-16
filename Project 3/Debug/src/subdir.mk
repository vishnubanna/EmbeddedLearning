################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/awesome-song.c \
../src/fifo.c \
../src/game.c \
../src/main.c \
../src/midi.c \
../src/screen.c \
../src/song.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/tty.c 

OBJS += \
./src/awesome-song.o \
./src/fifo.o \
./src/game.o \
./src/main.o \
./src/midi.o \
./src/screen.o \
./src/song.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/tty.o 

C_DEPS += \
./src/awesome-song.d \
./src/fifo.d \
./src/game.d \
./src/main.d \
./src/midi.d \
./src/screen.d \
./src/song.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/tty.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/vishnubanna/ECE362CODE/Project 3/Utilities" -I"/Users/vishnubanna/ECE362CODE/Project 3/StdPeriph_Driver/inc" -I"/Users/vishnubanna/ECE362CODE/Project 3/inc" -I"/Users/vishnubanna/ECE362CODE/Project 3/CMSIS/device" -I"/Users/vishnubanna/ECE362CODE/Project 3/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


