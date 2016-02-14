#!/usr/bin/env python3
#coding: utf-8

from libpy.colortext  import *
import ply.lex



brackets = []

tokens = (
    #"SUPERWORD",
    'WORD', 
    'FLOAT',
    'HEXNUMBER', 'BINNUMBER', 'NUMBER', 'STRING', "WRONGSTRING",

    'BOOLOP',
    'LPAREN',   'RPAREN',   # ( )
    'LBRACKET', 'RBRACKET', # [ ]
    'LBRACE',   'RBRACE',   # { }
    'COMMA', 'DOT', 'COLON', 'DOUBLECOLON', 'EQUALS', 'SEMI', 'AMPERSAND',
    'OR',

    'MPROD', 'DPROD',
    'DIVMUL', 'PLUSMINUS',
    'APPEND',
    
    'DIVIDER',
    
    
    'IF', 'ELSE',
    'LOOP',
    

    #PYTHON_FUNC
    'LISTDIR',
    'ISDIR',    

    'QUESTION',
    'NEWLINE',

    #KEYWORDS
    'FN',
    #'DEFINE',
    'MODULE',
    'DOWNLEVEL',
    
    'NOT',
    'AT',
    'APPLICATION',
    'VARIABLES',
    'PYTHON',
    'EXECFILE',
    'FILENAME',
    #'EXECTEXT',
    'INPUT',
    'RETURN',
    'REGEX',
    #'REPEAT',
    'BREAK',
    #'APLICATION',
    'YIELD',
    #'MLIST',
    'UNLESS',
    'AND',
    #'MODULES',
    'LENGTH',
    'MILLIS',
    'NONE',
    'NODE',
    #'CURFILE',
    #'ABSPATH',
    'RELPATH',
    'DIRPATH',
    #'RELPATHBASE',
    #'EXFILES',
    'EVALVAR',
    'EVALTREE',
    'DEFINE',
    'SLICE',
    'PRINTL',
    'SUBST',
    #'EXECSCAN',
    #'INVRELPATH',
    #'CYCLE',
    #'COMPILE',
    #'SUBST',
    #'DEFOP',
    'PASS',
    'UNVAR',
    #'IN',
    'PRINT',
)

t_ignore           = ' \t'

def t_ignore_COMMENT(t):
    r'(//.*)|(\#.*)'
    return None

def t_DIVIDER(t):
    r';+'
    return t

def t_NEWLINE(t):
    r'\n+'
    nr_newlines = t.value.count('\n')
    t.lexer.lineno += nr_newlines
    return None


def lexer_error(str, t):
    print(red("in file ") + lexer.lexed_file.name + " ", red(str), t)
    exit()

def check_bracket(br, t):
    if len(brackets) == 0: lexer_error("closed nonoppen bracket", t)
    if br != brackets[-1].value:
        lexer_error("wrong bracket type", t);
        exit()

# Paren/bracket counting
def t_LPAREN(t):   r'\('; brackets.append(t);  return t
def t_RPAREN(t):   r'\)'; check_bracket('(', t); del(brackets[-1]); return t
def t_LBRACKET(t): r'\['; brackets.append(t);  return t
def t_RBRACKET(t): r'\]'; check_bracket('[', t); del(brackets[-1]); return t
def t_LBRACE(t):   r'\{'; brackets.append(t);  return t
def t_RBRACE(t):   r'\}'; check_bracket('{', t); del(brackets[-1]); return t

def t_MPROD(t):    
    r'\*+'; 
    if len(t.value) > 2:  lexer_error("so many * * * 0_o", t);
    if len(t.value) == 2: t.type = "DPROD"
    if len(t.value) == 1: t.type = "DIVMUL"
    return t

# Delimeters
t_AT            = r'\@'
t_COMMA            = r','
t_COLON            = r':'
t_APPEND           = r'(\+\=)'
t_DOUBLECOLON      = r'::'
#t_DEFOP           = r'(\=\=\=)'
t_BOOLOP           = r'((\>\=)|(\<\=)|\<|\>|(\=\=)|(\!\=))'
t_EQUALS           = r'\='
t_OR               = r'\|'
t_AMPERSAND        = r'\&'
t_DIVMUL           = r'\/'
t_QUESTION           = r'\?'
t_DOT           = r'\.'
t_PLUSMINUS        = r'\+|\-'

def t_WORD(t):
    r'[A-Za-z_]\w*'
    if t.value == 'module': t.type = "MODULE"
    if t.value == 'fn': t.type = "FN"
    if t.value == 'application': t.type = "APPLICATION"
    if t.value == 'define': t.type = "DEFINE"
    if t.value == 'print': t.type = "PRINT"
    if t.value == 'cycle': t.type = "CYCLE"
    if t.value == 'return': t.type = "RETURN"
    if t.value == 'variables': t.type = "VARIABLES"
    if t.value == 'input': t.type = "INPUT"
    if t.value == 'downlevel': t.type = "DOWNLEVEL"
    if t.value == 'execfile': t.type = "EXECFILE"
    if t.value == 'subst': t.type = "SUBST"
    if t.value == 'exectext': t.type = "EXECTEXT"
    if t.value == 'python': t.type = "PYTHON"
    if t.value == 'unless': t.type = "UNLESS"
    if t.value == 'None': t.type = "NONE"
    if t.value == 'length': t.type = "LENGTH"
    if t.value == 'else': t.type = "ELSE"
    if t.value == 'printl': t.type = "PRINTL"
    if t.value == 'in': t.type = "IN"
    if t.value == 'if': t.type = "IF"
    if t.value == 'break': t.type = "BREAK"
    if t.value == 'regex': t.type = "REGEX"
    if t.value == 'repeat': t.type = "REPEAT"
    if t.value == 'subst': t.type = "SUBST"
    if t.value == 'unvar': t.type = "UNVAR"
    if t.value == '__dirpath__': t.type = "DIRPATH"
    if t.value == '__filename__': t.type = "FILENAME"
    if t.value == 'pass': t.type = "PASS"
    if t.value == 'exfiles': t.type = "EXFILES"
    if t.value == 'evalvar': t.type = "EVALVAR"
    if t.value == 'evaltree': t.type = "EVALTREE"
    if t.value == 'var': t.type = "DEFINE"
    if t.value == 'isdir': t.type = "ISDIR"
    if t.value == 'slice': t.type = "SLICE"
    if t.value == 'listdir': t.type = "LISTDIR"
    if t.value == 'millis': t.type = "MILLIS"
    if t.value == 'yield': t.type = "YIELD"
    if t.value == 'execscan': t.type = "EXECSCAN"
    if t.value == 'and': t.type = "AND"
    if t.value == 'aplication': t.type = "APLICATION"
    if t.value == 'compile': t.type = "COMPILE"
    if t.value == '__relpath__': t.type = "RELPATH"
    if t.value == 'loop': t.type = "LOOP"
    if t.value == 'Node': t.type = "NODE"
    return t

def t_HEXNUMBER(t):
    r'(0x)[\da-fA-F]+'
    t.value = int(t.value[2:],16)
    t.type = "NUMBER"
    return t

def t_BINNUMBER(t):
    r'(0b)[\da-fA-F]+'
    t.value = int(t.value[2:],2)
    t.type = "NUMBER"
    return t

def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_STRING(t):
    r'(\"([^\"]*)\"|\'([^\']*)\')'
    t.value = str(t.value[1:-1].encode().decode("unicode_escape"))
    return t

def t_WRONGSTRING(t):
    r'\"(.*)'
    lexer_error("wrong_string", t)
    return t

    return t

def t_error(t):
    lexer_error("ill_err", t);

def t_eof(t):
    if len(brackets) > 0: lexer_error("unclosed bracket on EOF", brackets[-1]); 
    return None

lexer = ply.lex.lex()

def lexfile(file):
    lexer.lexed_file = file 
    lexer.input(file.read())
    toks = []
    while True:
        x = lexer.token()
        if x == None: break 
        toks.append(x)
    return toks