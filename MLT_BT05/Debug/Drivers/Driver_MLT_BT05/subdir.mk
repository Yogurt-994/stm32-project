################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Driver_MLT_BT05/MLT_BT05.c 

OBJS += \
./Drivers/Driver_MLT_BT05/MLT_BT05.o 

C_DEPS += \
./Drivers/Driver_MLT_BT05/MLT_BT05.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Driver_MLT_BT05/%.o: ../Drivers/Driver_MLT_BT05/%.c Drivers/Driver_MLT_BT05/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/CubeIDE/workspace/MLT_BT05/Drivers/Driver_MLT_BT05" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Driver_MLT_BT05

clean-Drivers-2f-Driver_MLT_BT05:
	-$(RM) ./Drivers/Driver_MLT_BT05/MLT_BT05.d ./Drivers/Driver_MLT_BT05/MLT_BT05.o

.PHONY: clean-Drivers-2f-Driver_MLT_BT05

