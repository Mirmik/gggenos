#!/usr/bin/env python3
#coding: utf-8

import os
import subprocess
import mk.libpy.colortext

modules = []
ModDelegateList = {}
ReverseModDelegateList = {}

def exist_list_test(lst):
	for l in lst:
		if os.path.exists(l) == False:
			print ("File " + l + " is not exist")
			exit()


class Module:

	def __init__(self, pmod, options):
		self.opt = options
		self.need_to_compile = False
		self.name = pmod["name"].val
		self.dirpath = pmod["dirpath"].val
		self.cc_source = pmod["cc_source"].val
		self.cpp_source = pmod["cpp_source"].val
		self.depends = pmod["depends"].val
		self.mdepends = pmod["mdepends"].val + pmod["global_mdepends"].val
		self.headers = pmod["headers"].val
		self.include = pmod["include"].val
		self.defines = pmod["defines"].val
		self.target = pmod["target"].val
		self.cc = pmod["CC"].val
		self.cpp = pmod["CPP"].val
		self.ar = pmod["AR"].val
		self.options = pmod["options"].val
		self.cpp_flags = pmod["CXXFLAGS"].val
		self.cc_flags = pmod["CFLAGS"].val
		self.glb_include = pmod["global_include"].val
		self.pathed_cc_source = [self.dirpath + '/' + s for s in self.cc_source]
		self.pathed_cpp_source = [self.dirpath + '/' + s for s in self.cpp_source]
		self.pathed_depends = [self.dirpath + '/' + s for s in self.depends]
		self.pathed_headers = [self.dirpath + '/' + h for h in self.headers]
		self.pathed_include = [self.dirpath + '/' + h for h in self.include]
		self.pathed_target = "build/" + self.dirpath + '/' + self.target	
		exist_list_test(self.pathed_headers)
		exist_list_test(self.pathed_cc_source)
		exist_list_test(self.pathed_cpp_source)
		exist_list_test(self.pathed_depends)

		if self.target == "":
			self.pathed_target = None
			self.need_to_compile = False
			return

		if os.path.exists(self.pathed_target):
			self.target_date = os.path.getmtime(self.pathed_target) 
		else:
			self.target_date = None

		deps =  (self.pathed_cc_source + 
				self.pathed_cpp_source + 
				self.pathed_headers +
				self.pathed_depends)
		#print(deps);
		#exit();
		if len(deps) != 0:
			self.source_date = os.path.getmtime(deps[0])
			for d in deps:
				if os.path.getmtime(d) > self.source_date:
					self.source_date = os.path.getmtime(d)

			if self.target_date == None:
				self.need_to_compile = True
			else:
				if self.source_date > self.target_date:
					self.need_to_compile = True
		else:
			self.need_to_compile = False

	def __repr__(self):
		return self.name

	def depends_list_construct(self, mlist):
		if self.name in mlist:
			return []
		dlist = [ReverseModDelegateList[self.name]]
		mlist.append(self.name)

		for m in self.mdepends:
			if m in ModDelegateList:
				dlist += find_module(ModDelegateList[m]).depends_list_construct(mlist)
			else:
				print ("Error module name " + m) 
				exit()
		return dlist

	def compile(self):
		otarget = []

		mlist = []
		dlist = self.depends_list_construct(mlist)
		inclstr = "-I" + self.glb_include + " "
		defstr = ""

		incllst = []
		deflst = []
		
		for m in dlist:
			mod = find_module(ModDelegateList[m])
			incllst += mod.pathed_include
			deflst += mod.defines
		incllst = list(set(incllst))

		for incl in incllst:
			inclstr += "-I" + incl + " "
			
		for deff in deflst:
			defstr += "-D" + deff + " "

		optionsstr = self.opt
		#optionsstr = ""

		for fcpp in self.pathed_cpp_source:
			ocpp = "build/" + fcpp[:-4] + ".o"
			command = self.cpp + " " + fcpp + " -c -o " + ocpp + " " + self.cpp_flags + " " + inclstr + " " + defstr + " " + optionsstr
			print('\n' + command)
			subprocess.check_output(command.split())
			otarget.append(ocpp)

		for fcc in self.pathed_cc_source:
			occ = "build/" + fcc[:-2] + ".o"
			command = self.cc + " " + fcc + " -c -o " + occ + " " + self.cc_flags + " " + inclstr + " " + defstr + " " + optionsstr
			print('\n' + command)
			subprocess.check_output(command.split())
			otarget.append(occ)

		#print(self.name)
		#print(self.ar)
		#print(self.pathed_target)
		#print(otarget)


		command = self.ar + " rc " + self.pathed_target + " " + " ".join(otarget)
		print ('\n' + command)
		subprocess.check_output(command.split())

def find_module(name):
	for m in modules:
		if m.name == name:
			return m 
	print("Find Module error " + name)
	exit()

def makesystem(mods, apps):
	print("MAKESYSTEM START:")
	for key_app in apps:
		app = apps[key_app]
		mdeclares = app["modules"].val
		#print(mdeclares)
		#Проверка на существование модулей
		
		for m in mdeclares:
			ml = len(m)
			if ml == 3:
				mm = m[1]
				ModDelegateList.update({m[0]: m[1]})
				ReverseModDelegateList.update({m[1]: m[0]})
				options = m[2]
			elif ml == 2:
				mm = m[0]
				ModDelegateList.update({m[0]: m[0]})
				ReverseModDelegateList.update({m[0]: m[0]})
				options = m[1]
			else:
				print("Makesystem error")
				exit()
			if mm in mods:
				modules.append(Module(mods[mm], options))
				print("Using module " + mm)
			else:
				print("Error makesystem 2 " + mm)
				exit();

#Принудительная компиляция зависимых модулей.
		for m in modules:
			mlist=[]
			dlist = m.depends_list_construct(mlist)
			for d in dlist:
				dmod = find_module(ModDelegateList[d])
				if dmod.need_to_compile == True:
					m.need_to_compile = True
					break

		ntc = 0
		for m in modules:
			if m.need_to_compile == True:
				ntc += 1
				m.compile()
		print('\n' + ntc.__repr__() + " modules was compiled")

		targets = []
		for m in modules:
			targets.append(m.pathed_target)

		targets = [value for value in targets if value]
		print(targets)

		LD = app["LD"].val;
		LDFLAGS = app["LDFLAGS"].val;
		atarget = app["target"].val;

		targstr = " ".join(targets)

		command = LD + " -Wl,--start-group " + targstr + " -Wl,--end-group " + " -o " + atarget + " " + LDFLAGS + " "
		print('\n' + command + '\n')
		subprocess.check_output(command.split())

		print(mk.libpy.colortext.green("Application was successfully constructed"));



