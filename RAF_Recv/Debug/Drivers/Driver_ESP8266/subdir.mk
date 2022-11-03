################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Driver_ESP8266/ESP8266.c 

OBJS += \
./Drivers/Driver_ESP8266/ESP8266.o 

C_DEPS += \
./Drivers/Driver_ESP8266/ESP8266.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Driver_ESP8266/%.o: ../Drivers/Driver_ESP8266/%.c Drivers/Driver_ESP8266/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/RAF_Recv/Drivers/STM32F4xx_VisualTFT" -I"D:/CubeIDE/workspace/RAF_Recv/Drivers/Driver_ESP8266" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Driver_ESP8266

clean-Drivers-2f-Driver_ESP8266:
	-$(RM) ./Drivers/Driver_ESP8266/ESP8266.d ./Drivers/Driver_ESP8266/ESP8266.o

.PHONY: clean-Drivers-2f-Driver_ESP8266

