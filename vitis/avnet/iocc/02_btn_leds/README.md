# Exercise: Buttons, LEDs, and Timer

Learning objectives:
- setting up interrupts in standalone domain
- accessing hardware in standalone domain
- using Xilinx low-level drivers in standalone domain

## 1. Prompt

Write a software application that accomplishes the following:
- sets up timer to fire every 1 second
- turn on each of the following LED one at a time in the order listed: D7, D8, D11, D12, D13, D14, D16, and D17
- once all LEDs are turned ON, turn them off at the next timer tick and then repeat the pattern again
- use pushbutton `SW1` to decrease the tick period by 250ms (minimum tick period = 250ms)
- use pushbutton `SW2` to increase the tick period by 250ms (maximum tick period = 2s)
- every time a button push event occurs (either `SW1` or `SW2`), print a small message with information about which button is pressed to the console (using `xil_printf`)

> Note: Debounce the pushbuttons such that a PUSH-RELEASE sequence counts as 1 push event (independent of how long the button is pressed)


## 2. Help / Resources

This section provides a few hints/notes/links that can help solve the assigned problem.

### 2.1 Xilinx IP Standalone Drivers and Examples

Many Xilinx IP such as the GPIO and AXI Timer come with baremetal (standalone) drivers and examples on how they can be used.

- for GPIO, browse the following links:
    - examples: https://github.com/Xilinx/embeddedsw/tree/master/XilinxProcessorIPLib/drivers/gpiops/examples
    - driver source code: https://github.com/Xilinx/embeddedsw/tree/master/XilinxProcessorIPLib/drivers/gpiops/src
- for AXI Timer, browse the following links:
    - examples: https://github.com/Xilinx/embeddedsw/tree/master/XilinxProcessorIPLib/drivers/tmrctr/examples
    - driver source code: https://github.com/Xilinx/embeddedsw/tree/master/XilinxProcessorIPLib/drivers/tmrctr/src

### 2.2 GIC Interrupt Controller

The A53 cores on the ZynqUlstraScale use the ARM Generic Interrupt Controller for managing interrupts.

An example of how to set up the GIC in standalone mode can be found here: https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib/drivers/scugic/examples/xscugic_tapp_example.c. Together with interrupt-based examples for the other IP (e.g. AXI Timer) should provide enough information how it works and how to use it.

### 2.3 Button De-Bouncing

I'll leave it to these two articles to explain the bouncing phenomenon and examples of de-bouncing schemes:
- https://www.thegeekpub.com/246471/debouncing-a-switch-in-hardware-or-software/
- https://digilent.com/reference/learn/microprocessor/tutorials/debouncing-via-software/start

In our case, we will use software debouncing. The task is, therefore, to write debouncing code for the `SW1` and `SW2` push buttons alongside our standalone application.

### 2.4 Device IDs, Addresses, and Interrupt IDs

Each IP instantiated in the hardware design gets assigned an ID. This is called a `Device ID`.
The `xparameters.h` include file is automatically generated during software platform creation in Vitis and contains information regarding Device IDs.

The nomenclature for Device ID is typically `XPAR_<IP_NAME>_<IP_INSTANCE_NUMBER>_DEVICE_ID` (all caps) where:
- `<IP_NAME>` is the name of the IP (e.g. AXI_GPIO(or GPIO in canonical form))
- `<IP_INSTANCE_NUMBER>` this is 0 for the first instantiated IP and incrementally add 1 for each additional instance.

For example, in the case of our AXI GPIO used to connect LEDs and push buttons, the Device ID = `XPAR_GPIO_0_DEVICE_ID` or (XPAR_AXI_GPIO_0_DEVICE_ID)

This is also true for device base address (`XPAR_GPIO_0_BASEADDR`) if the device is allocated an address during hardware construction.

For AXI Timer, we have:
- Device ID = `XPAR_AXI_TIMER_0_DEVICE_ID` (or the canonical `XPAR_TMRCTR_0_DEVICE_ID`)
- Base Address = `XPAR_AXI_TIMER_0_BASEADDR` (or `XPAR_TMRCTR_0_BASEADDR`)

As for `Interrupt IDs`:
- we have `XPAR_FABRIC_<IP_NAME>_<IP_INSTANCE_NUMBER>_INTERRUPT_INTR`
- or the canonical `XPAR_FABRIC_<IP_CANONICAL_NAME>_<IP_INSTANCE_NUMBER>_VEC_ID` (e.g. `XPAR_FABRIC_TMRCTR_0_VEC_ID`)

> Note: Most driver APIs will use these IDs during initialization. Therefore, it is important to understand this nomenclature.
> A search through the xparameters.h file should also show more information collected for a particular IP
