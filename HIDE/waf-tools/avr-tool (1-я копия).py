

print('->loading the avr tool')

#from waflib.Configure import conf

def options(opt): 
	#opt.load('compiler_c compiler_cxx')
	pass

def configure(ctx): 	
	ctx.env.ARCH = 'avr'
	ctx.env.CC = 'avr-gcc'
	ctx.env.CXX = 'avr-g++'	
	ctx.env.CCFLAGS = '-std=gnu99 -nostdlib'
	ctx.env.CXXFLAGS = '-nostdlib'
	pass