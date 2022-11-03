################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ADS8688/ADS8688.c 

OBJS += \
./Drivers/ADS8688/ADS8688.o 

C_DEPS += \
./Drivers/ADS8688/ADS8688.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ADS8688/%.o Drivers/ADS8688/%.su: ../Drivers/ADS8688/%.c Drivers/ADS8688/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/F4_ADS8688/Drivers/ADS8688" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ADS8688

clean-Drivers-2f-ADS8688:
	-$(RM) ./Drivers/ADS8688/ADS8688.d ./Drivers/ADS8688/ADS8688.o ./Drivers/ADS8688/ADS8688.su

.PHONY: clean-Drivers-2f-ADS8688

