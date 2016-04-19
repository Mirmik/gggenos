sudo avrdude -P/dev/ttyUSB0 -cwiring -patmega2560 -b115200 -D -Uflash:w:./genos

sudo gtkterm -p /dev/ttyUSB0 -s 115200