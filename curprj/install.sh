#sudo avrdude -P/dev/ttyACM0 -cwiring -patmega2560 -b115200 -D -Uflash:w:./genos

#sudo gtkterm -p /dev/ttyACM0 -s115200

arm-none-eabi-objcopy -O binary genos genos.bin
sudo st-flash write genos.bin 0x8000000
sudo gtkterm -p /dev/ttyUSB0 -s 38400