#!/usr/bin/env python3
#coding: utf8

from glink.lang.pars import parse_file as parse_file 
from glink.lang.pars import parse_text as parse_text 
	
import os
contextlevels = []
executed_files = []
modules = []
application = []

class Variable:
	def __init__(self, var):
		self.var  = var

	def __repr__(self):
		return("Var(v:" + self.var.__repr__() + ")")

	def equal(self, nvar):
		self.var = nvar

class GFunction:
	def __init__(self, args, block):
		self.args = args; self.block = block;

	def exec(self, args, yblock):
		#print(self.args.parts)
		#print (args.parts)
		nn = [evaluate(e) for e in args.parts]
		contextlevels.append({})
		for z in zip(self.args.parts, nn):
			new_local_var(z[0].parts[0], z[1])
		new_local_var("__yield__", yblock);
		new_local_var("__repeat__", False);
		#print (contextlevels[-1])
		ret = execblock(self.block)
		try:
			if ret[0] == "__block__return__":
				return ret[1]
		except:
			pass
		if ret == "__break__":
			ret = contextlevels[-1]
		del contextlevels[-1]
		return ret
	#print(contextlevels)
	

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
def downlevel(blk):
	contextlevels[-1].update(blk)

@see 
def new_local_var(name, val = None):
	if name in contextlevels[-1]:
		print("that name already exist")
		exit()
	else:
		v = Variable(None)
		v.equal(val)
		contextlevels[-1].update({name: v})
		return v

@see
def get_local_var(name):
	ret = None
	if name in contextlevels[-1]:
		return contextlevels[-1][name]
	else:
		return new_local_var(name) 

@see
def get_exist_local_var(name):
	ret = None
	if name in contextlevels[-1]:
		return contextlevels[-1][name]
	else:
		print("Wrong Local name " + name)
		exit(); 
@see
def under_get_var(name, level):
	ret = None
	if name in contextlevels[level]:
		return contextlevels[level][name]
	else:
		print("Wrong name on level" + level.__repr__())
		exit(); 

@see
def execblock(blk):
	ret = None
	while(True):
		for p in blk.parts:
			ret = evaluate(p)
			if ret == "__yield__":
				ret = execblock(get_local_var("__yield__").var);
			if ret == "__repeat__":
				get_local_var("__repeat__").equal(True);
			if ret == "__break__":
				return "__break__";
			try:
				if ret[0] == "__block__return__": 
					return ret			
			except: 
				pass
		if get_local_var("__repeat__").var == True:
			get_local_var("__repeat__").equal(False)
			continue 
		break
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


def global_start_module_mode(blk, glb,_seed):
	_str = _basepath + 'curprj/' + 'mkscript.gl'
	file = open(_str)
	blk = parse_file(file)
	global _glb 
	global seed
	_glb = glb
	seed = _seed
	contextlevels.append({})
	executed_files.append(_str)
	execblock(blk)
	return {"modules": modules, "applications": application}

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
	modules.append(Module(expr.parts[0], evaluate(expr.parts[1])))
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

def option_parse(option):
	opt = {}
	for o in option.parts:
		if o.type == "equal":
			#opt.update({"a": 2})
			opt.update({o.parts[0]: evaluate(o.parts[1])})
		else:
			print("ERROR SYNTAX option_parse")
			exit()
	return opt

class Module(object):
	""" """
	def __init__ (self, name, variables):
		self.name = name
		self.variables = variables
	def __repr__(self):
		return "Module(n:" + self.name + " v:" + self.variables.__repr__() + ")" 

class Application(object):
	""" """
	def __init__ (self, name, mlist):
		self.name = name
		self.mlist = mlist
	def __repr__ (self):
		return "Application(n:" + self.name + " mlist:" + self.mlist.__repr__() + ")"

class ModuleInc(object):
	"""docstring for ModuleInc"""
	def __init__(self, name, implname, options):
		super(ModuleInc, self).__init__()
		self.name = name
		self.implname = implname
		self.options = options
	def __repr__ (self):
		return "ModuleInc(n:" + self.name + " i:" + self.implname + " o:" + self.options.__repr__() + ")"  		

def application_interpreter(block):
	ret = []
	for e in block.parts:
		if e.type == "var":
			ret.append(ModuleInc(e.parts[0], e.parts[0], None));
		if e.type == "func":
			ret.append(ModuleInc(e.parts[0], e.parts[0], option_parse(e.parts[1])));
		if e.type == "equal":
			if e.parts[1].type == "var":
				ret.append(ModuleInc(e.parts[0], e.parts[1].parts[0], None));
			if e.parts[1].type == "func":
				ret.append(ModuleInc(e.parts[0], e.parts[1].parts[0], option_parse(e.parts[1].parts[1])));
			

	return ret

def construct_application(app, block):
	global application
	#print(block)
	application.append(Application(app, application_interpreter(block)))

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
	ret = execblock(parse_file(_file))
	_file.close()
	del contextlevels[-1]
	del executed_files[-1]
	return ret

def _execfile(expr):
	return __execfile(evaluate(expr.parts[0]))

def evaluate_func(expr):
	v = get_exist_local_var(expr.parts[0])
	ret = v.var.exec(expr.parts[1], expr.parts[2])
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
	
	if expr.type == 'less': return evaluate(expr.parts[0]) < evaluate(expr.parts[1]) 
	

	if expr.type == 'append': return append(expr) 
	
	if expr.type == 'deffunc': 
		 v = new_local_var(expr.parts[0]);
		 v.equal( GFunction(expr.parts[1],expr.parts[2]) ); 
		 return 0
	if expr.type == 'var': return get_local_var(expr.parts[0]).var 
	if expr.type == 'undervar': return under_get_var(expr.parts[0].parts[0], expr.parts[1]).var


	if expr.type == 'module': return module(expr); 
	if expr.type == 'inblock': 
		return evaluate_block(expr, 0)
	if expr.type == 'python': return python_import_impl(evaluate(expr.parts[0])) 
	if expr.type == 'downlevel': return downlevel(evaluate(expr.parts[0])) 
	if expr.type == 'repeat': return '__repeat__' 
	if expr.type == 'variables': return contextlevels[evaluate(expr.parts[0])]
	if expr.type == 'yield': return "__yield__"

	if expr.type == 'isdir': return os.path(evaluate(expr.parts[0]));
	if expr.type == 'listdir': return os.listdir(evaluate(expr.parts[0]));	

	if expr.type == 'mlist': return mlist(expr);
	if expr.type == 'curfile': return curfile();
	if expr.type == 'exfiles': return executed_files;
	if expr.type == 'abspath': return abspath();
	if expr.type == 'relpath': return relpath();
	if expr.type == 'relpathbase': return relpathbase();
	if expr.type == 'cycle': return "__cycle__";
	if expr.type == 'length': return len(evaluate(expr.parts[0]));
	if expr.type == 'invrelpath': return invrelpath();
	if expr.type == 'application': 
		construct_application(expr.parts[0],expr.parts[1]); return 0;

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
	if expr.type == 'evaluate': return evaluate(evaluate(expr.parts[0]))

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

	if expr.type == 'unless': 
		if not evaluate(expr.parts[0]):
			ret = evaluate(expr.parts[1])
		else:
			if expr.parts[2] != None:
				ret = evaluate(expr.parts[2])
			else: 
				ret = None
		return(ret)
	if expr.type == 'func': 
		return evaluate_func(expr)

	if expr.type == 'equal':
		ev = evaluate(expr.parts[1])
		if expr.parts[0].type == "subst":
			v = get_exist_local_var(expr.parts[0].parts[0].parts[0]).var
			if v.type == "var":
				var = get_local_var(v.parts[0])
			if v.type == "undervar":
				var = under_get_var(v.parts[0].parts[0], v.parts[1])
		if expr.parts[0].type == "var":
			var = get_local_var(expr.parts[0].parts[0])
		if expr.parts[0].type == "undervar":
			var = under_get_var(expr.parts[0].parts[0].parts[0], expr.parts[0].parts[1])
		var.equal(ev)
		return(ev)

	if expr.type == 'parttree':
		return(expr.parts[0])

#	if expr.type == 'define':
#		ev = evaluate(expr.parts[1])
#		new_var(expr.parts[0], ev)
#		return(ev)
	print(expr, "EVALUATE ERROR")
	exit()
