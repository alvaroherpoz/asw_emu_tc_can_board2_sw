################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include swpackages/pi_free_can_drv/src/subdir.mk
-include swpackages/leon3_occan_drv/src/subdir.mk
-include swpackages/leon3_hw_irqs/src/subdir.mk
-include swpackages/icuasw_pus_services_slib_head/src/subdir.mk
-include swpackages/emu_tc_programming/src/subdir.mk
-include swpackages/datapool_mng_slib_head/src/subdir.mk
-include swpackages/can_queue/src/subdir.mk
-include edroomsl_types/src/subdir.mk
-include edroomsl/src/subdir.mk
-include edroombp/src/rtemsapi_4_6/rtems_4_6/subdir.mk
-include edroom_glue/src/subdir.mk
-include dataclasses/CDSensorTMBufferStatus/src/subdir.mk
-include components/rcctm_channelctrl/src/subdir.mk
-include components/rccicuasw/src/subdir.mk
-include components/rccepdmanager/src/subdir.mk
-include components/cchk_fdirmng/src/subdir.mk
-include components/ccbkgtcexec/src/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := asw_emu_tc_can_board2_sw
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: asw_emu_tc_can_board2_sw

# Tool invocations
asw_emu_tc_can_board2_sw: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	sparc-rtems-g++ -msoft-float -o "asw_emu_tc_can_board2_sw" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) asw_emu_tc_can_board2_sw
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
