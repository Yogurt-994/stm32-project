################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
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
Drivers/STM32F4xx_VisualTFT/%.o Drivers/STM32F4xx_VisualTFT/%.su: ../Drivers/STM32F4xx_VisualTFT/%.c Drivers/STM32F4xx_VisualTFT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT = 1U' -D_TARGET_FPU_VFP -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/SimCirChaTest/Drivers/AD9959" -I"D:/CubeIDE/workspace/SimCirChaTest/Drivers/STM32F4xx_VisualTFT" -I"D:/CubeIDE/workspace/SimCirChaTest/Drivers/ADS8688" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32F4xx_VisualTFT

clean-Drivers-2f-STM32F4xx_VisualTFT:
	-$(RM) ./Drivers/STM32F4xx_VisualTFT/cmd_process.d ./Drivers/STM32F4xx_VisualTFT/cmd_process.o ./Drivers/STM32F4xx_VisualTFT/cmd_process.su ./Drivers/STM32F4xx_VisualTFT/cmd_queue.d ./Drivers/STM32F4xx_VisualTFT/cmd_queue.o ./Drivers/STM32F4xx_VisualTFT/cmd_queue.su ./Drivers/STM32F4xx_VisualTFT/hmi_driver.d ./Drivers/STM32F4xx_VisualTFT/hmi_driver.o ./Drivers/STM32F4xx_VisualTFT/hmi_driver.su ./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.d ./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.o ./Drivers/STM32F4xx_VisualTFT/hmi_user_uart.su

.PHONY: clean-Drivers-2f-STM32F4xx_VisualTFT

