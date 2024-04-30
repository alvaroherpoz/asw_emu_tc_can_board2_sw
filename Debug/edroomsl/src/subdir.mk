################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../edroomsl/src/edroomsl.cpp \
../edroomsl/src/edroomtc.cpp 

CPP_DEPS += \
./edroomsl/src/edroomsl.d \
./edroomsl/src/edroomtc.d 

OBJS += \
./edroomsl/src/edroomsl.o \
./edroomsl/src/edroomtc.o 


# Each subdirectory must supply rules for building sources it contributes
edroomsl/src/%.o: ../edroomsl/src/%.cpp edroomsl/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	sparc-rtems-g++ -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/config/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/leon3_hw_irqs/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/can_queue/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/pi_free_can_drv/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/leon3_occan_drv/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDEventList/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDRecovAction/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDSensorTMBufferStatus/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTCDescriptor/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTMList/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTMMemory/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroom_glue/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroombp/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/ccbkgtcexec/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/cchk_fdirmng/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rccepdmanager/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rccicuasw/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rcctm_channelctrl/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/datapool_mng_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/icuasw_pus_services_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/sc_channel_drv_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroomsl/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroomsl_types/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/rtems_osswr/include" -O0 -g3 -Wall -c -fmessage-length=0 -msoft-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-edroomsl-2f-src

clean-edroomsl-2f-src:
	-$(RM) ./edroomsl/src/edroomsl.d ./edroomsl/src/edroomsl.o ./edroomsl/src/edroomtc.d ./edroomsl/src/edroomtc.o

.PHONY: clean-edroomsl-2f-src

