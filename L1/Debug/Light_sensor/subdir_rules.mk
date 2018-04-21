################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Light_sensor/HAL_I2C.obj: ../Light_sensor/HAL_I2C.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/ti/ccsv7/ccs_base/arm/include" --include_path="/home/esteban/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="/home/esteban/workspace_v7/L1" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Light_sensor/HAL_I2C.d_raw" --obj_directory="Light_sensor" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Light_sensor/HAL_OPT3001.obj: ../Light_sensor/HAL_OPT3001.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/ti/ccsv7/ccs_base/arm/include" --include_path="/home/esteban/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="/home/esteban/workspace_v7/L1" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Light_sensor/HAL_OPT3001.d_raw" --obj_directory="Light_sensor" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


