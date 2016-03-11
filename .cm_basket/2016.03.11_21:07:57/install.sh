sudo avrdude -P/dev/ttyACM0 -cwiring -patmega2560 -b115200 -D -Uflash:w:./genos

sudo gtkterm -p /dev/ttyACM0