# KbdScan

## Description
This is source code for a loadable kernel module. The module implements a character device driver, which will be installed as __/dev/kbdscan__.
Please note that the module will only work on **Raspberry Pi** modules, due to the implemented hardware GPIO register access. Adaptation to a
different hardware can be done by modifying the **mygpio.c** functions.

## How to install the driver ?
The module can either be installed by calling `make load`. This will not generate the necessary device in the /dev/ folder. You will have to
create the device manually. 
Alternatively use the script provided in the same folder which will do everything needed after building `sudo ./load_kbdscan`.

## How to build the module
Simply call `make all` and it will generate the kernel module object for you. The kernel module object is __kbdscan.ko__.

## How to uninstall the driver ?
The module should be uninstalled by calling `make unload`.

## Which parameters can be modified ?
There are two kinds of paramters for the module. The ones that are static and need to be provided when loading the kernel module and those which
are dynamic and can be modified by changing the inside the /sys/kbdscan/matrix0/ folder.

### Static parameters
* __gpioCols__   - **array of integers** - an array of GPIO pins to use as inputs for the keyboard matrix
* __gpioRows__   - **array of integers** - an array of GPIO pins to use as outputs for the keyboard matrix, make sure a pull-down resistor is
available either as internal or external devices.

### Dynamic parameters and variables
* __currentRow__ - **read-write** - the current row being checked for a key press
* __keyNum__     - **read-only**  - the currently detected key code
* __singleLine__ - **read-write** - activate single-row scan mode with a 1, activate automatic scan mode with a 0

## My use case
I have created this module in order to be able to interface a flight simulator panel. The panel is purchased from Hispapanels.com. It is an
airbus A320 FMC panel. The keys are arranged in an 9x9 matrix. I use the raspberry pi module to read the keys and to pass it to a custom
python script which will display the FMC user interface and send status and requests to the main flight simulator program.

## How does the module work ?
The keys are arrange in a 9x9 matrix, i.e. 9 columns and 9 rows. Everytime a key will be pressed it will connect the column it is attached
to the row it is connected. The module will activate one row at a time (writing a '1' to the row pin) and read all the 9 column inputs.
If it detects one of the columns inputs to be on high level (i.e. a '1'), it knows that the corresponding key (column, row) has been pressed.
After one full scan is complete (going through all available rows), the module starts the scan from row 0 again.

## Limitations
* The module in its current state will not be able to work properly in case more than one key is pressed.
* The timing is not yet optimal. Due to the selected schedule, the keys may be detected only after a longer key press.
* There is currently no debouncing implemented, which may lead to incorrectly detected keys
