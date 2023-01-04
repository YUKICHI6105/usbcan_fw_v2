################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/CRSLib/Can/RM0365/Sample/src/sample.cpp 

OBJS += \
./Drivers/CRSLib/Can/RM0365/Sample/src/sample.o 

CPP_DEPS += \
./Drivers/CRSLib/Can/RM0365/Sample/src/sample.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CRSLib/Can/RM0365/Sample/src/%.o Drivers/CRSLib/Can/RM0365/Sample/src/%.su: ../Drivers/CRSLib/Can/RM0365/Sample/src/%.cpp Drivers/CRSLib/Can/RM0365/Sample/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=c++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/taman/STM32CubeIDE/workspace_1.11.0/usbcan_fw_v2/Drivers" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CRSLib-2f-Can-2f-RM0365-2f-Sample-2f-src

clean-Drivers-2f-CRSLib-2f-Can-2f-RM0365-2f-Sample-2f-src:
	-$(RM) ./Drivers/CRSLib/Can/RM0365/Sample/src/sample.d ./Drivers/CRSLib/Can/RM0365/Sample/src/sample.o ./Drivers/CRSLib/Can/RM0365/Sample/src/sample.su

.PHONY: clean-Drivers-2f-CRSLib-2f-Can-2f-RM0365-2f-Sample-2f-src

