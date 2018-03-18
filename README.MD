# KbdScan

## Description
This is source code for a loadable kernel module. The module implements a character device driver, which will be installed as __/dev/kbdscan__.
Please note that the module will only work on **Raspberry Pi** modules, due to the implemented hardware GPIO register access. Adaptation to a
different hardware can be done by modifying the **mygpio.c** functions.

## How to install the driver ?
The module can either be installed by calling Â´<make load>Â´. This will not generate the necessary device in the /dev/ folder.
It should better be installed by executing the shell script '<sudo ./load_kbdscan>'.

## How to uninstall the driver ?
The module should be uninstalled by calling `<make unload>`.

## Which parameters can be modified ?

## My use case
I have created this module in order to be able to interface a flight simulator panel. The panel is purchased from Hispapanels.com. It is an
airbus A320 FMC panel. The keys are arranged in an 9x9 matrix. I use the raspberry pi module to read the keys and to pass it to a custom
pÃython script which will display the FMC user interface and send status and requests to the main flight simulator program.

## How does the module work ?
The keys are arrange in a 9x9 matrix, i.e. 9 columns and 9 rows. Everytime a key will be pressed it will connect the column it is attached
to the row it is connected. The module will activate one column at a time (writing a '1' to the column pin) and read all the 9 row inputs.
If it detects one of the rows inÃputs to be on high level (i.e. a '1'), it knows that the corresponding key (column, row) has been pressed.
After one full scan is complete (going through all available columns), the module starts the scan from column 0 again.

## Limitations
* The module in its current state will not be able to work properly in case more than one key is pressed.
* The timing is not yet optimal. Due to the selected schedule, the keys may be detected only after a longer key press.
* There is currently no debouncing implemented, which may lead to incorrectly detected keys
