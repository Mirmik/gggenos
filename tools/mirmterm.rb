#!/usr/bin/env ruby
#coding: utf-8

if ARGV.count != 1 then
	puts "wrong parametr count"
	exit
end



input_term = File.open(ARGV[0], 'r')
output_term = File.open(ARGV[0], 'w')

puts $stdin.stat.zero?

input_term.close
output_term.close
