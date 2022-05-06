lexer grammar VecMathLexer;
@header{package dae.gd;}

channels{
    whitespace
}

WHITESPACE: [ \t]+ -> channel(whitespace);

ASSIGN: '=' ;
LBRACKET: '[' ;
RBRACKET: ']' ;
LPAREN: '(';
RPAREN: ')';
COMMA: ',' ; 

ADD: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
DOT: '.';
ABS: '|';
POWER: '^';

COS: 'cos';
SIN: 'sin';
TAN: 'tan';
ACOS: 'acos';
ASIN: 'asin';
ATAN: 'atan';
DEGTORAD: 'deg2rad';
RADTODEG: 'rad2deg';
CON : 'con';
SQRT: 'sqrt';

PRINT: 'print';
PRINTALL: 'printAll';
CLEAR: 'clear';
EXIT: 'bye' | 'exit';
HELP: 'help';
 
ID : [a-zA-Z]+ [a-zA-Z0-9]* ;
FLOAT: [0-9]+ ('.'  [0-9]+)?;
