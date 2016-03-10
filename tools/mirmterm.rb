#!/usr/bin/env ruby
#coding: utf-8
require 'rubygems'  
require 'serialport' # use Kernel::require on windows, works better.  

#params for serial port  
port_str = "/dev/ttyUSB0"  #may be different for you  
baud_rate = 9600  
data_bits = 8  
stop_bits = 1  
parity = SerialPort::NONE  

sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)  

i = 15

#just write forever  
while true do  
  sp.write(i.to_s(2))
  puts sp.read()


  sleep 1
end