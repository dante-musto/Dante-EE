# Smart-Watch
This project is meant to contain a MSP432, SSD1306, HC05 and a piezo buzzer. These files can be downloaded and edited to print different information
on the screen. Here is a pin map of how to connect the MSP


Board
Pin
Purpose










MSP432
1.6
SCL signal using I2C to control OLED Display
1.7
SDA signal using I2C to control OLED Display
3V3
Used to power DRV2605 on Group PCB
GND
Multiple GND pins - used to ground all boards
4.4
Enable for DRV2605L
6.0
Pin signal that all switches will use to control MSP432 - Will be a High signal for the MSP432
3.2
Input from PCB from buttons to control Modes
3.3
Input from PCB from buttons to control Modes
4.1
Input from PCB from buttons to control Modes
4.3
Input from PCB from buttons to control Modes
6.5
SDA signal using I2C to control DRV2605L
6.4
SCL signal using I2C to control DRV2605L
3V3
Powers Bluetooth module with 3.3V and 50mA
GND
Grounds Bluetooth module
2.5
TXD signal between MSP and HC-05
3.0
RXD signal between MSP and HC-05



SSD1306 OLED Display


VCC
Power from MSP432 3.3V
GND
Grounds OLED display
SCL
I2C communication with MSP432
SDA
I2C communication with MSP432



Datasheets

DRV2605L Datasheet
http://www.ti.com/lit/ds/symlink/drv2605l.pdf
MSP432 Technical Reference Manual
http://www.ti.com/lit/ug/slau356i/slau356i.pdf
HC05 Datasheet
http://www.electronicaestudio.com/docs/istd016A.pdf
SSD1306
https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf






