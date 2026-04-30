################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -c -mcpu=cortex-m3 -march=armv7-m -mthumb -mfloat-abi=soft -DDeviceFamily_CC13X0 -DCCFG_FORCE_VDDR_HH=0 -DSUPPORT_PHY_CUSTOM -DSUPPORT_PHY_50KBPS2GFSK -DSUPPORT_PHY_625BPSLRM -DSUPPORT_PHY_5KBPSSLLR -I"C:/Users/jdado/workspace_v12/rfCarrierWave_CC1310_LAUNCHXL_nortos_gcc" -I"C:/ti/simplelink_cc13x0_sdk_4_20_02_07/source" -I"C:/ti/simplelink_cc13x0_sdk_4_20_02_07/kernel/nortos" -I"C:/ti/simplelink_cc13x0_sdk_4_20_02_07/kernel/nortos/posix" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


