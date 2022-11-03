################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AD9959/AD9959.c 

OBJS += \
./Drivers/AD9959/AD9959.o 

C_DEPS += \
./Drivers/AD9959/AD9959.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AD9959/%.o Drivers/AD9959/%.su: ../Drivers/AD9959/%.c Drivers/AD9959/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 '-D__FPU_PRESENT = 1U' -D_TARGET_FPU_VFP -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/CubeIDE/workspace/__2022_F6/Drivers/ADS1256" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/ADS8688_SPI" -I../Core/Inc -I"D:/CubeIDE/workspace/__2022_F6/Drivers/AT24CXX" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/W25QXX" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/Driver_ESP8266" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/DAC8563" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/AD9959" -I"D:/CubeIDE/workspace/__2022_F6/Drivers/STM32F4xx_VisualTFT" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-AD9959

clean-Drivers-2f-AD9959:
	-$(RM) ./Drivers/AD9959/AD9959.d ./Drivers/AD9959/AD9959.o ./Drivers/AD9959/AD9959.su

.PHONY: clean-Drivers-2f-AD9959

