#!/usr/bin/python
# -*- coding: UTF-8 -*-
import sys


#sys.path.append('./mk/compiletree')
sys.path.append('./mk/ply_conf')
#from conf_parser import build_tree
from conf_lexer import lexer





lexer.input(
"""
@add fda "<?dasdfasfasf"
@add fda "<?dasdfasfasf"
@add fda "<?dasdfasfasf"
@add fda "<?dasdfasfasf"
@add fda +dasdfasfasf
"""
	)



while True:
        tok = lexer.token() # читаем следующий токен
        if not tok: break      # закончились печеньки
        print tok


#tree = build_tree
#print tree


