################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F4xx_VisualTFT/cmd_process.c \
../Drivers/STM32F4xx_VisualTFT/cmd_queue.c \
../Drivers/STM32F4xx_VisualTFT/hmi_driver.c \
../Drivers/STM32F4xx_VisualTFT/hmi_user_uart.c 

OBJS += \
./Drivers/STM32F4xx_VisualTFT/cmd_process.o \
./Drivers/STM32F4xx_VisualTFT/cmd_queue.o \
./Drivers/STM32F4xx_VisualTFT/hmi_driver.o \
./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.o 

C_DEPS += \
./Drivers/STM32F4xx_VisualTFT/cmd_process.d \
./Drivers/STM32F4xx_VisualTFT/cmd_queue.d \
./Drivers/STM32F4xx_VisualTFT/hmi_driver.d \
./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_VisualTFT/%.o: ../Drivers/STM32F4xx_VisualTFT/%.c Drivers/STM32F4xx_VisualTFT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/RAF_Recv/Drivers/STM32F4xx_VisualTFT" -I"D:/CubeIDE/workspace/RAF_Recv/Drivers/Driver_ESP8266" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32F4xx_VisualTFT

clean-Drivers-2f-STM32F4xx_VisualTFT:
	-$(RM) ./Drivers/STM32F4xx_VisualTFT/cmd_process.d ./Drivers/STM32F4xx_VisualTFT/cmd_process.o ./Drivers/STM32F4xx_VisualTFT/cmd_queue.d ./Drivers/STM32F4xx_VisualTFT/cmd_queue.o ./Drivers/STM32F4xx_VisualTFT/hmi_driver.d ./Drivers/STM32F4xx_VisualTFT/hmi_driver.o ./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.d ./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.o

.PHONY: clean-Drivers-2f-STM32F4xx_VisualTFT

