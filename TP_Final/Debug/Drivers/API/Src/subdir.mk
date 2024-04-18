################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_FSM.c \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_delay.c \
../Drivers/API/Src/API_lcd.c \
../Drivers/API/Src/API_lcd_port.c \
../Drivers/API/Src/dht.c \
../Drivers/API/Src/dht_port.c 

OBJS += \
./Drivers/API/Src/API_FSM.o \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_delay.o \
./Drivers/API/Src/API_lcd.o \
./Drivers/API/Src/API_lcd_port.o \
./Drivers/API/Src/dht.o \
./Drivers/API/Src/dht_port.o 

C_DEPS += \
./Drivers/API/Src/API_FSM.d \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_delay.d \
./Drivers/API/Src/API_lcd.d \
./Drivers/API/Src/API_lcd_port.d \
./Drivers/API/Src/dht.d \
./Drivers/API/Src/dht_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/CMSIS/Include" -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/Core/Inc" -I"/home/pablo/CESE/Materias/PdM/PdM_workspace/TP_Final/Drivers/API/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_FSM.cyclo ./Drivers/API/Src/API_FSM.d ./Drivers/API/Src/API_FSM.o ./Drivers/API/Src/API_FSM.su ./Drivers/API/Src/API_debounce.cyclo ./Drivers/API/Src/API_debounce.d ./Drivers/API/Src/API_debounce.o ./Drivers/API/Src/API_debounce.su ./Drivers/API/Src/API_delay.cyclo ./Drivers/API/Src/API_delay.d ./Drivers/API/Src/API_delay.o ./Drivers/API/Src/API_delay.su ./Drivers/API/Src/API_lcd.cyclo ./Drivers/API/Src/API_lcd.d ./Drivers/API/Src/API_lcd.o ./Drivers/API/Src/API_lcd.su ./Drivers/API/Src/API_lcd_port.cyclo ./Drivers/API/Src/API_lcd_port.d ./Drivers/API/Src/API_lcd_port.o ./Drivers/API/Src/API_lcd_port.su ./Drivers/API/Src/dht.cyclo ./Drivers/API/Src/dht.d ./Drivers/API/Src/dht.o ./Drivers/API/Src/dht.su ./Drivers/API/Src/dht_port.cyclo ./Drivers/API/Src/dht_port.d ./Drivers/API/Src/dht_port.o ./Drivers/API/Src/dht_port.su

.PHONY: clean-Drivers-2f-API-2f-Src

