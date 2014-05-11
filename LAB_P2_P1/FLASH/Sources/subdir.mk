################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ADC.c" \
"../Sources/DelayManager.c" \
"../Sources/GPIO.c" \
"../Sources/LcdManager.c" \
"../Sources/MTIM.c" \
"../Sources/SpeedManager.c" \
"../Sources/StepperMotor.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/ADC.c \
../Sources/DelayManager.c \
../Sources/GPIO.c \
../Sources/LcdManager.c \
../Sources/MTIM.c \
../Sources/SpeedManager.c \
../Sources/StepperMotor.c \
../Sources/main.c \

OBJS += \
./Sources/ADC_c.obj \
./Sources/DelayManager_c.obj \
./Sources/GPIO_c.obj \
./Sources/LcdManager_c.obj \
./Sources/MTIM_c.obj \
./Sources/SpeedManager_c.obj \
./Sources/StepperMotor_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/ADC_c.obj" \
"./Sources/DelayManager_c.obj" \
"./Sources/GPIO_c.obj" \
"./Sources/LcdManager_c.obj" \
"./Sources/MTIM_c.obj" \
"./Sources/SpeedManager_c.obj" \
"./Sources/StepperMotor_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/ADC_c.d \
./Sources/DelayManager_c.d \
./Sources/GPIO_c.d \
./Sources/LcdManager_c.d \
./Sources/MTIM_c.d \
./Sources/SpeedManager_c.d \
./Sources/StepperMotor_c.d \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/ADC_c.d" \
"./Sources/DelayManager_c.d" \
"./Sources/GPIO_c.d" \
"./Sources/LcdManager_c.d" \
"./Sources/MTIM_c.d" \
"./Sources/SpeedManager_c.d" \
"./Sources/StepperMotor_c.d" \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/ADC_c.obj \
./Sources/DelayManager_c.obj \
./Sources/GPIO_c.obj \
./Sources/LcdManager_c.obj \
./Sources/MTIM_c.obj \
./Sources/SpeedManager_c.obj \
./Sources/StepperMotor_c.obj \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/ADC_c.obj: ../Sources/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/ADC.args" -ObjN="Sources/ADC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/DelayManager_c.obj: ../Sources/DelayManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/DelayManager.args" -ObjN="Sources/DelayManager_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/GPIO_c.obj: ../Sources/GPIO.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/GPIO.args" -ObjN="Sources/GPIO_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/LcdManager_c.obj: ../Sources/LcdManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/LcdManager.args" -ObjN="Sources/LcdManager_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/MTIM_c.obj: ../Sources/MTIM.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/MTIM.args" -ObjN="Sources/MTIM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/SpeedManager_c.obj: ../Sources/SpeedManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/SpeedManager.args" -ObjN="Sources/SpeedManager_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/StepperMotor_c.obj: ../Sources/StepperMotor.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/StepperMotor.args" -ObjN="Sources/StepperMotor_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


