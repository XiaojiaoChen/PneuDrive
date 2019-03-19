################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/framework/frameSrc/ADBoard.c \
../Src/framework/frameSrc/Controller.c \
../Src/framework/frameSrc/DABoard.c \
../Src/framework/frameSrc/PWMBoardI2C.c \
../Src/framework/frameSrc/PWMBoardSPI.c \
../Src/framework/frameSrc/builtInAnalog.c \
../Src/framework/frameSrc/builtInDigital.c \
../Src/framework/frameSrc/builtInPWM.c \
../Src/framework/frameSrc/kalman.c \
../Src/framework/frameSrc/myPID.c \
../Src/framework/frameSrc/myUsartFunction.c \
../Src/framework/frameSrc/pressureRegulator_ITV2030.c \
../Src/framework/frameSrc/trajectoryGeneration.c \
../Src/framework/frameSrc/valveFlowFunc.c 

CPP_SRCS += \
../Src/framework/frameSrc/PneuFunctions.cpp 

OBJS += \
./Src/framework/frameSrc/ADBoard.o \
./Src/framework/frameSrc/Controller.o \
./Src/framework/frameSrc/DABoard.o \
./Src/framework/frameSrc/PWMBoardI2C.o \
./Src/framework/frameSrc/PWMBoardSPI.o \
./Src/framework/frameSrc/PneuFunctions.o \
./Src/framework/frameSrc/builtInAnalog.o \
./Src/framework/frameSrc/builtInDigital.o \
./Src/framework/frameSrc/builtInPWM.o \
./Src/framework/frameSrc/kalman.o \
./Src/framework/frameSrc/myPID.o \
./Src/framework/frameSrc/myUsartFunction.o \
./Src/framework/frameSrc/pressureRegulator_ITV2030.o \
./Src/framework/frameSrc/trajectoryGeneration.o \
./Src/framework/frameSrc/valveFlowFunc.o 

C_DEPS += \
./Src/framework/frameSrc/ADBoard.d \
./Src/framework/frameSrc/Controller.d \
./Src/framework/frameSrc/DABoard.d \
./Src/framework/frameSrc/PWMBoardI2C.d \
./Src/framework/frameSrc/PWMBoardSPI.d \
./Src/framework/frameSrc/builtInAnalog.d \
./Src/framework/frameSrc/builtInDigital.d \
./Src/framework/frameSrc/builtInPWM.d \
./Src/framework/frameSrc/kalman.d \
./Src/framework/frameSrc/myPID.d \
./Src/framework/frameSrc/myUsartFunction.d \
./Src/framework/frameSrc/pressureRegulator_ITV2030.d \
./Src/framework/frameSrc/trajectoryGeneration.d \
./Src/framework/frameSrc/valveFlowFunc.d 

CPP_DEPS += \
./Src/framework/frameSrc/PneuFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
Src/framework/frameSrc/%.o: ../Src/framework/frameSrc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__FPU_PRESENT=1U' -DARM_MATH_CM7 -DCIFX_TOOLKIT_HWIF '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Inc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/framework/frameInc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/framework/PneumaticElements" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/User" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/Include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/framework/frameSrc/%.o: ../Src/framework/frameSrc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__FPU_PRESENT=1U' -DARM_MATH_CM7 -DCIFX_TOOLKIT_HWIF '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Inc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/framework/frameInc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/framework/PneumaticElements" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Src/User" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32WorkSpaceOnedrive/PneuDrive/Drivers/CMSIS/Include"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


