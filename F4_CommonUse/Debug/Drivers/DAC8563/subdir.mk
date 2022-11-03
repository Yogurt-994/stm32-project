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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 '-D__FPU_PRESENT = 1U' -D_TARGET_FPU_VFP -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/ADS1256" -I../Core/Inc -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/AT24CXX" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/W25QXX" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/Driver_ESP8266" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/DAC8563" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/AD9959" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/ADS8688" -I"D:/CubeIDE/workspace/F4_CommonUse/Drivers/STM32F4xx_VisualTFT" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DAC8563

clean-Drivers-2f-DAC8563:
	-$(RM) ./Drivers/DAC8563/DAC8563.d ./Drivers/DAC8563/DAC8563.o ./Drivers/DAC8563/DAC8563.su

.PHONY: clean-Drivers-2f-DAC8563

