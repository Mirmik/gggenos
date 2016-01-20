

print('->loading the arm tool')

#from waflib.Configure import conf

def options(opt): 
	#opt.load('compiler_c compiler_cxx')
	pass

def configure(ctx): 	
	ctx.env.ARCH = 'arm-stm32f100'
	ctx.env.CC = 'arm-none-eabi-gcc'
	ctx.env.AS = 'arm-none-eabi-as'
	ctx.env.CXX = 'arm-none-eabi-g++'	
	ctx.env.AR = 'arm-none-eabi-ar'	
	ctx.env.LD = 'arm-none-eabi-gcc'	
	#ctx.env.CCFLAGS = '-std=gnu99  -mmcu=atmega2560 -DF_CPU=16000000 -O1 -fdata-sections -ffunction-sections -Wl,--gc-sections'
	#ctx.env.CXXFLAGS = ' -mmcu=atmega2560 -DF_CPU=16000000 -fno-unwind-tables -frtti -fno-exceptions -std=c++11 -O1 -fdata-sections -ffunction-sections -Wl,--gc-sections'
	
	ctx.env.TOOLS_PATH='/usr'
	ctx.env.TOOLS_PREFIX='arm-none-eabi-'
	ctx.env.TOOLS_VERSION='4.9.3'
	ctx.env.BIN='project'

	ctx.env.CCFLAGS = '-std=gnu99'
	ctx.env.CXXFLAGS = '-std=c++11'
	ctx.env.CFLAGS+=' -c -mcpu=cortex-m3 -mthumb -Wall -O0 -mapcs-frame -D__thumb2__=1 '
	ctx.env.CFLAGS+=' -msoft-float -gdwarf-2 -mno-sched-prolog -fno-hosted -mtune=cortex-m3 '
	ctx.env.CFLAGS+=' -march=armv7-m -mfix-cortex-m3-ldrd -ffunction-sections -fdata-sections '
	ctx.env.CFLAGS+=' -I./cmsis -I./stm32_lib -I.'
	ctx.env.CXXFLAGS+=' -c -mcpu=cortex-m3 -mthumb -Wall -O0 -mapcs-frame -D__thumb2__=1 '
	ctx.env.CXXFLAGS+=' -msoft-float -gdwarf-2 -mno-sched-prolog -fno-hosted -mtune=cortex-m3 '
	ctx.env.CXXFLAGS+=' -march=armv7-m -mfix-cortex-m3-ldrd -ffunction-sections -fdata-sections '
	ctx.env.CXXFLAGS+=' -I./cmsis -I./stm32_lib -I.'
	ctx.env.ASFLAGS+=' -mcpu=cortex-m3 -I./cmsis -I./stm32_lib -gdwarf-2 -gdwarf-2 '
	ctx.env.LDFLAGS+=' -static -mcpu=cortex-m3 -mthumb -mthumb-interwork -Wl,--start-group '
	ctx.env.LDFLAGS+=' -L${TOOLS_PATH}/lib/gcc/arm-none-eabi/${TOOLS_VERSION}/thumb2 '
	ctx.env.LDFLAGS+=' -L${TOOLS_PATH}/arm-none-eabi/lib/thumb2 -nostdlib '
	ctx.env.LDFLAGS+=' -Ttext=0x8000000 '
	ctx.env.LDFLAGS+=' -Wl,--end-group -Xlinker -Map -Xlinker project.map -Xlinker '
	ctx.env.LDFLAGS+=' -T ../stm32_lib/device_support/gcc/stm32f100rb_flash.ld -o project.elf -nostartfiles'
	#ctx.env.LDFLAGS+=' -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all '
	ctx.env.LDFLAGS+='  '


