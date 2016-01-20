

PLATFORM = 'avr-mega2560'

modules = [
'src',
#'stm32_lib'
]


def _echo(s):
	return 'echo ' + s + ' && ' + s 

LIBC_PATH = '../src/compat/libc/genos_libc/include '
#LIBC_PATH = '../src/compat/libc/musl/include '

def options(ctx):
	ctx.load(PLATFORM + '-tool', tooldir = './waf-tools')

def configure(ctx):

	ctx.env.INCLUDE = ' '
	ctx.env.INCLUDEXX = ''
	ctx.env.ASFLAGS = ''
	ctx.env.LDFLAGS += ''
	ctx.env.CFLAGS = '-std=gnu99 '
	ctx.env.CFLAGS += ''
	ctx.env.ASFLAGS += '  '
	ctx.env.CXXFLAGS = '-std=c++11 '
	ctx.env.CXXFLAGS += ' -fno-exceptions '
	ctx.env.LDFLAGS = ''

	ctx.env.LD_LIBS = ''
	ctx.load(PLATFORM + '-tool', tooldir = './waf-tools')


	ctx.env.INCLUDE = '-I ../src/include -I ../src/arch/' + ctx.env.ARCH + '/include '
	#ctx.env.INCLUDE +='-I ' + LIBC_PATH
	#ctx.env.INCLUDEXX ='-I ../src/compat/cxx/include ' 
	#ctx.env.INCLUDE +='-I ../src/compat/posix/include ' 
	#ctx.env.INCLUDE +='-I ../src/lib '
	ctx.env.AS_OBJ = '${AS} ${ASFLAGS} ${SRC} -c -o ${TGT}'
	ctx.env.CC_OBJ = '${CC} ${SRC} -c -o ${TGT} ${CFLAGS}  ${INCLUDE}'
	ctx.env.CXX_OBJ = '${CXX} ${SRC} -c -o ${TGT}  ${CXXFLAGS}  ${INCLUDE}  ${INCLUDEXX}'
	ctx.env.LNK = '${LD} ${OBJS} -o ${TGT}  ${LDFLAGS}   ${INCLUDE}'	
	

def build(ctx):
	ctx.env.OBJS = " "
	ctx.env.LIBC = " "
	ctx.env.LIBCXX = " "
	ctx.env.LIB = " "

	for m in modules:
		ctx.recurse(m)

	ctx(rule = _echo(ctx.env.LNK), source= ctx.env.OBJS  , target='project')
	ctx(rule = 'cp ${SRC} ${TGT}', source='project', target='../gen')