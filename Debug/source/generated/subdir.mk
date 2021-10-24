################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/generated/usb_device_composite.c \
../source/generated/usb_device_descriptor.c 

OBJS += \
./source/generated/usb_device_composite.o \
./source/generated/usb_device_descriptor.o 

C_DEPS += \
./source/generated/usb_device_composite.d \
./source/generated/usb_device_descriptor.d 


# Each subdirectory must supply rules for building sources it contributes
source/generated/%.o: ../source/generated/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\board" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


