################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Light_sensor/HAL_I2C.obj: ../Light_sensor/HAL_I2C.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/workspace_v7/IE119-Lab1" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Light_sensor/HAL_I2C.d_raw" --obj_directory="Light_sensor" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Light_sensor/HAL_OPT3001.obj: ../Light_sensor/HAL_OPT3001.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/workspace_v7/IE119-Lab1" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Light_sensor/HAL_OPT3001.d_raw" --obj_directory="Light_sensor" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


