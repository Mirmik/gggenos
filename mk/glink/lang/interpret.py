#!/usr/bin/env python3
#coding: utf8

from glink.lang.pars import parse_file as parse_file 
from glink.lang.pars import parse_text as parse_text 
from glink.lang.pars import Node
	
import os

import time

levels = []
yieldblk = []
exfiles = []
block_except = None
	
def see(f):
    def _f(expr):
        print(expr)
        ret = f(expr)
        print("ret = " + ret.__str__())
        return ret 
    return _f

class Variable:

	def __init__(self, name, val):
		self.name = name; self.val = val;

	def __str__(self):
		return self.name + " -> " + self.val.__str__() 

	def set(self, nval):
		self.val = nval 

	def get(self):
		return self.val 

	def __add__(self, b):
		return self.val + b

	def __radd__(self, b):
		return b + self.val

	def __sub__(self, other):
		return self.val - other

	def __rsub__(self, b):
		return b - self.val

	def __lt__(self, other):
		return self.val < other

	def __eq__(self, other):
		return self.val == other

	def __ne__(self, other):
		return self.val != other

	def __gt__(self, other):
		return self.val > other

	def __le__(self, other):
		return self.val <= other

	def __ge__(self, other):
		return self.val >= other

	def __len__(self):
		return len(self.val)

	def __getitem__(self, key):
		#print (self.val)
		return self.val[key]

	def __index__(self):
		return self.val

class GFunction:

	def __init__(self, name, args, block, attr):
		self.name = name; self.args = args; self.block = block; 
		self.attr = attr; self.leveled = True; self.downleveled = False;

	def __str__(self):
		return "gfunc:" + self.name 


	def invoke_names_helper(self, slfarg, extarg):
		#print (slfarg.parts[0].parts[0]);
		if slfarg.type == "var":
			return slfarg.parts[0] 
		if slfarg.type == "parttree":
			return slfarg.parts[0].parts[0]

	def invoke_vars_helper(self, slfarg, extarg):
		#print(evaluate(extarg))
		if slfarg.type == "var":
			return Variable(slfarg.parts[0], evaluate(extarg)) 
		if slfarg.type == "parttree":
			if extarg.type != "parttree":
				return Variable(slfarg.parts[0].parts[0], extarg)
			else:
				return Variable(slfarg.parts[0].parts[0], evaluate(evaluate(extarg)).val)
	
	def attribute_parser(self, attr):
		for var in attr:
			if var.parts[0] == "noleveled":
				self.leveled = False;
		for var in attr:
			if var.parts[0] == "downleveled":
				self.leveled = False;

	def invoke(self, ext_args, yblock):
		global block_except;
		dict = {
		self.invoke_names_helper(self.args.parts[i], ext_args.parts[i]): 
		self.invoke_vars_helper(self.args.parts[i], ext_args.parts[i])
		for i in range(len(self.args.parts))}
		if self.attr!=None: self.attribute_parser(self.attr.parts)
		if self.leveled == True: add_level()
		levels[-1].update(dict)
		if yblock != None:	yieldblk.append(yblock)
		ret = execblock(self.block)
		if yblock != None:	del yieldblk[-1]
		block_except = None
		if self.leveled == True: del_level()
		if self.downleveled == True: levels[-1].update(ret);
		return ret


class LIterator:

	def __init__(self, list, key):
		self.list = list; self.key = key;

	def set(self, nval):
		self.list[self.key] = nval 

	def __str__(self):
		return "li" + " -> " + self.list[self.key] 

	def __add__(self, b):
		return self.list[self.key] + b

	def __radd__(self, b):
		return b + self.list[self.key]

	def __sub__(self, other):
		return self.list[self.key] - other

	def __rsub__(self, b):
		return b - self.list[self.key]

	def __lt__(self, other):
		return self.list[self.key] < other

	def __eq__(self, other):
		return self.list[self.key] == other

	def __ne__(self, other):
		return self.list[self.key] != other

	def __gt__(self, other):
		return self.list[self.key] > other

	def __le__(self, other):
		return self.list[self.key] <= other

	def __ge__(self, other):
		return self.list[self.key] >= other

	def __len__(self):
		return len(self.list[self.key])

	def __index__(self):
		return self.list[self.key]

	def __getitem__(self, key):
		#print (self.val)
		return self.list[self.key][key]

	def get(self):
		return self.list[self.key]


def add_level():
	levels.append({})

def del_level():
	del levels[-1]

def global_start(_f, glb):
	gexecfile(_f)

def gexecfile(_f):
	f = open(_f)
	global exfiles;
	exfiles.append(_f)
	tree = parse_file(f)
	add_level()
	execblock(tree)
	ret = levels[-1]
	del_level()
	del exfiles[-1]
	block_except = None
	return ret

def execblock(block):
	ret = None
	global block_except 
	for metaexpr in block.parts:
		ret = evaluate(metaexpr)
		if block_except != None:
			if block_except == "yield":
				block_except = None;
				yblock = yieldblk[-1]
				del yieldblk[-1]
				execblock(yblock)
				yieldblk.append(yblock)
			if block_except == "return":
				return ret
			if block_except == "break":
				return ret
	return levels[-1] 

def evaluate_loop(expr):
	ret = None
	global block_except 
	while(True):
		ret = execblock(expr.parts[0])
		if block_except != None:
			if block_except == "return":
				return ret
			if block_except == "break":
				block_except = None
				return ret
	return ret 

def evaluate_downlevel(expr):
	levels[-1].update(evaluate(expr.parts[0]))

def evaluate(expr):
	if expr.type in evaluate_table:
		return evaluate_table[expr.type](expr)
	else:
		print("evaluate_error \n" + expr.__str__())
		exit()


def new_local_var(expr, val):
	if expr.parts[0] in levels[-1]:
		print("new_local_var error")
		exit()
	else:
		levels[-1].update({expr.parts[0]: Variable(expr.parts[0],val)})
		return levels[-1][expr.parts[0]]

def get_var(expr):
	for l in levels[::-1]:
		if expr.parts[0] in l:
			return l[expr.parts[0]]
	else:
		print("wrong var name " + expr.parts[0])
		exit()
	


def evaluate_var(expr):
	return get_var(expr)

def evaluate_print(expr):
	print(evaluate(expr.parts[0]))
	return "__print__"

import sys;
def evaluate_printl(expr):
	sys.stdout.write(evaluate(expr.parts[0]))
	return "__printl__"

def evaluate_int(expr):
	return expr.parts[0]
def evaluate_str(expr):
	return expr.parts[0]

def evaluate_plus(expr):
	return evaluate(expr.parts[0]) + evaluate(expr.parts[1]) 
def evaluate_minus(expr):
	return evaluate(expr.parts[0]) - evaluate(expr.parts[1]) 
def evaluate_mul(expr):
	return evaluate(expr.parts[0]) * evaluate(expr.parts[1]) 
def evaluate_div(expr):
	return evaluate(expr.parts[0]) / evaluate(expr.parts[1]) 

def evaluate_equal(expr):
	ev = evaluate(expr.parts[1])
	var = evaluate(expr.parts[0])
	#print (type(var))
	if type(ev) == Variable:
		var.set(ev.get())
	else:	
		var.set(ev)
	return ev;

def evaluate_define(expr):
	var = new_local_var(expr.parts[0], None)
	if expr.parts[1] != None:
		ev = evaluate(expr.parts[1])
		if type(ev) == Variable:
			var.set(ev.get())
		else:	
			var.set(ev)

def evaluate_fn(expr):
	var = new_local_var(expr.parts[0], GFunction(expr.parts[0].parts[0],expr.parts[1],expr.parts[2], expr.parts[3]))
	return "__deffunc__"

def evaluate_dict(expr):
	dict = {evaluate(decl.parts[0]):evaluate(decl.parts[1]) for decl in expr.parts}
	return dict

def evaluate_list(expr):
	list = [evaluate(l) for l in expr.parts]
	return list

def evaluate_invoke(expr):
	if expr.parts[0].type == "var":
		var = evaluate(expr.parts[0])
		return var.val.invoke(expr.parts[1],expr.parts[2])

def evaluate_return(expr):
	global block_except
	block_except = "return"
	return evaluate(expr.parts[0])
def evaluate_break(expr):
	global block_except
	block_except = "break"
	return None
def evaluate_yield(expr):
	global block_except
	block_except = "yield"
	return  

def evaluate_and(expr):
	return evaluate(expr.parts[0]) and evaluate(expr.parts[1])

def evaluate_less(expr):
	return evaluate(expr.parts[0]) < evaluate(expr.parts[1])
def evaluate_more(expr):
	return evaluate(expr.parts[0]) > evaluate(expr.parts[1])
def evaluate_eqless(expr):
	return evaluate(expr.parts[0]) <= evaluate(expr.parts[1])
def evaluate_eqmore(expr):
	return evaluate(expr.parts[0]) >= evaluate(expr.parts[1])
def evaluate_eq(expr):
	return evaluate(expr.parts[0]) == evaluate(expr.parts[1])
def evaluate_noteq(expr):
	return evaluate(expr.parts[0]) != evaluate(expr.parts[1])

def evaluate_if(expr):
	if evaluate(expr.parts[0]): 
		return evaluate(expr.parts[1]) 
	else: 
		if expr.parts[2] != None:
			return evaluate(expr.parts[2])
		else:
			return None

def evaluate_unless(expr):
	if not evaluate(expr.parts[0]): 
		return evaluate(expr.parts[1]) 
	else: 
		if expr.parts[2] != None:
			return evaluate(expr.parts[2])
		else:
			return None
	
def evaluate_millis(expr):
	return time.time() * 1000

def evaluate_element(expr):
	return LIterator(evaluate(expr.parts[0]).get(), evaluate(expr.parts[1]))

def evaluate_execfile(expr):
	return gexecfile(evaluate(expr.parts[0]))

def evaluate_evaltree(expr):
	#print(expr.parts[0])
	#print(evaluate(expr.parts[0]).val)
	return evaluate(evaluate(expr.parts[0]))

def evaluate_evalvar(expr):
	return evaluate(evaluate(expr.parts[0]).val)

def evaluate_unvar(expr):
	return evaluate(expr.parts[0]).get()

def evaluate_subst(expr):
	return evaluate(expr.parts[0])

def evaluate_variables(expr):
	return levels[evaluate(expr.parts[0])]

def evaluate_Node(expr):
	return Node(evaluate(expr.parts[0].parts[0]),evaluate(expr.parts[0].parts[1]))

def evaluate_None(expr):
	return None

def evaluate_parttree(expr):
	return expr.parts[0];

def evaluate_length(expr):
	return len(evaluate(expr.parts[0]));
	
def evaluate_slice(expr):
	if len(expr.parts[0].parts) == 4:
		return evaluate(expr.parts[0].parts[0])[evaluate(expr.parts[0].parts[1]):evaluate(expr.parts[0].parts[2]):evaluate(expr.parts[0].parts[3])]
	if len(expr.parts[0].parts) == 3:
		return evaluate(expr.parts[0].parts[0])[evaluate(expr.parts[0].parts[1]):evaluate(expr.parts[0].parts[2])]
	print("Slice error")
	exit()

def evaluate_listdir(expr):
	return os.listdir(evaluate(expr.parts[0]));

def evaluate_pass(expr):
	return None;


modules = {}
applications = {}

def evaluate_application(expr):
	add_level()
	new_local_var(Node("var", ["name"]), expr.parts[0])
	evaluate(Node("invoke", [Node("var", ["init_application"]), Node("args", []), 'noblock']))
	execblock(expr.parts[1])
	applications.update({expr.parts[0]: levels[-1]});
	del_level()
	return None

def evaluate_module(expr):
	add_level()
	new_local_var(Node("var", ["name"]), expr.parts[0])
	evaluate(Node("invoke", [Node("var", ["init_module"]), Node("args", []), 'noblock']))
	execblock(expr.parts[1])
	modules.update({expr.parts[0]: levels[-1]});
	del_level()
	return None

def evaluate_python(expr):
	return globals()[evaluate(expr.parts[0])]

def evaluate_isdir(expr):
	return os.path.isdir(evaluate(expr.parts[0]))

def evaluate_member(expr):
	return evaluate(expr.parts[0])[expr.parts[1].parts[0]]

def evaluate_relpath(expr):
	return exfiles[-1] 

def evaluate_dirpath(expr):
	return os.path.dirname(exfiles[-1]) 

def evaluate_filename(expr):
	return os.path.basename(exfiles[-1]) 

import re
def evaluate_regex(expr):
	args = expr.parts[0]
	return  (re.findall(evaluate(args.parts[0]),evaluate(args.parts[1])))

evaluate_table = {
	"print" : evaluate_print,	
	"int" : evaluate_int,	
	"str" : evaluate_str,	
	"+" : evaluate_plus,	
	"-" : evaluate_minus,	
	"*" : evaluate_mul,	
	"/" : evaluate_div,	
	"equal" : evaluate_equal,	
	"var" : evaluate_var,	
	"dict" : evaluate_dict,	
	"list" : evaluate_list,	
	"element" : evaluate_element,	
	"return" : evaluate_return,	
	"break" : evaluate_break,	
	"fn" : evaluate_fn,
	"invoke" : evaluate_invoke,
	"if" : evaluate_if,
	"unless" : evaluate_unless,
	"<" : evaluate_less,
	">" : evaluate_more,
	"<=" : evaluate_eqless,
	">=" : evaluate_eqmore,
	"==" : evaluate_eq,
	"!=" : evaluate_noteq,
	"loop" : evaluate_loop,
	"millis" : evaluate_millis,
	"execfile" : evaluate_execfile,
	"yield" : evaluate_yield,
	"define" : evaluate_define,
	"downlevel" : evaluate_downlevel,
	"evalvar" : evaluate_evalvar,
	"evaltree" : evaluate_evaltree,
	"unvar" : evaluate_unvar,
	"subst" : evaluate_subst,
	"variables" : evaluate_variables,
	"parttree" : evaluate_parttree,
	"pass" : evaluate_pass,
	"length" : evaluate_length,
	"slice" : evaluate_slice,
	"Node" : evaluate_Node,
	"None" : evaluate_None,
	"member" : evaluate_member,
	"and" : evaluate_and,
	"python" : evaluate_python,
	"listdir" : evaluate_listdir,
	"isdir" : evaluate_isdir,
	"relpath" : evaluate_relpath,
	"filename" : evaluate_filename,
	"dirpath" : evaluate_dirpath,
	"printl" : evaluate_printl,
	"module" : evaluate_module,
	"regex" : evaluate_regex,
	"application" : evaluate_application,
}