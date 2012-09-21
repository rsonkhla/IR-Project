################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HeapDocScore.cpp \
../IREngine.cpp \
../Parameters.cpp \
../PostingsValue.cpp \
../QuerryProcessor.cpp \
../main.cpp \
../searchindex.cpp 

OBJS += \
./HeapDocScore.o \
./IREngine.o \
./Parameters.o \
./PostingsValue.o \
./QuerryProcessor.o \
./main.o \
./searchindex.o 

CPP_DEPS += \
./HeapDocScore.d \
./IREngine.d \
./Parameters.d \
./PostingsValue.d \
./QuerryProcessor.d \
./main.d \
./searchindex.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


