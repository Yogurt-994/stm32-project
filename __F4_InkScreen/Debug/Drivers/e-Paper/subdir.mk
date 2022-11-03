################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/e-Paper/EPD_2in13_V3.c \
../Drivers/e-Paper/EPD_2in13_V3_test.c \
../Drivers/e-Paper/ImageData.c \
../Drivers/e-Paper/ImageData2.c 

OBJS += \
./Drivers/e-Paper/EPD_2in13_V3.o \
./Drivers/e-Paper/EPD_2in13_V3_test.o \
./Drivers/e-Paper/ImageData.o \
./Drivers/e-Paper/ImageData2.o 

C_DEPS += \
./Drivers/e-Paper/EPD_2in13_V3.d \
./Drivers/e-Paper/EPD_2in13_V3_test.d \
./Drivers/e-Paper/ImageData.d \
./Drivers/e-Paper/ImageData2.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/e-Paper/%.o Drivers/e-Paper/%.su: ../Drivers/e-Paper/%.c Drivers/e-Paper/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Config" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/Fonts" -I"D:/CubeIDE/workspace/__F4_InkScreen/Drivers/e-Paper/GUI" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-e-2d-Paper

clean-Drivers-2f-e-2d-Paper:
	-$(RM) ./Drivers/e-Paper/EPD_2in13_V3.d ./Drivers/e-Paper/EPD_2in13_V3.o ./Drivers/e-Paper/EPD_2in13_V3.su ./Drivers/e-Paper/EPD_2in13_V3_test.d ./Drivers/e-Paper/EPD_2in13_V3_test.o ./Drivers/e-Paper/EPD_2in13_V3_test.su ./Drivers/e-Paper/ImageData.d ./Drivers/e-Paper/ImageData.o ./Drivers/e-Paper/ImageData.su ./Drivers/e-Paper/ImageData2.d ./Drivers/e-Paper/ImageData2.o ./Drivers/e-Paper/ImageData2.su

.PHONY: clean-Drivers-2f-e-2d-Paper

