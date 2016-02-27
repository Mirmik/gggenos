

print('->loading the avr tool')

#from waflib.Configure import conf

def options(opt): 
	#opt.load('compiler_c compiler_cxx')
	pass

def configure(ctx): 	
	ctx.env.ARCH = 'avr'
	ctx.env.CC = ' avr-gcc '
	ctx.env.CXX = ' avr-g++ '	
	ctx.env.LD = ' avr-g++ '	
	ctx.env.CFLAGS = '-std=gnu99  -mmcu=atmega2560 -DF_CPU=16000000 -O1 -fdata-sections -ffunction-sections -Wl,--gc-sections'
	ctx.env.CXXFLAGS = ' -mmcu=atmega2560 -DF_CPU=16000000 -fno-unwind-tables -frtti -fno-exceptions -std=c++11 -O1 -fdata-sections -ffunction-sections -Wl,--gc-sections'
	
	#ctx.env.CFLAGS += '-std=gnu99  -mmcu=atmega2560 -DF_CPU=16000000 -O1 '
	#ctx.env.CXXFLAGS += ' -mmcu=atmega2560 -DF_CPU=16000000  -std=c++11 -O1  '
	ctx.env.LD += ctx.env.CXXFLAGS
	
	ctx.env.LDFLAGS = ' -T /home/mirmik/project/genos/genos/avr_aiop.lds'
	ctx.env.LDFLAGS += ' -nostdinc' 
	#ctx.env.LDFLAGS += ' /opt/avrchain/lib/gcc/avr/6.0.0/avr6/libgcc.a ' 
	#ctx.env.LDFLAGS += ' /opt/avrchain/avr/lib/avr6/crtm2560.o ' 
	#ctx.env.LDFLAGS += ' /opt/avrchain/avr/lib/avr6/crtm2564rfr2.o '
	#ctx.env.LDFLAGS += ' /opt/avrchain/avr/lib/avr6/libm.a ' 

	
	#ctx.env.LNKFLAGS = ''
	pass
	#-T /home/mirmik/project/genos/genos/ld.ld