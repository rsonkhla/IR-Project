################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CommonSettingsAndUtilities.cpp \
../DataCollector.cpp \
../DataTypeDetector.cpp \
../Dictionary.cpp \
../DictionaryHashMap.cpp \
../HashMapImplementation.cpp \
../Indexer.cpp \
../InvertedIndexGenerator.cpp \
../InvertedIndexHashMap.cpp \
../PostingsValue.cpp \
../TextParser.cpp \
../TimeAnalysis.cpp \
../WikiParser.cpp \
../fwIndexer.cpp \
../main.cpp \
../merger.cpp \
../parameters.cpp \
../posting.cpp \
../reader.cpp 

OBJS += \
./CommonSettingsAndUtilities.o \
./DataCollector.o \
./DataTypeDetector.o \
./Dictionary.o \
./DictionaryHashMap.o \
./HashMapImplementation.o \
./Indexer.o \
./InvertedIndexGenerator.o \
./InvertedIndexHashMap.o \
./PostingsValue.o \
./TextParser.o \
./TimeAnalysis.o \
./WikiParser.o \
./fwIndexer.o \
./main.o \
./merger.o \
./parameters.o \
./posting.o \
./reader.o 

CPP_DEPS += \
./CommonSettingsAndUtilities.d \
./DataCollector.d \
./DataTypeDetector.d \
./Dictionary.d \
./DictionaryHashMap.d \
./HashMapImplementation.d \
./Indexer.d \
./InvertedIndexGenerator.d \
./InvertedIndexHashMap.d \
./PostingsValue.d \
./TextParser.d \
./TimeAnalysis.d \
./WikiParser.d \
./fwIndexer.d \
./main.d \
./merger.d \
./parameters.d \
./posting.d \
./reader.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


