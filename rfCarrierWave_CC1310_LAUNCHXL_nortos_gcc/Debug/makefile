################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/gcc-arm-none-eabi_7_2_1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./CC1310_LAUNCHXL.o" \
"./CC1310_LAUNCHXL_fxns.o" \
"./ccfg.o" \
"./main_nortos.o" \
"./rfCarrierWave.o" \
"./smartrf_settings/smartrf_settings.o" \
-Wl,-T"../CC1310_LAUNCHXL_NoRTOS.lds" \
$(GEN_CMDS__FLAG) \
-l:"ti/display/lib/display.am3g" \
-l:"ti/grlib/lib/gcc/m3/grlib.a" \
-l:"third_party/spiffs/lib/gcc/m3/spiffs_cc26xx.a" \
-l:"ti/drivers/rf/lib/rf_multiMode_cc13x0.am3g" \
-l:"ti/drivers/lib/drivers_cc13x0.am3g" \
-l:"lib/nortos_cc13x0.am3g" \
-l:"ti/devices/cc13x0/driverlib/bin/gcc/driverlib.lib" \
-lgcc \
-lc \
-lm \
-lnosys \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include smartrf_settings/subdir_vars.mk
-include subdir_rules.mk
-include smartrf_settings/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.out 

EXE_OUTPUTS__QUOTED += \
"rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.out" 


# All Target
all: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.out"

# Tool invocations
rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.out: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -DDeviceFamily_CC13X0 -DCCFG_FORCE_VDDR_HH=0 -DSUPPORT_PHY_CUSTOM -DSUPPORT_PHY_50KBPS2GFSK -DSUPPORT_PHY_625BPSLRM -DSUPPORT_PHY_5KBPSSLLR -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -march=armv7-m -mthumb -Wl,-Map,"rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.map" --entry resetISR -nostartfiles -static -Wl,--gc-sections -L"C:/ti/simplelink_cc13x0_sdk_4_20_02_07/source" -L"C:/ti/simplelink_cc13x0_sdk_4_20_02_07/kernel/nortos" -L"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/lib/thumb/v7-m" -L"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/lib" -march=armv7-m -mthumb --specs=nano.specs -o"rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "CC1310_LAUNCHXL.o" "CC1310_LAUNCHXL_fxns.o" "ccfg.o" "main_nortos.o" "rfCarrierWave.o" "smartrf_settings\smartrf_settings.o" 
	-$(RM) "CC1310_LAUNCHXL.d" "CC1310_LAUNCHXL_fxns.d" "ccfg.d" "main_nortos.d" "rfCarrierWave.d" "smartrf_settings\smartrf_settings.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

