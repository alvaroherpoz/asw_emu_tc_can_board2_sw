################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../components/cantxirq/src/CANTxIRQB.cpp \
../components/cantxirq/src/CANTxIRQS.cpp 

CPP_DEPS += \
./components/cantxirq/src/CANTxIRQB.d \
./components/cantxirq/src/CANTxIRQS.d 

OBJS += \
./components/cantxirq/src/CANTxIRQB.o \
./components/cantxirq/src/CANTxIRQS.o 


# Each subdirectory must supply rules for building sources it contributes
components/cantxirq/src/%.o: ../components/cantxirq/src/%.cpp components/cantxirq/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	sparc-rtems-g++ -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/config/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/components/cantxirq/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/dataclasses/CDTxData/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroom_glue/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/leon3_can_drv/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroombp/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroomsl/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/edroomsl_types/include" -I"/home/atcsol/git/edroom_nexys_txirq/edroom_ej_nexys_can_txrirq_23_24/rtems_osswr/include" -O0 -g3 -Wall -c -fmessage-length=0 -msoft-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-components-2f-cantxirq-2f-src

clean-components-2f-cantxirq-2f-src:
	-$(RM) ./components/cantxirq/src/CANTxIRQB.d ./components/cantxirq/src/CANTxIRQB.o ./components/cantxirq/src/CANTxIRQS.d ./components/cantxirq/src/CANTxIRQS.o

.PHONY: clean-components-2f-cantxirq-2f-src

