# Tutorial : Buttons and LEDs

In this tutorial, we are going to create a software application to run along our hardware design generated from Vivado.

## Prerequisites

1. Vitis 2022.1 accessible
2. Hardware XSA file from Vivado

### 1. Launch Vitis

If launching from the command line, Vitis can be launched as follows:
```sh
bash> vitis -workspace <PATH_TO_DESIRED_WORKSPACE_DIR> &> /dev/null
```

The above command assumes BASH shell is used and performs the follwoing:
- redirects both standard and error output streams to /dev/null (essentially turning off outputs on the console)
- sets the workspace directory for Vitis to the value provided for `<PATH_TO_DESIRED_WORKSPACE_DIR>` (if current directory is used, `<PATH_TO_DESIRED_WORKSPACE_DIR>` can be set to `.`)

The following window should open up

![](./images/01_Launch_Vitis.png)

### 2. Create Platform Project

The first thing we need to do in Vitis is create a software platform project. A software platform contains all the necessary software components needed to run our application. These components can include:
- operating system (e.g Linux, RTOS)
- low-level drivers for IP integrated into the hardware platform (created from Vivado)
- libraries (e.g. filesystem)

- go to `File` menu and click on `File > New > Platform Project...` as shown below

![](./images/02_1_Create_PFM.png)

- in the `Create new platform project` window that opens up, set the `Platform project name` to `btns_leds` and click `Next` as shown below

![](./images/02_2_Create_PFM.png)

- in the `Platform` window, select the `Create a new platform from hardware(XSA)` tab if not selected
- in the `Hardware Specification` pane, browse for the `XSA file` (find the `system.xsa` file generated from Vivado)
- in the `Software Specification` pane,
    - set `Operating System` to `standalone`
    - set `Processor` to `psu_cortexa53_0`
    - set 'Architecture' to `64-bit`
- in the `Boot Components` pane,
    - check the `Generate boot components` checkbox
    - select `psu_cortexa53_0` for `Target processor to create FSBL` (FSBL stands for first-stage boot loader)
    - click `Finish`

![](./images/02_3_Create_PFM.png)

- after a little while, the software platform should be created. Now, click on the hammer tool in the toolbar to build the platform as shown below

![](./images/02_4_Create_PFM.png)

- when the build is successful, we should get a message within the Console at the bottom of the window as shown below

![](./images/02_5_Create_PFM.png)

### 3. Create Application Project

With the software platform created, now we can create an application to run on it.

- go to `File` menu and click on `File > New > Application Project...` as shown below

![](./images/03_1_Create_App.png)

- Click `Next` on the `Create a New Application Project` window that pops up (as shown below)

![](./images/03_2_Create_App.png)

- in the `Platform` window that opens up:
    - select the `Select a platform for repository` tab
    - select the `btns_leds` platform created in previous steps
    - click `Next`

![](./images/03_3_Create_App.png)

-  in the `Application Project Details` window
    - set `Application project name` to `btns_leds_app`
    - make sure the `psu_cortexa53_0` and associated application `btns_leds_app` are selected in the `Target processor` pane as shown below
    - click `Next`

![](./images/03_4_Create_App.png)

- in the `Domain` window, select `standalone on psu_cortexa53_0` as a domain and click `Next` as shown below

![](./images/03_5_Create_App.png)

- in the `Templates` window, select `Empty Application(C)` as shown below
- click `Finish`

![](./images/03_6_Create_App.png)

- in the `Explorer` window tab, right-click on `btns_leds_app_system > btns_leds_app > src`
- in the context menu that pops up, click on `New > File` as shown below

![](./images/03_7_Create_App.png)

- in the `Create New File` window, select `btns_leds_app > src` as shown below
- set `File name` to `main.c`
- click `Finish`

![](./images/03_8_Create_App.png)

- a new `main.c` file should now be added to `btns_leds_app_system > btns_leds_app > src` folder within the `Explorer` window
- double-click on `main.c` to open it in the editor (if not already open)
- add the code shown below to the `main.c` file

![](./images/03_9_Create_App.png)

- build the application by clicking on the hammer tool in the toolbar as shown below

![](./images/03_10_Create_App.png)

- once the application is successfully built, we should get the following in the Console window

![](./images/03_11_Create_App.png)

### 4. Create Boot Image

There are a few ways in which we can boot up the physical FPGA device/board. In this case, we will use the SD-Card method. Essentially, we need to prepare an image file that contains the different bootloaders, FPGA bitstream, and our application. This image file would then be placed in a FAT partition of an SD-card.

- go to `Xilinx` menu and click on `Xilinx > Create Boot Image > Zynq and Zynq Ultrascale` as shown below

![](./images/04_1_Create_Boot.png)

- in the `Create Boot Image` window:
    - set `Architecture` to `Zynq MP`
    - select `Create new BIF file`
    - in the `Basic` tab:
        - select a convenient output path for the image specification file (.bif file)
        - select a convenient output path for the image file (BOOT.bin file)

![](./images/04_2_Create_Boot.png)

- then, in the `Boot image partitions` pane, click `Add` as shown above
- for `File path` in the `Add new boot image partition` window, navigate to `btns_leds > export > btns_leds > sw > btns_leds > boot` and select `fsbl.elf` and click `Open` as shown below

![](./images/04_3_Create_Boot.png)

- then:
    - select `bootloader` for `Partition type`
    - set `Destination Device` to `PS`
    - set `Destination CPU` to `A53 0`
    - and click `OK` as shown below

![](./images/04_4_Create_Boot.png)

- click `Add` in the `Boot image partitions` again as shown below:

![](./images/04_5_Create_Boot.png)

- for `File path`, navigate to `btns_leds > export > btns_leds > sw > btns_leds > boot` and select `pmufw.elf` and click `Open` as shown below:

![](./images/04_6_Create_Boot.png)

- then back on the `Add Partition` window:
    - set `Partition type` to `pmu(loaded by bootroom)`
    - click `OK` as shown below

![](./images/04_7_Create_Boot.png)

- click `Add` in the `Boot image partitions` again as shown below:

![](./images/04_8_Create_Boot.png)

- for `File path`, navigate to `btns_leds > export > btns_leds > hw` and select `system.bit` and click `Open` as shown below:

![](./images/04_9_Create_Boot.png)

- then back on the `Add Partition` window:
    - set `Partition type` to `datafile`
    - set `Destination Device` to `PL`
    - click `OK` as shown below

![](./images/04_10_Create_Boot.png)

- click `Add` in the `Boot image partitions` again as shown below:

![](./images/04_11_Create_Boot.png)

- for `File path`, navigate to `btns_leds_app > Debug` and select `btns_leds_app.elf`
- click `Open` as shown below

![](./images/04_12_Create_Boot.png)

- then back on the `Add Partition` window:
    - set `Partition type` to `datafile`
    - set `Destination Device` to `PS`
    - set `Destination CPU` to `A53 0`
    - click `OK` as shown below

![](./images/04_13_Create_Boot.png)

- then back on the `Create Boot Image` window, click `Create Image` as shown below

![](./images/04_14_Create_Boot.png)

- once boot image is built, Vitis should show a success message (as shown below) in the Console

![](./images/04_15_Create_Boot.png)

- Finally, there should be a `BOOT.bin` file generated under the working directory as shown below

![](./images/04_16_Create_Boot.png)
