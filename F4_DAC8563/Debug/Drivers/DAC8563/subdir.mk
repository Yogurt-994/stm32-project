################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DAC8563/DAC8563.c 

OBJS += \
./Drivers/DAC8563/DAC8563.o 

C_DEPS += \
./Drivers/DAC8563/DAC8563.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DAC8563/%.o Drivers/DAC8563/%.su: ../Drivers/DAC8563/%.c Drivers/DAC8563/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/F4_DAC8563/Drivers/DAC8563" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DAC8563

clean-Drivers-2f-DAC8563:
	-$(RM) ./Drivers/DAC8563/DAC8563.d ./Drivers/DAC8563/DAC8563.o ./Drivers/DAC8563/DAC8563.su

.PHONY: clean-Drivers-2f-DAC8563

