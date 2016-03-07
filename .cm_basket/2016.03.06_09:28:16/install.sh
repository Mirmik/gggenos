sudo avrdude -P /dev/ttyUSB0 -carduino -p atmega328p -b57600 -D -Uflash:w:./genos
sudo gtkterm -p /dev/ttyUSB0