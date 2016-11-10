################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../testlibs/Arduino.cpp \
../testlibs/Ethernet2.cpp \
../testlibs/Logging.cpp \
../testlibs/MockComponent.cpp \
../testlibs/MockDevice.cpp \
../testlibs/MockEventBus.cpp \
../testlibs/MockEventHandler.cpp 

OBJS += \
./testlibs/Arduino.o \
./testlibs/Ethernet2.o \
./testlibs/Logging.o \
./testlibs/MockComponent.o \
./testlibs/MockDevice.o \
./testlibs/MockEventBus.o \
./testlibs/MockEventHandler.o 

CPP_DEPS += \
./testlibs/Arduino.d \
./testlibs/Ethernet2.d \
./testlibs/Logging.d \
./testlibs/MockComponent.d \
./testlibs/MockDevice.d \
./testlibs/MockEventBus.d \
./testlibs/MockEventHandler.d 


# Each subdirectory must supply rules for building sources it contributes
testlibs/%.o: ../testlibs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/Volumes/Storage/rag/Documents/workspace/shuttercontroltest/src" -I"/Volumes/Storage/rag/Documents/workspace/shuttercontroltest/testlibs" -I"/Volumes/Storage/rag/Documents/workspace/shuttercontrol/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


