################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\drivers" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\utilities" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\serial_manager" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\uart" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\include" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class\cdc" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device\class" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\include" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\source\khci" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\device\class" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\usb\output\source\device" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\lists" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\component\osa" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source\generated" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\CMSIS" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\device" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\board" -I"C:\Users\Lauta\Documents\MCUXpressoIDE_11.4.0_6224\workspace\usbsss\source" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


