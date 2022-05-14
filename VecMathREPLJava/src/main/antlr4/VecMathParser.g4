parser grammar VecMathParser;
@header{package dae.gd;}

options{tokenVocab=VecMathLexer;}

expression : (assign | command) EOF;

assign : ID ASSIGN value;

quaternion: LBRACKET w=value COMMA LPAREN x=value COMMA y=value COMMA z=value RPAREN RBRACKET;
vector : LBRACKET value (COMMA value)* RBRACKET;
literal: FLOAT|vector|quaternion;
value: 
 op=ABS value ABS
 | op1=value op=POWER op2=value
 | op1=value op=(MUL | DIV | DOT) op2=value  
 | op1=value op=(ADD | MINUS | CROSS) op2=value
 | op=(ADD|MINUS) op1=value
 | (f=function)? LPAREN op1=value RPAREN
 | literal
 | ID;

function: SIN|COS|TAN|ASIN|ACOS|ATAN|RADTODEG|DEGTORAD|CON|SQRT|ID;
command : print | PRINTALL | EXIT | JOKE | clear | precision | HELP;
print: PRINT ID;
precision: PRECISION FLOAT;
clear: CLEAR ID?;