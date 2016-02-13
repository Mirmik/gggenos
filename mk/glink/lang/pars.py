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

def p_module(p):
    """module : MODULE WORD block"""
    p[0] = Node("module", [p[2], p[3]])

def p_deffunc(p):
    """deffunc : DEFFUNC WORD LPAREN args RPAREN block"""
    p[0] = Node("deffunc", [p[2], p[4], p[6]])
    

def p_define(p):
    """define : DEFINE WORD expr"""
    p[0] = Node("define", [p[2], p[3]])

def p_defop(p):
    """define : WORD DEFOP expr"""
    p[0] = Node("define", [p[1], p[3]])

def p_dict(p):
    """dict : LBRACE declares RBRACE
            | LBRACE RBRACE"""
    if len(p) == 3:
        p[0] = Node("dict", [])
    else:
        p[0] = change_type(p[2], "dict")

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
    """if : IF LPAREN expr RPAREN expr"""
    p[0] = Node("if", [p[3],p[5]])

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


def p_modules(p):
    """modules : MODULES"""
    p[0] = Node("modules", [])

def p_append(p):
    """append : var APPEND expr"""
    p[0] = Node("append", [p[1], p[3]])

def p_func(p):
    """func : WORD LPAREN args RPAREN block
            | WORD LPAREN args RPAREN
            """
    if len(p) == 5:
        p[0] = Node("func", [p[1], p[3], 'noblock'])
    else:
        p[0] = Node("func", [p[1], p[3], p[5]])


def p_metaexpr(p):
    """metaexpr : expr
                | deffunc
                | module
                | application
                | declare
                | append
                | define
                | execscan
                | compile
                | downlevel
                | if"""
    p[0] = p[1]


def p_list(p):
    """list : LBRACKET comms RBRACKET
            | LBRACKET RBRACKET"""
    if len(p) == 3:
        p[0] = Node("list", [])
    else:
        p[0] = change_type(p[2], "list")
  

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

#def p_loop(p):
#    """loop : LOOP COLON expr"""
#    p[0] = Node("loop", [p[3]])

def p_break(p):
    """break : BREAK"""
    p[0] = Node("break", [])

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
             | comms COMMA expr"""
    if len(p) == 2:
        p[0] = Node("comms", [p[1]])
    else:
        p[0] = p[1].add_parts([p[3]])

def p_equal(p):
    """equal : expr EQUALS expr"""
    p[0] = Node("equal", [p[1], p[3]])


def p_application(p):
    """application : APPLICATION WORD block"""
    p[0] = Node("application", [p[2], p[3]])

def p_isdir(p):
    """isdir : ISDIR LPAREN expr RPAREN"""
    p[0] = Node("isdir", [p[3]])

def p_listdir(p):
    """listdir : LISTDIR LPAREN expr RPAREN """
    p[0] = Node("listdir", [p[3]])

def p_expr(p):
    """expr : expralg
            | parttree
            | alg0
            | print
            | varvar
            | variables
            | python
            | execfile
            | break
            | unless
            | curfile
            | exectext
            | equal
            | element
            | less
            | length
            | cycle
            | mlist
            | evaluate
            | yield
            | repeat
            | pass
            | dict
            | return
            | modules
            | exfiles
            | input"""
    p[0] = p[1]


def p_expralg(p):
    """expralg : term
            | abspath
            | invrelpath
            | subst
            | relpath
            | relpathbase
            | func
            | listdir
            | isdir
            | block
            | list"""
    p[0] = p[1]

def p_return(p):
    """return : RETURN expr"""
    p[0] = Node("return", [p[2]])


def p_cycle(p):
    """cycle : CYCLE"""
    p[0] = Node("cycle", [None])


def p_term(p):
    """term : str
            | float
            | int 
            | var
            | undervar"""
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

def p_evaluate(p):
    """evaluate : EVALUATE expr"""
    p[0] = Node("evaluate", [p[2]])

def p_subst(p):
    """subst : SUBST LPAREN expr RPAREN"""
    p[0] = Node("subst", [p[3]])

def p_execscan(p):
    """execscan : EXECSCAN expr"""
    p[0] = Node("execscan", [p[2]])


def p_less(p):
    """less : expr LESS expr"""
    p[0] = Node("less", [p[1], p[3]])

def p_length(p):
    """length : LENGTH LPAREN expr RPAREN"""
    p[0] = Node("length", [p[3]])

def p_exectext(p):
    """exectext : EXECTEXT expr"""
    p[0] = Node("exectext", [p[2]])

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

def p_repeat(p):
    """repeat : REPEAT"""
    p[0] = Node("repeat", [p[1]])


def p_element(p):
    """element : expr LBRACKET expr RBRACKET"""
    p[0] = Node("element", [p[1], p[3]])

def p_var(p):
    """var : WORD"""
    p[0] = Node("var", [p[1]])

def p_variables(p):
    """variables : VARIABLES expr"""
    p[0] = Node("variables", [p[2]])


def p_mlist(p):
    """mlist : MLIST COLON block"""
    p[0] = change_type(p[3], "mlist")
    
def p_varvar(p):
    """varvar : var
              | varvar POINT var"""
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = Node("varvar", [p[1], p[3]])

def p_curfile(p):
    """curfile : CURFILE"""
    p[0] = Node("curfile", [None])


def p_relpath(p):
    """relpath : RELPATH"""
    p[0] = Node("relpath", [None])

def p_exfiles(p):
    """exfiles : EXFILES"""
    p[0] = Node("exfiles", [None])

def p_relpathbase(p):
    """relpathbase : RELPATHBASE"""
    p[0] = Node("relpathbase", [None])

def p_compile(p):
    """compile : COMPILE"""
    p[0] = Node("compile", [None])

def p_pass(p):
    """pass : PASS"""
    p[0] = Node("pass", [None])

def p_invrelpath(p):
    """invrelpath : INVRELPATH"""
    p[0] = Node("invrelpath", [None])

def p_(p):
    """abspath : ABSPATH"""
    p[0] = Node("abspath", [None])

def parse_error(str,p):
    print(red(str), p)

def p_error(p):
    print('Unexpected token:', p)
    exit()


def p_alg0(p):
    """alg0 : alg1
            | alg0 PLUSMINUS alg1
            | PLUSMINUS alg1"""
    if len(p) == 2:
        p[0] = p[1]
    elif len(p) == 3:
        p[0] = Node(p[1], [Node('int', [0]), p[2]])
    else:
        p[0] = Node(p[2], [p[1], p[3]])

def p_alg1(p):
    """alg1 : alg2
            | alg1 DIVMUL alg2"""
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[0] = Node(p[2], [p[1], p[3]])

def p_alg2(p):
    """alg2 : alg3
            | alg2 DPROD alg3"""
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