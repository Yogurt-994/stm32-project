################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Driver_ESP8266/ESP8266.c 

OBJS += \
./Drivers/Driver_ESP8266/ESP8266.o 

C_DEPS += \
./Drivers/Driver_ESP8266/ESP8266.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Driver_ESP8266/%.o Drivers/Driver_ESP8266/%.su: ../Drivers/Driver_ESP8266/%.c Drivers/Driver_ESP8266/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/CubeIDE/workspace/ESP8266/Drivers/Driver_ESP8266" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Driver_ESP8266

clean-Drivers-2f-Driver_ESP8266:
	-$(RM) ./Drivers/Driver_ESP8266/ESP8266.d ./Drivers/Driver_ESP8266/ESP8266.o ./Drivers/Driver_ESP8266/ESP8266.su

.PHONY: clean-Drivers-2f-Driver_ESP8266

