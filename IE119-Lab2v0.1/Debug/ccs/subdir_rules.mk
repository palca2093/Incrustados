################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ccs/startup_msp432p401r_ccs.obj: ../ccs/startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/esteban/workspace_v7/IE119-Lab2v0.1" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/third_party/CMSIS/Include" --include_path="/home/esteban/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --include_path="/home/esteban/workspace_v7/IE119-Lab2v0.1/driverlib/MSP432P4xx" --include_path="/home/esteban/workspace_v7/IE119-Lab2v0.1/LcdDriver" --include_path="/home/esteban/ti/simplelink_msp432p4_sdk_2_10_00_14/source/ti/grlib" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="ccs/startup_msp432p401r_ccs.d_raw" --obj_directory="ccs" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

