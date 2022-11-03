################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/e-Paper/GUI/GUI_Paint.c 

OBJS += \
./Drivers/e-Paper/GUI/GUI_Paint.o 

C_DEPS += \
./Drivers/e-Paper/GUI/GUI_Paint.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/e-Paper/GUI/%.o Drivers/e-Paper/GUI/%.su: ../Drivers/e-Paper/GUI/%.c Drivers/e-Paper/GUI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Config" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Fonts" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/GUI" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-e-2d-Paper-2f-GUI

clean-Drivers-2f-e-2d-Paper-2f-GUI:
	-$(RM) ./Drivers/e-Paper/GUI/GUI_Paint.d ./Drivers/e-Paper/GUI/GUI_Paint.o ./Drivers/e-Paper/GUI/GUI_Paint.su

.PHONY: clean-Drivers-2f-e-2d-Paper-2f-GUI

