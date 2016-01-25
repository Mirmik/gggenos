#!/usr/bin/env python3
#coding: utf8

from glink.lang.pars import parse_file as parse_file 
from glink.lang.pars import parse_text as parse_text 
	
import os
contextlevels = []
executed_files = []
modules = []
cmodules = []
application = 0

_basepath = os.getcwd() + '/'

seed = 0

def see(f):
	def func(*e):
		if seed == 1:
			print(f.__name__)
			print (*e)
		ret = f(*e)
		return ret
	return func

def curfile():
	return executed_files[-1].split('/')[-1]

def abspath():
	return executed_files[-1] + '/'

def relpath():
	return 0

def relpathbase():
	return os.path.relpath(os.path.dirname(executed_files[-1]), _basepath) + '/'

def invrelpath():
	return 0

@see
def equal_var(name, var):
	for context in contextlevels[::-1]:
		for v in context:
			if name in context:
				context[name] = var
				return var
	print("wrong name to equal")
	exit()
@see
def dequal_var(name, el, var):
	for context in contextlevels[::-1]:
		for v in context:
			if name in context:
				context[name][el] = var
				return var
	print("wrong name to equal", name, ' ' , el)
	exit()
def new_var(name, var):
	contextlevels[-1].update({name : var});
	
@see
def get_var(name):
	for context in contextlevels[::-1]:
		for v in context:
			if name in context:
				return context[name]
	print("wrong variable " , name)
	exit()

@see
def find_in(list, name):
	return(list[name])
	exit()

@see
def append(expr):
	ret = get_var(expr.parts[0].parts[0]) + evaluate(expr.parts[1])
	equal_var(expr.parts[0].parts[0], ret)
	return ret

@see
def downlevel(blk):
	contextlevels[-1].update(blk)

#@see
#def moduleblock(name, blk):
#	modules.append([name,[]]); 
#	pass


@see
def execblock(blk, yield_slot):
	ret = None
	repeat = 0
	while True:
		repeat = 0
		for p in blk.parts:
			ret = evaluate(p)
			try:
				if ret[0] == "__block__return__": 
					return ret[1]
			except: 
				pass
			if ret == "__break__": 
				return "__break__"
			if ret == '__yield__':
				execblock(yield_slot, 0)
			if ret == '__repeat__':
				repeat = 1
		if repeat == 0: break 
	return contextlevels[-1]

_glb = None

def global_start(blk, glb,_seed):
	_str = _basepath + 'curprj/' + 'mkscript.gl'
	file = open(_str)
	blk = parse_file(file)
	global _glb 
	global seed
	_glb = glb
	seed = _seed
	contextlevels.append({})
	executed_files.append(_str)
	return execblock(blk, 0)

def python_import_impl(str):
	kkk = _glb
	kkk.update(globals())
	return kkk[str]	

def list_to_list(l):
	ll = []
	for v in l.parts:
		ll.append([evaluate(v)])
	return ll

def dict_to_dict(d):
	dd = {}
	for v in d.parts:
		dd.update({evaluate(v.parts[0]) : evaluate(v.parts[1])})
	return dd

def find_module(name):
	for m in modules:
		if m[0] == name:
			return m
	print("wrong module " , name)
	exit()

def find_cmodule(name):
	for m in cmodules:
		if m[0] == name[0]:
			return m
	print("wrong module " , name)
	exit()

def module(expr):
	for m in modules:
		if m[0] == expr.parts[0]:
			print("modules names conflict")
			exit()
	modules.append([expr.parts[0], evaluate(expr.parts[1])])
	return 0

def construct_ord(ord,app):
	for n in find_in(app[1],'mdepend'):
		m = find_cmodule(n)
		if m in ord:
			pass
		else:
			ord.append(m)
			ord = construct_ord(ord, m)
	return ord


def construct_application(application):
	ord_modules = []
	app = find_module(application)
	ord_modules.append(app)
	ord_modules = construct_ord(ord_modules, app) 
	print(ord_modules) 
	



def mlist(blk):
	for p in blk.parts:
		if p.type == 'func':
			cmodules.append([p.parts[0],find_module(p.parts[0])[1]])
		if p.type == 'equal':
			cmodules.append([p.parts[0],find_module(p.parts[1].parts[0])[1]])


def execscan_dir(path):
	files = os.listdir(path)
	for f in files:
		if os.path.isdir(path + '/' + f) and f!='HIDE':
			execscan_dir(path + '/' + f)
		elif f[-2:] == 'gl': 
			print(f)
			__execfile_abs(path + '/' + f)

def execscan(expr):
	return execscan_dir(_basepath +  evaluate(expr.parts[0]))

def __execfile_abs(_str):
	contextlevels.append({})
	executed_files.append(_str)
	_file = open(_str)
	ret = execblock(parse_file(_file), 0)
	_file.close()
	del contextlevels[-1]
	del executed_files[-1]
	return ret

def __execfile(_str):
	contextlevels.append({})
	executed_files.append(_basepath + _str)
	_file = open(_str)
	ret = execblock(parse_file(_file), 0)
	_file.close()
	del contextlevels[-1]
	del executed_files[-1]
	return ret

def _execfile(expr):
	return __execfile(evaluate(expr.parts[0]))

def evaluate_block(expr, yield_slot):
	contextlevels.append({}) 
	ret = execblock(expr, 0)
	del contextlevels[-1]
	return ret 

def evaluate_func(expr):
	v = get_var(expr.parts[0])
	contextlevels.append({})
	for z in zip(v[0].parts[1].parts, expr.parts[1].parts):
		new_var(z[0].parts[0], evaluate(z[1]))
	yield_slot = expr.parts[2]
	ret = execblock(v[1], expr.parts[2])
	#print(contextlevels)
	del contextlevels[-1]
	return(ret)


@see
def evaluate(expr):	
	if expr.type == 'int': return expr.parts[0] 
	if expr.type == 'list': return list_to_list(expr)
	if expr.type == 'dict': return dict_to_dict(expr)
	if expr.type == '+': return evaluate(expr.parts[0]) + evaluate(expr.parts[1]) 
	if expr.type == '*': return evaluate(expr.parts[0]) * evaluate(expr.parts[1]) 
	if expr.type == '-': return evaluate(expr.parts[0]) - evaluate(expr.parts[1]) 
	if expr.type == '/': return evaluate(expr.parts[0]) / evaluate(expr.parts[1]) 
	if expr.type == '**': return evaluate(expr.parts[0]) ** evaluate(expr.parts[1]) 
	
	if expr.type == 'append': return append(expr) 
	
	if expr.type == 'deffunc': new_var(expr.parts[0].parts[0], [expr.parts[0],expr.parts[1]]); return 0
	if expr.type == 'var': return get_var(expr.parts[0]) 
	if expr.type == 'module': return module(expr); 
	if expr.type == 'inblock': 
		return evaluate_block(expr, 0)
	if expr.type == 'python': return python_import_impl(evaluate(expr.parts[0])) 
	if expr.type == 'downlevel': return downlevel(evaluate(expr.parts[0])) 
	if expr.type == 'repeat': return '__repeat__' 
	if expr.type == 'variables': return contextlevels[evaluate(expr.parts[0])]
	if expr.type == 'yield': return "__yield__"
	

	if expr.type == 'mlist': return mlist(expr);
	if expr.type == 'curfile': return curfile();
	if expr.type == 'exfiles': return executed_files;
	if expr.type == 'abspath': return abspath();
	if expr.type == 'relpath': return relpath();
	if expr.type == 'relpathbase': return relpathbase();
	if expr.type == 'invrelpath': return invrelpath();
	if expr.type == 'application': 
		construct_application(expr.parts[0].parts[0]); return 0;

	if expr.type == 'compile': return compile(expr);

	if expr.type == 'loop':  
		while True:
			ret = evaluate(expr.parts[0])
			if ret == "__break__": break
		return 0

	if expr.type == 'break':
		return "__break__"

	#if expr.type == 'pfor':  return 0

	if expr.type == 'while':  
		while True:
			if evaluate(expr.parts[0]) == 0: break
			ret = evaluate(expr.parts[1])
			if ret == "__break__": break
		return 0
	
	if expr.type == 'varvar': 
		#print (get_var(expr.parts[0].parts[0]))
		#print (expr.parts[1].parts[0])
		return find_in(evaluate(expr.parts[0]), expr.parts[1].parts[0]) 

	if expr.type == 'element': return evaluate(expr.parts[0])[evaluate(expr.parts[1])]
	if expr.type == 'execfile': return _execfile(expr)

	if expr.type == 'execscan': return execscan(expr)

	if expr.type == 'exectext': 
		return execblock(parse_text(evaluate(expr.parts[0])))
	if expr.type == 'str': return expr.parts[0]
	if expr.type == 'return': 
		return ["__block__return__", evaluate(expr.parts[0])]
	
	if expr.type == 'modules': return modules; 
	if expr.type == 'input': return input(); 
	if expr.type == 'pass': return None; 
	if expr.type == 'print': 
		ev = evaluate(expr.parts[0])
		print(ev)
		return(ev)
	
	if expr.type == 'if': 
		if evaluate(expr.parts[0]):
			ret = evaluate(expr.parts[1])
		else:
			ret = evaluate(expr.parts[2])
		return(ret)

	if expr.type == 'func': 
		return evaluate_func(expr)

	if expr.type == 'equal':
		ev = evaluate(expr.parts[1])
		equal_var(expr.parts[0], ev)
		return(ev)


	if expr.type == 'dequal':
		ev = evaluate(expr.parts[1])
		dequal_var(expr.parts[0].parts[0].parts[0], evaluate(expr.parts[0].parts[1]), ev)
		return(ev)


	if expr.type == 'parttree':
		return(expr.parts[0])

	if expr.type == 'define':
		ev = evaluate(expr.parts[1])
		new_var(expr.parts[0], ev)
		return(ev)
	print(expr, "EVALUATE ERROR")
	exit()
