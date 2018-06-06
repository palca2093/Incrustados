################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Tasks/LED.obj: ../Tasks/LED.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/Escritorio/Incrustados-master/IE119-Lab2" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Tasks/LED.d_raw" --obj_directory="Tasks" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Tasks/LINE_COUNT.obj: ../Tasks/LINE_COUNT.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/Escritorio/Incrustados-master/IE119-Lab2" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Tasks/LINE_COUNT.d_raw" --obj_directory="Tasks" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Tasks/SCREEN_UPDATE.obj: ../Tasks/SCREEN_UPDATE.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/Escritorio/Incrustados-master/IE119-Lab2" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Tasks/SCREEN_UPDATE.d_raw" --obj_directory="Tasks" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Tasks/UPDATE_ADC.obj: ../Tasks/UPDATE_ADC.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/Escritorio/Incrustados-master/IE119-Lab2" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Tasks/UPDATE_ADC.d_raw" --obj_directory="Tasks" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


