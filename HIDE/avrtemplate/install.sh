sudo avrdude -P/dev/ttyUSB0 -cwiring -patmega2560 -b115200 -D -Uflash:w:./gen

sudo gtkterm -p /dev/ttyUSB0