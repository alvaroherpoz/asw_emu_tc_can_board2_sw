################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../edroombp/src/rtemsapi_4_6/rtems_4_6/edroombp.cpp 

CPP_DEPS += \
./edroombp/src/rtemsapi_4_6/rtems_4_6/edroombp.d 

OBJS += \
./edroombp/src/rtemsapi_4_6/rtems_4_6/edroombp.o 


# Each subdirectory must supply rules for building sources it contributes
edroombp/src/rtemsapi_4_6/rtems_4_6/%.o: ../edroombp/src/rtemsapi_4_6/rtems_4_6/%.cpp edroombp/src/rtemsapi_4_6/rtems_4_6/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	sparc-rtems-g++ -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/config/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/leon3_hw_irqs/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/can_queue/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/pi_free_can_drv/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/leon3_occan_drv/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDEventList/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDRecovAction/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDSensorTMBufferStatus/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTCDescriptor/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTMList/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/dataclasses/CDTMMemory/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroom_glue/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroombp/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/ccbkgtcexec/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/cchk_fdirmng/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rccepdmanager/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rccicuasw/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/components/rcctm_channelctrl/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/datapool_mng_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/icuasw_pus_services_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/swinterfaces/sc_channel_drv_iface_v1/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroomsl/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/edroomsl_types/include" -I"/home/atcsol/eclipse-workspace/asw_emu_tc_can_board2_sw/rtems_osswr/include" -O0 -g3 -Wall -c -fmessage-length=0 -msoft-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6

clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6:
	-$(RM) ./edroombp/src/rtemsapi_4_6/rtems_4_6/edroombp.d ./edroombp/src/rtemsapi_4_6/rtems_4_6/edroombp.o

.PHONY: clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6

