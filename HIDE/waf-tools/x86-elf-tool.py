

print('->loading the avr tool')

#from waflib.Configure import conf

def options(opt): 
	#opt.load('compiler_c compiler_cxx')
	pass

def configure(ctx): 	
	ctx.env.ARCH = 'x86_elf'
	ctx.env.CC = 'gcc'
	ctx.env.CXX = 'g++'	
	ctx.env.CCFLAGS = '-nostdinc -nostartfiles'
	ctx.env.CXXFLAGS = '-nostdinc -nostartfiles'
	pass