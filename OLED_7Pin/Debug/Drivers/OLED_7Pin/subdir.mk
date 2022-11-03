################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/OLED_7Pin/oled.c 

OBJS += \
./Drivers/OLED_7Pin/oled.o 

C_DEPS += \
./Drivers/OLED_7Pin/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/OLED_7Pin/%.o Drivers/OLED_7Pin/%.su: ../Drivers/OLED_7Pin/%.c Drivers/OLED_7Pin/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/OLED_7Pin/Drivers/OLED_7Pin" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-OLED_7Pin

clean-Drivers-2f-OLED_7Pin:
	-$(RM) ./Drivers/OLED_7Pin/oled.d ./Drivers/OLED_7Pin/oled.o ./Drivers/OLED_7Pin/oled.su

.PHONY: clean-Drivers-2f-OLED_7Pin

