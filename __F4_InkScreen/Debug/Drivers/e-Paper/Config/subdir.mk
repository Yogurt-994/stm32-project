################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/e-Paper/Config/DEV_Config.c 

OBJS += \
./Drivers/e-Paper/Config/DEV_Config.o 

C_DEPS += \
./Drivers/e-Paper/Config/DEV_Config.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/e-Paper/Config/%.o Drivers/e-Paper/Config/%.su: ../Drivers/e-Paper/Config/%.c Drivers/e-Paper/Config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Config" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Fonts" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/GUI" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-e-2d-Paper-2f-Config

clean-Drivers-2f-e-2d-Paper-2f-Config:
	-$(RM) ./Drivers/e-Paper/Config/DEV_Config.d ./Drivers/e-Paper/Config/DEV_Config.o ./Drivers/e-Paper/Config/DEV_Config.su

.PHONY: clean-Drivers-2f-e-2d-Paper-2f-Config

