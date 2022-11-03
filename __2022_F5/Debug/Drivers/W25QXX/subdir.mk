################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/W25QXX/W25QXX.c 

OBJS += \
./Drivers/W25QXX/W25QXX.o 

C_DEPS += \
./Drivers/W25QXX/W25QXX.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/W25QXX/%.o Drivers/W25QXX/%.su: ../Drivers/W25QXX/%.c Drivers/W25QXX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 '-D__FPU_PRESENT = 1U' -D_TARGET_FPU_VFP -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/CubeIDE/workspace/__2022_F/Drivers/ADS1256" -I"D:/CubeIDE/workspace/__2022_F/Drivers/ADS8688_SPI" -I../Core/Inc -I"D:/CubeIDE/workspace/__2022_F/Drivers/AT24CXX" -I"D:/CubeIDE/workspace/__2022_F/Drivers/W25QXX" -I"D:/CubeIDE/workspace/__2022_F/Drivers/Driver_ESP8266" -I"D:/CubeIDE/workspace/__2022_F/Drivers/DAC8563" -I"D:/CubeIDE/workspace/__2022_F/Drivers/AD9959" -I"D:/CubeIDE/workspace/__2022_F/Drivers/STM32F4xx_VisualTFT" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-W25QXX

clean-Drivers-2f-W25QXX:
	-$(RM) ./Drivers/W25QXX/W25QXX.d ./Drivers/W25QXX/W25QXX.o ./Drivers/W25QXX/W25QXX.su

.PHONY: clean-Drivers-2f-W25QXX

