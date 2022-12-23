from ast import keyword
from ctypes.wintypes import CHAR
from lib2to3.pgen2 import token
import re
from enum import Enum
from typing import List
fileName = "input_sourcecode.txt"

class Token(Enum):
    IDENTIFIER = 0
    INTEGER = 1
    OPERATOR = 2
    KEYWORD = 3
    LITERAL = 4
    COMMENT = 5

lexemeTerminators = [ '(', '{', '}', ' ', ')', ';', '\n', ','] 

re_identifier = "[a-z]|\_([a-zA-Z0-9]|-|_)*"
re_integer = "([0-9]|-)[0-9]*"
re_operator = "=|==|\+|-|>|>=|<|<=|>>|<<"
re_keyword = "for|while|if|else|int|float|cout|return"

def identify_token(lexeme: str) -> Token:
    regex_dict = {
        "keyword": re_keyword,
        "identifier": re_identifier, 
        "integer": re_integer,
        "operator": re_operator,
    }
    for key, val in regex_dict.items():
        mth = re.search(val, lexeme)
        if mth == None:
            continue
        else:
            if key == "identifier":
                return Token.IDENTIFIER
            elif key == "operator":
                return Token.OPERATOR
            elif key == "integer":
                return Token.INTEGER
            elif key == "keyword":
                return Token.KEYWORD

def lexer(string: str) -> List[Token]:
    tokens = []
    inQuote = False
    previousCharacter = ""
    inComment = False
    lexeme = ""
    i = 0
    while i < len(string):
        if string[i] == '\"':
            if(inQuote): # Received second quote
                lexeme += string[i]
                stringToken = (Token.LITERAL,lexeme)
                tokens.append(stringToken)
                lexeme = ""
                i += 1
                inQuote = False
            else:
                inQuote = True
        elif string[i] == "/" and previousCharacter == "/" and not inQuote:
            inComment = True  
        elif string[i] == "\n" and inComment:
            commentToken = (Token.COMMENT,lexeme)
            tokens.append(commentToken)
            lexeme = ""
            inComment = False
        elif string[i] in lexemeTerminators and not inQuote and not inComment:
            if len(lexeme) > 0:
                tokens.append((identify_token(lexeme), lexeme))
                lexeme = ""
        if string[i] not in lexemeTerminators or inQuote or inComment:
            lexeme += string[i]
        previousCharacter = string[i]
        i += 1
    return tokens

def output(tokensList: list) -> str:
    output = "Tokens               Lexeme\n"
    output +="---------------------------\n"
    for tuplePair in tokensList:
        line = f"{str(tuplePair[0]):16} ---> {str(tuplePair[1]):10}"
        output += line
        output += "   "
        output += "\n"
    return output

def main():
    with open(fileName) as f:
        tokens = lexer(f.read())
    resultOutput = output(tokens)
    print(resultOutput)
    output_file = open("output.txt", "w")
    n = output_file.write(resultOutput)
    output_file.close()

if __name__ == "__main__":
    main()