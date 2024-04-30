################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/leon3_hw_irqs.c 

C_DEPS += \
./edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/leon3_hw_irqs.d 

OBJS += \
./edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/leon3_hw_irqs.o 


# Each subdirectory must supply rules for building sources it contributes
edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/%.o: ../edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/%.c edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	sparc-rtems-gcc -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroom_glue/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroombp/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroomsl/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/rtems_osswr/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/leon3_can_drv/include" -O0 -g3 -Wall -c -fmessage-length=0 -msoft-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6-2f-leon3

clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6-2f-leon3:
	-$(RM) ./edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/leon3_hw_irqs.d ./edroombp/src/rtemsapi_4_6/rtems_4_6/leon3/leon3_hw_irqs.o

.PHONY: clean-edroombp-2f-src-2f-rtemsapi_4_6-2f-rtems_4_6-2f-leon3

