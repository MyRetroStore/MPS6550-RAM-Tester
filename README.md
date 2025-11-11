# MPS6550 RAM Tester

Arduino shield to test MPS6550 RAM chips used in the Commodore PET.

## Introduction
Modern EPROM programmers like the TL866 are unable to read obsolete MPS6550 RAM chips used in Commodore PET's.

The MPS6550 is a custom  1024 x 4 bit RAM chip manufactured by Commodore. 

I built a shield for the Arduino Mega 2560 which runs 4 tests:
* Writes B1111 to each memory address and verifies
* Writes B0000 to each memory address and verified
* Writes alternate B0101 and B1010 to address and verifies
* Writes incremental data to each address and verifies
 

## Usage
This Arduino sketch was built and tested using Arduino IDE 1.8.19 but should work with later versions.
There is a library dependency for DIO2

Compile and upload the code. 
Remove power and insert the MPS6550 chip to be tested. Take note of the orientation (Pin 1)

Connect power and open the serial monitor.  The output from the tests will be written to the serial port (Baud 115200)

The red/green LED's will flicker inducating the tests are in progress. 
At the end of the test if the RAM passes the green LED will be enabled. 

Any errors will be displayed on the serial monitor, andf the red LED will be enabled. 

![Serial Output](https://raw.githubusercontent.com/MyRetroStore/MPS6550-RAM-Tester/main/hardware/mps6550.png)

![Serial Output](https://raw.githubusercontent.com/MyRetroStore/MPS6550-RAM-Tester/main/hardware/mps6550-serial.png)

## Shield

The gerber files for the shield can be found under hardware/gerbers

![Retro ROM Tester](https://github.com/MyRetroStore/MPS6550-RAM-Tester/blob/main/hardware/mps6550-hat.png?raw=true)

## BOM
| Reference | Value |
| ------ | ------ |
| PASS | Green LED 0805 |
| FAIL | Red LED 0805 |
| R1, R2 | 1k Resistor 0805 |
| U1 | 2.54mm round female sockkets |
| SW1 | Push Button Switch 6x6x6 |
| F1 | 100mA 0805 Poly Fuse |
| | 2.54 Male SIL Headers |

## Purchasing

If you don't want build the shield yourself, assembled shields can be purchased from:
- [MyRetroStore](https://myretrostore.co.uk/)
- [eBay](https://www.ebay.co.uk/str/myretrostoreuk)

## License
CC BY-NC-SA 4.0


[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H8RDX9W)

