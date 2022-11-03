################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/e-Paper/Fonts/font12.c \
../Drivers/e-Paper/Fonts/font12CN.c \
../Drivers/e-Paper/Fonts/font16.c \
../Drivers/e-Paper/Fonts/font20.c \
../Drivers/e-Paper/Fonts/font24.c \
../Drivers/e-Paper/Fonts/font24CN.c \
../Drivers/e-Paper/Fonts/font8.c 

OBJS += \
./Drivers/e-Paper/Fonts/font12.o \
./Drivers/e-Paper/Fonts/font12CN.o \
./Drivers/e-Paper/Fonts/font16.o \
./Drivers/e-Paper/Fonts/font20.o \
./Drivers/e-Paper/Fonts/font24.o \
./Drivers/e-Paper/Fonts/font24CN.o \
./Drivers/e-Paper/Fonts/font8.o 

C_DEPS += \
./Drivers/e-Paper/Fonts/font12.d \
./Drivers/e-Paper/Fonts/font12CN.d \
./Drivers/e-Paper/Fonts/font16.d \
./Drivers/e-Paper/Fonts/font20.d \
./Drivers/e-Paper/Fonts/font24.d \
./Drivers/e-Paper/Fonts/font24CN.d \
./Drivers/e-Paper/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/e-Paper/Fonts/%.o Drivers/e-Paper/Fonts/%.su: ../Drivers/e-Paper/Fonts/%.c Drivers/e-Paper/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Config" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Fonts" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/GUI" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-e-2d-Paper-2f-Fonts

clean-Drivers-2f-e-2d-Paper-2f-Fonts:
	-$(RM) ./Drivers/e-Paper/Fonts/font12.d ./Drivers/e-Paper/Fonts/font12.o ./Drivers/e-Paper/Fonts/font12.su ./Drivers/e-Paper/Fonts/font12CN.d ./Drivers/e-Paper/Fonts/font12CN.o ./Drivers/e-Paper/Fonts/font12CN.su ./Drivers/e-Paper/Fonts/font16.d ./Drivers/e-Paper/Fonts/font16.o ./Drivers/e-Paper/Fonts/font16.su ./Drivers/e-Paper/Fonts/font20.d ./Drivers/e-Paper/Fonts/font20.o ./Drivers/e-Paper/Fonts/font20.su ./Drivers/e-Paper/Fonts/font24.d ./Drivers/e-Paper/Fonts/font24.o ./Drivers/e-Paper/Fonts/font24.su ./Drivers/e-Paper/Fonts/font24CN.d ./Drivers/e-Paper/Fonts/font24CN.o ./Drivers/e-Paper/Fonts/font24CN.su ./Drivers/e-Paper/Fonts/font8.d ./Drivers/e-Paper/Fonts/font8.o ./Drivers/e-Paper/Fonts/font8.su

.PHONY: clean-Drivers-2f-e-2d-Paper-2f-Fonts

