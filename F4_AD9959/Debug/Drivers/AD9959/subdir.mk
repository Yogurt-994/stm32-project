################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AD9959/AD9959.c 

OBJS += \
./Drivers/AD9959/AD9959.o 

C_DEPS += \
./Drivers/AD9959/AD9959.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AD9959/%.o: ../Drivers/AD9959/%.c Drivers/AD9959/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/F4_AD9959/Drivers/AD9959" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-AD9959

clean-Drivers-2f-AD9959:
	-$(RM) ./Drivers/AD9959/AD9959.d ./Drivers/AD9959/AD9959.o

.PHONY: clean-Drivers-2f-AD9959

