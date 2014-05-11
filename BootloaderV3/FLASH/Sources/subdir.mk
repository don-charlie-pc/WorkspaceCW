################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/SerialManager.c" \
"../Sources/bootloader.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/SerialManager.c \
../Sources/bootloader.c \
../Sources/main.c \

OBJS += \
./Sources/SerialManager_c.obj \
./Sources/bootloader_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/SerialManager_c.obj" \
"./Sources/bootloader_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/SerialManager_c.d \
./Sources/bootloader_c.d \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/SerialManager_c.d" \
"./Sources/bootloader_c.d" \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/SerialManager_c.obj \
./Sources/bootloader_c.obj \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/SerialManager_c.obj: ../Sources/SerialManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/SerialManager.args" -ObjN="Sources/SerialManager_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/bootloader_c.obj: ../Sources/bootloader.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/bootloader.args" -ObjN="Sources/bootloader_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


