################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc16.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_pit.c \
../drivers/fsl_smc.c \
../drivers/fsl_sysmpu.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_adc16.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_pit.o \
./drivers/fsl_smc.o \
./drivers/fsl_sysmpu.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_adc16.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_pit.d \
./drivers/fsl_smc.d \
./drivers/fsl_sysmpu.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\board" -I"D:\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


