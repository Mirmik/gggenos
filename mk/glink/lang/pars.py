#!/usr/bin/env python3
# coding=utf8

from libpy.colortext  import *

from glink.lang.lex import tokens
from glink.lang.lex import lexer
import ply.yacc as yacc

from collections import Iterable

start = 'inblock'


class Node:
    def parts_str(self):
        st = []
        for part in self.parts:
            st.append( str( part ) )
        return "\n".join(st)

    def __repr__(self):
        #if isinstance(self.parts, Iterable):
        return self.type + ":\n\t" + self.parts_str().replace("\n", "\n\t")
        #else:
        #    return self.type + ": " + str(self.parts)

    def add_parts(self, parts):
        self.parts += parts
        return self

    def __init__(self, type, parts):
        self.type = type
        self.parts = parts

def change_type(r, str):
    r2 = r; r2.type = str; return r2

def p_block(p):
    """block : LBRACE inblock RBRACE"""
    p[0] = p[2]

def p_inblock(p):
    """inblock :  metaexpr 
               | inblock divider metaexpr
               | divider inblock
               | inblock divider"""
    if len(p) == 1: p[0] = Node("inblock", [])
    elif len(p) == 2:   p[0] = Node("inblock", [p[1]])
    elif len(p) == 3: 
        if p[2] == "divider": p[0] = p[1]
        else: p[0] = p[2]
    else:             p[0] = p[1].add_parts([p[3]])


def p_print(p):
    """print : PRINT expr"""
    p[0] = Node("print", [p[2]])

def p_printl(p):
    """printl : PRINTL expr"""
    p[0] = Node("printl", [p[2]])

def p_module(p):
    """module : MODULE WORD block"""
    p[0] = Node("module", [p[2], p[3]])

def p_fn(p):
    """fn : FN var LPAREN args RPAREN block
          | FN var LPAREN args RPAREN comms block"""
    if len(p) == 7:
        p[0] = Node("fn", [p[2], p[4], p[6], None])
    else:
        p[0] = Node("fn", [p[2], p[4], p[7], p[6]])
    
def p_dict(p):
    """dict : LBRACE declares RBRACE
            | LBRACE RBRACE"""
    if len(p) == 3:
        p[0] = Node("dict", [])
    else:
        p[0] = change_type(p[2], "dict")

def p_list(p):
    """list : LBRACKET comms RBRACKET
            | LBRACKET RBRACKET"""
    if len(p) == 3:
        p[0] = Node("list", [])
    else:
        p[0] = change_type(p[2], "list")


def p_declares(p):
    """declares : declare
                | declares COMMA declare"""
    if len(p) == 2:
        p[0] = Node("declares", [p[1]])
    else:
        p[0] = p[1].add_parts([p[3]])

def p_declare(p):
    """declare : expr COLON expr"""
    p[0] = Node("declare", [p[1], p[3]])


def p_if(p):
    """if : IF LPAREN expr RPAREN expr
              | IF LPAREN expr RPAREN expr ELSE expr"""
    if len(p) == 6:
        p[0] = Node("if", [p[3],p[5], None])
    else:
        p[0] = Node("if", [p[3],p[5], p[7]])

def p_unless(p):
    """unless : UNLESS LPAREN expr RPAREN expr
             | UNLESS LPAREN expr RPAREN expr ELSE expr"""
    if len(p) == 6:
        p[0] = Node("unless", [p[3],p[5], None])
    else:
        p[0] = Node("unless", [p[3],p[5], p[7]])

def p_divider(p):
    """divider : DIVIDER"""
    p[0] = "divider"

def p_input(p):
    """input : INPUT"""
    p[0] = Node("input", [])

def p_define(p):
    """define : DEFINE var
              | DEFINE var EQUALS expr"""
    if len(p) == 3:
        p[0] = Node("define", [p[2], None])
    else:
        p[0] = Node("define", [p[2], p[4]])

def p_invoke(p):
    """invoke : var LPAREN args RPAREN block
            | var LPAREN args RPAREN
            | member LPAREN args RPAREN block
            | member LPAREN args RPAREN
            """
    if len(p) == 5:
        p[0] = Node("invoke", [p[1], p[3], 'noblock'])
    else:
        p[0] = Node("invoke", [p[1], p[3], p[5]])


def p_metaexpr(p):
    """metaexpr : expr
                | fn
                | module
                | application
                | downlevel"""
    p[0] = p[1]

  
def p_undervar(p):
    """undervar : AT undervar
                | AT var"""
    if p[2].type == "undervar":
        p[2].parts[1] = p[2].parts[1] - 1; 
    else:
        p[0] = Node("undervar", [p[2], -2]);

#def p_for(p):
#    """for : FOR var IN expr COLON expr"""
#    p[0] = Node("pfor", [p[2],p[4],p[6]])
    
#def p_while(p):
#    """while : WHILE expr COLON expr"""
#    p[0] = Node("while", [p[2],p[4]])

def p_loop(p):
    """loop : LOOP LPAREN RPAREN expr"""
    p[0] = Node("loop", [p[4]])


def p_millis(p):
    """millis : MILLIS LPAREN RPAREN"""
    p[0] = Node("millis", None)

def p_break(p):
    """break : BREAK"""
    p[0] = Node("break", [])

def p_None(p):
    """None : NONE"""
    p[0] = Node("None", [None])

def p_parttree(p):
    """parttree : AMPERSAND metaexpr"""
    p[0] = Node("parttree", [p[2]])

def p_args(p):
    """args : 
            | comms"""
    if len(p) == 1: p[0] = Node("args", [])
    else: p[0] = change_type(p[1], "args")

def p_comms(p):
    """comms : expr
             | comms COMMA expr
             | comms COMMA"""
    if len(p) == 2:
        p[0] = Node("comms", [p[1]])
    elif len(p) == 4:
        p[0] = p[1].add_parts([p[3]])
    else:
        p[0] = p[1];

def p_equal(p):
    """equal : expr EQUALS expr"""
    p[0] = Node("equal", [p[1], p[3]])

def p_and(p):
    """and : expr AND expr"""
    p[0] = Node("and", [p[1], p[3]])

def p_application(p):
    """application : APPLICATION WORD block"""
    p[0] = Node("application", [p[2], p[3]])

def p_isdir(p):
    """isdir : ISDIR LPAREN expr RPAREN"""
    p[0] = Node("isdir", [p[3]])

def p_listdir(p):
    """listdir : LISTDIR LPAREN expr RPAREN """
    p[0] = Node("listdir", [p[3]])

def p_regex(p):
    """regex : REGEX LPAREN args RPAREN """
    p[0] = Node("regex", [p[3]])

def p_relpath(p):
    """relpath : RELPATH LPAREN RPAREN """
    p[0] = Node("relpath", [])

def p_dirpath(p):
    """dirpath : DIRPATH LPAREN RPAREN """
    p[0] = Node("dirpath", [])

def p_filename(p):
    """filename : FILENAME LPAREN RPAREN """
    p[0] = Node("filename", [])

def p_expr(p):
    """expr : expralg
            | boolop
            | parttree
            | alg0
            | print
            | loop
            | variables
            | python
            | execfile
            | break
            | equal
            | if
            | unless
            | subst
            | yield
            | pass
            | define
            | printl
            | and
            | regex
            | dict
            | Node
            | return
            | input"""
    p[0] = p[1]


def p_expralg(p):
    """expralg : term
            | invoke
            | element
            | millis
            | evaltree
            | evalvar
            | unvar
            | slice
            | listdir
            | None
            | relpath
            | filename
            | dirpath
            | isdir
            | block
            | length
            | list"""
    p[0] = p[1]

def p_return(p):
    """return : RETURN expr"""
    p[0] = Node("return", [p[2]])


def p_term(p):
    """term : str
            | float
            | int 
            | var
            | undervar
            | member"""
    p[0] = p[1]

def p_int(p):
    """int : NUMBER"""
    p[0] = Node("int", [p[1]])

def p_python(p):
    """python : PYTHON str"""
    p[0] = Node("python", [p[2]])

def p_execfile(p):
    """execfile : EXECFILE expr"""
    p[0] = Node("execfile", [p[2]])

def p_evalvar(p):
    """evalvar : EVALVAR LPAREN expr RPAREN"""
    p[0] = Node("evalvar", [p[3]])

def p_unvar(p):
    """unvar : UNVAR LPAREN expr RPAREN"""
    p[0] = Node("unvar", [p[3]])

def p_evaltree(p):
    """evaltree : EVALTREE LPAREN expr RPAREN"""
    p[0] = Node("evaltree", [p[3]])

def p_slice(p):
    """slice : SLICE LPAREN args RPAREN"""
    p[0] = Node("slice", [p[3]])

def p_subst(p):
    """subst : SUBST expr"""
    p[0] = Node("subst", [p[2]])

def p_Node(p):
    """Node : NODE LPAREN args RPAREN"""
    p[0] = Node("Node", [p[3]])

#def p_subst(p):
#    """subst : SUBST LPAREN expr RPAREN"""
#    p[0] = Node("subst", [p[3]])

#def p_execscan(p):
#    """execscan : EXECSCAN expr"""
#    p[0] = Node("execscan", [p[2]])

def p_boolop(p):
    """boolop : alg0 BOOLOP alg0"""
    p[0] = Node(p[2], [p[1], p[3]])

def p_length(p):
    """length : LENGTH LPAREN expr RPAREN"""
    p[0] = Node("length", [p[3]])

def p_downlevel(p):
    """downlevel : DOWNLEVEL expr"""
    p[0] = Node("downlevel", [p[2]])

def p_float(p):
    """float : FLOAT"""
    p[0] = Node("float", [p[1]])

def p_yield(p):
    """yield : YIELD"""
    p[0] = Node("yield", [])

def p_str(p):
    """str : STRING"""
    p[0] = Node("str", [p[1]])

def p_element(p):
    """element : expr LBRACKET expr RBRACKET"""
    p[0] = Node("element", [p[1], p[3]])

def p_var(p):
    """var : WORD"""
    p[0] = Node("var", [p[1]])

def p_member(p):
    """member : var
              | member DOT var"""
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = Node("member", [p[1], p[3]])






def p_variables(p):
    """variables : VARIABLES expr"""
    p[0] = Node("variables", [p[2]])

#def p_curfile(p):
#    """curfile : CURFILE"""
#    p[0] = Node("curfile", [None])


#def p_relpath(p):
#    """relpath : RELPATH"""
#    p[0] = Node("relpath", [None])

#def p_exfiles(p):
#    """exfiles : EXFILES"""
#    p[0] = Node("exfiles", [None])

#def p_relpathbase(p):
#    """relpathbase : RELPATHBASE"""
#    p[0] = Node("relpathbase", [None])

def p_pass(p):
    """pass : PASS"""
    p[0] = Node("pass", [None])

#def p_abspath(p):
#    """abspath : ABSPATH"""
#    p[0] = Node("abspath", [None])

def parse_error(str,p):
    print(red(str), p)

def p_error(p):
    print('Unexpected token:', p)
    exit()


def p_alg0(p):
    """alg0 : alg0 PLUSMINUS alg1
            | PLUSMINUS alg1
            | alg1"""
    if len(p) == 2:
        p[0] = p[1]
    elif len(p) == 3:
        p[0] = Node(p[1], [Node('int', [0]), p[2]])
    else:
        p[0] = Node(p[2], [p[1], p[3]])

def p_alg1(p):
    """alg1 : alg1 DIVMUL alg2
             | alg2 """
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = Node(p[2], [p[1], p[3]])

def p_alg2(p):
    """alg2 : alg2 DPROD alg3
            | alg3"""
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = Node(p[2], [p[1], p[3]])

def p_alg3(p):
    """alg3 : expralg"""
    p[0] = p[1]

parser = yacc.yacc()


def parse_file(file):
    lexer.lexed_file = file
    ret = parser.parse(file.read())
    return ret 

def parse_text(text):
    ret = parser.parse(text)
    return ret 