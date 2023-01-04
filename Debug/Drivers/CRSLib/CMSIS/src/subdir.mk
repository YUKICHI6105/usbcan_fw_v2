################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.c 

C_DEPS += \
./Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.d 

OBJS += \
./Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CRSLib/CMSIS/src/%.o Drivers/CRSLib/CMSIS/src/%.su: ../Drivers/CRSLib/CMSIS/src/%.c Drivers/CRSLib/CMSIS/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CRSLib-2f-CMSIS-2f-src

clean-Drivers-2f-CRSLib-2f-CMSIS-2f-src:
	-$(RM) ./Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.d ./Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.o ./Drivers/CRSLib/CMSIS/src/cmsis_for_cpp.su

.PHONY: clean-Drivers-2f-CRSLib-2f-CMSIS-2f-src

