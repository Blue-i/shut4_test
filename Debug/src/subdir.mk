################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EventManagerTests.cpp \
../src/Main.cpp \
../src/ProjectorTests.cpp \
../src/ShutterLEDControllerTests.cpp \
../src/ShutterTests.cpp 

OBJS += \
./src/EventManagerTests.o \
./src/Main.o \
./src/ProjectorTests.o \
./src/ShutterLEDControllerTests.o \
./src/ShutterTests.o 

CPP_DEPS += \
./src/EventManagerTests.d \
./src/Main.d \
./src/ProjectorTests.d \
./src/ShutterLEDControllerTests.d \
./src/ShutterTests.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/Volumes/Storage/rag/Documents/workspace/shuttercontroltest/src" -I"/Volumes/Storage/rag/Documents/workspace/shuttercontroltest/testlibs" -I"/Volumes/Storage/rag/Documents/workspace/shuttercontrol/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


