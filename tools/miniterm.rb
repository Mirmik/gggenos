#!/usr/bin/env ruby
#coding: utf-8

require "serialport"

#params for serial port  
port_str = "/dev/ttyUSB0"  #may be different for you  
baud_rate = 9600  
data_bits = 8  
stop_bits = 1  
parity = SerialPort::EVEN

sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)  

sleep 1

open("/dev/tty", "r+") { |tty|
  tty.sync = true
  Thread.new {
    while true do
      tty.printf("%c", sp.getc)
    end
  }
  #while (l = tty.gets) do
    #sp.write(l.sub("\n", "\r"))

    message = "x201x00xF8";
    #array = message.unpack
    message[0] = 1.chr
    message[4] = 2.chr
    message[7] = 3.chr
    
    #puts message
#while true do
    sp.write(message);
    sleep 1
    puts
   sp.write(message);
    sleep 1
    puts
   sp.write(message);
    sleep 1
    puts
   sp.write(message);
    sleep 1
    puts
#
#end
  #end
}

sp.close