@ECHO OFF
set /p COMPORT=Enter the port of the bootloader 
avrdude -p atmega32u4 -P %COMPORT% -c avr109 -b 9600 -U flash:w:HummingbirdV2.hex
PAUSE