################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/JLX12864G_Driver/JLX12864.c 

OBJS += \
./Drivers/JLX12864G_Driver/JLX12864.o 

C_DEPS += \
./Drivers/JLX12864G_Driver/JLX12864.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/JLX12864G_Driver/%.o: ../Drivers/JLX12864G_Driver/%.c Drivers/JLX12864G_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/JLX12864G/Drivers/JLX12864G_Driver" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-JLX12864G_Driver

clean-Drivers-2f-JLX12864G_Driver:
	-$(RM) ./Drivers/JLX12864G_Driver/JLX12864.d ./Drivers/JLX12864G_Driver/JLX12864.o

.PHONY: clean-Drivers-2f-JLX12864G_Driver

