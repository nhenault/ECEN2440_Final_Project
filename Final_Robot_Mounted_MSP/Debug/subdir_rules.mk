################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
circbuf_final.obj: ../circbuf_final.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="circbuf_final.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

functions_final.obj: ../functions_final.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="functions_final.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

port_final.obj: ../port_final.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="port_final.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer_final.obj: ../timer_final.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="timer_final.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_final.obj: ../uart_final.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ti/ccsv7/ccs_base/arm/include" --include_path="D:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="D:/Nathan/Workspace/Final_Robot_Mounted_MSP" --include_path="D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="uart_final.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


