/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: a/1/I
 * Soubor:   scanner.c
 *
 * Popis:
 *
 *
 * Datum:    21.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include "scanner.h"

#define add_Token_Data( a ) ( strAppend(&(glob_Token.data), (char)a) )
#define is_num( a ) (( a >= '0' && a <= '9'))
#define is_a_z( a ) ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))

#define is_hexa( a ) (( a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'))
#define get_hexa( a ) (( a >= 9)? a - '0' : ( (a <= 'F') ? ( ( a - 'A' ) + 10 ):( ( a - 'a' ) + 10 )))

FILE* glob_FileHandler = NULL;
struct_Token glob_Token;

char* list_KeyWords[5] = {"function", "else", "if", "return", "while"};

char* list_Reserved[10] = {"boolval", "doubleval", "intval", "strval", "get_string", "put_string",
	"strlen", "get_substring", "find_string", "sort_string"};

void init_Token(){
    glob_Token.row = 1;
    glob_Token.column = 0;

    rewind(glob_FileHandler);

    strInit(&(glob_Token.data));
}

void clear_Token(){
    strFree(&(glob_Token.data));
    strInit(&(glob_Token.data));
}

void free_Token(){
	strFree(&(glob_Token.data));
}

enum_RetVal get_Token()
{
	clear_Token();	//clear Token
	
	enum_State cur_state = STATE_START;
	
	int cur_char;
	int last_char;
	
	//Hlavni nacitaci smycka
    while((cur_char = getc(glob_FileHandler)))
	{
        glob_Token.column++;
		
        if(cur_char == '\n'){
            glob_Token.row++;
            glob_Token.column = 0;
        }
        
        switch(cur_state)
		{
            case STATE_START:
                if(cur_char == EOF){ return TTYPE_EOF;}
                else if(cur_char == '('){ add_Token_Data(cur_char); return TTYPE_L_BRACKET;}
                else if(cur_char == ')'){ add_Token_Data(cur_char); return TTYPE_R_BRACKET;}
                else if(cur_char == '['){ add_Token_Data(cur_char); return TTYPE_L_SBRACKET;}
                else if(cur_char == ']'){ add_Token_Data(cur_char); return TTYPE_R_SBRACKET;}
                else if(cur_char == '{'){ add_Token_Data(cur_char); return TTYPE_L_BRACE;}
                else if(cur_char == '}'){ add_Token_Data(cur_char); return TTYPE_R_BRACE;}
                else if(cur_char == '+'){ add_Token_Data(cur_char); return TTYPE_ADDITION;}
                else if(cur_char == '-'){ add_Token_Data(cur_char); return TTYPE_SUBTRACTION;}
                else if(cur_char == '*'){ add_Token_Data(cur_char); return TTYPE_MULTIPLICATION;}
                else if(cur_char == '/'){ add_Token_Data(cur_char); cur_state = STATE_SLASH; break;}
                else if(cur_char == ';'){ add_Token_Data(cur_char); return TTYPE_SEMICOLON;}
                else if(cur_char == ','){ add_Token_Data(cur_char); return TTYPE_COMMA;}
                else if(cur_char == '$'){ add_Token_Data(cur_char); cur_state = STATE_VARIABLE; break;}
                else if(cur_char == '='){ add_Token_Data(cur_char); cur_state = STATE_ASSIGN; break;}
                else if(cur_char == '!'){ add_Token_Data(cur_char); cur_state = STATE_NOT_EQUAL; break;}
                else if(cur_char == '<'){ add_Token_Data(cur_char); cur_state = STATE_LESSER; break;}
                else if(cur_char == '>'){ add_Token_Data(cur_char); cur_state = STATE_GREATER; break;}
                else if(cur_char == '.'){ add_Token_Data(cur_char); return TTYPE_DOT;}
                else if(cur_char == '"'){ cur_state = STATE_STRING; break;}
                else if(is_a_z(cur_char)){ add_Token_Data(cur_char); cur_state = STATE_TEXT; break;}
                else if(is_num(cur_char)){ add_Token_Data(cur_char); cur_state = STATE_NUMBER; break;}
                break;
			case STATE_LESSER:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '='){
					add_Token_Data(cur_char);
                    return TTYPE_LESSER_EQUAL; }
				else if(cur_char == '?'){
					add_Token_Data(cur_char);
                    cur_state = STATE_PHP_START; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_LESSER; }
                break;
			case STATE_GREATER:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '='){
					add_Token_Data(cur_char);
                    return TTYPE_GREATER_EQUAL; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_GREATER; }
                break;
			case STATE_EQUAL:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '='){
					add_Token_Data(cur_char);
					return TTYPE_EQUAL; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_ERROR; }
                break;
			case STATE_NOT_EQUAL:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '='){
					add_Token_Data(cur_char);
					if(strCompare(glob_Token.data , "!=="))
						return TTYPE_NOT_EQUAL; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_ERROR; }
                break;
			case STATE_ASSIGN:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '='){
					add_Token_Data(cur_char);
					cur_state = STATE_EQUAL; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_ASSIGN; }
                break;
			case STATE_SLASH:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == '/'){
					clear_Token();
					cur_state = STATE_LINE_COMMENT; }
				else if(cur_char == '*'){
					clear_Token();
					cur_state = STATE_BLOCK_COMMENT; }
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_DIVISION; }
                break;
			case STATE_LINE_COMMENT:
                if(cur_char == EOF)
                    return TTYPE_EOF;
                else if(cur_char == '\n'){
					clear_Token();
					cur_state = STATE_START; }
                break;
			case STATE_BLOCK_COMMENT:
                if(cur_char == EOF)
                    return TTYPE_EOF;
                else if(cur_char == '*'){
					clear_Token();
					add_Token_Data(cur_char);}
				else if(cur_char == '/' && strCompare(glob_Token.data, "*")){
					clear_Token();
					cur_state = STATE_START; }
                break;
			case STATE_PHP_START:
                if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(cur_char == 'p' && strCompare(glob_Token.data , "<?")){
					add_Token_Data(cur_char); 
					break;}
				else if(cur_char == 'p' && strCompare(glob_Token.data , "<?ph")){		
					add_Token_Data(cur_char); 
					break;}
                else if(cur_char == 'h' && strCompare(glob_Token.data , "<?p"))
					add_Token_Data(cur_char); 
				else if((cur_char == '\n' || cur_char == ' ' || cur_char == '\t') && strCompare(glob_Token.data , "<?php"))
					return TTYPE_PHP_START; 
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_ERROR;}
                break;
			case STATE_VARIABLE:
				if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(is_a_z(cur_char) || is_num(cur_char) || cur_char == '_')
				{
					if((cur_char >= '0' && cur_char <= '9') && strCompare(glob_Token.data , "$"))
						return TTYPE_ERROR;
					add_Token_Data(cur_char);
				}
                else{
                    ungetc(cur_char, glob_FileHandler);
                    return TTYPE_VARIABLE; }
                break;
			case STATE_STRING:
				if(cur_char == EOF || cur_char < 31)
                    return TTYPE_ERROR;
				else if(cur_char == '\\')
				{
					if(last_char == '\\')
					{
						add_Token_Data('\\');
						last_char = 0;
					}
					else
						last_char = '\\';
				}
				else if(cur_char == '$' && last_char != '\\')
                    return TTYPE_ERROR;
				else if(cur_char == 'n' && last_char == '\\')
				{
					add_Token_Data('\n');
					last_char = 0;
				}
				else if(cur_char == 't' && last_char == '\\')
				{
					add_Token_Data('\t');
					last_char = 0;
				}
				else if(cur_char == '"' && last_char == '\\')
				{
					add_Token_Data('\"');
					last_char = 0;
				}
				else if(cur_char == '$' && last_char == '\\')
				{
					add_Token_Data('$');
					last_char = 0;
				}
				else if(cur_char == '"' && last_char != '\\')
				{
					return TTYPE_STRING;
				}
				else if(cur_char == 'x' && last_char == '\\')
				{
					int tmp1 = getc(glob_FileHandler);
					int tmp2 = getc(glob_FileHandler);
					
					if(!is_hexa(tmp1) || !is_hexa(tmp2))
						return TTYPE_ERROR;
					
					tmp1 = get_hexa(tmp1);
					tmp2 = get_hexa(tmp2);
					tmp1 = tmp1 * 16 + tmp2;
					
					add_Token_Data(tmp1);
					
					last_char = 0;
				}
				else
				{
					if(last_char == '\\')
					{
						add_Token_Data('\\');
						last_char = 0;
					}
					add_Token_Data(cur_char);
				}
                break;
			case STATE_TEXT:
				if(cur_char == EOF)
                    return TTYPE_ERROR;
                else if(is_a_z(cur_char) || is_num(cur_char) || cur_char == '_')
				{
					add_Token_Data(cur_char);
				}
                else{
                    ungetc(cur_char, glob_FileHandler);
					
					for(int i=0;i<5;i++)
					{
						if(strCompare(glob_Token.data, list_KeyWords[i]))
							return TTYPE_KEYWORD;
					}
					
					for(int i=0;i<5;i++)
					{
						if(strCompare(glob_Token.data, list_Reserved[i]))
							return TTYPE_RESERVED;
					}
					
					if(strCompare(glob_Token.data, "true"))
						return TTYPE_TRUE;
					if(strCompare(glob_Token.data, "false"))
						return TTYPE_FALSE;
					if(strCompare(glob_Token.data, "null"))
						return TTYPE_NULL;
					
					return TTYPE_FUNCTION;}
                break;
			case STATE_NUMBER:
				if(cur_char == EOF)
                    return TTYPE_ERROR;
				else if(is_num(cur_char))
					add_Token_Data(cur_char);
				else if(cur_char == '.'){
					cur_state = STATE_NUMBER_DOT;
					add_Token_Data(cur_char); }
				else if(cur_char == 'e' || cur_char == 'E'){
					cur_state = STATE_NUMBER_EXP;
					add_Token_Data(cur_char); }
				else
				{
					ungetc(cur_char, glob_FileHandler);
					return TTYPE_NUMBER;
				}
                break;
			case STATE_NUMBER_DOT:
				if(cur_char == EOF)
                    return TTYPE_ERROR;
				else if(is_num(cur_char))
					add_Token_Data(cur_char);
				else if(cur_char == 'e' || cur_char == 'E'){
					cur_state = STATE_NUMBER_EXP;
					last_char = cur_char;
					add_Token_Data(cur_char); }
				else
				{
					ungetc(cur_char, glob_FileHandler);
					return TTYPE_DEC_NUMBER;
				}
                break;
			case STATE_NUMBER_EXP:
				if(cur_char == EOF)
                    return TTYPE_ERROR;
				else if((last_char == 'e' || last_char == 'E') && (cur_char == '+' || cur_char == '-'))
				{
					last_char = cur_char;
					add_Token_Data(cur_char);
				}
				else if(is_num(cur_char))
					add_Token_Data(cur_char);
				else
				{
					ungetc(cur_char, glob_FileHandler);
					if((last_char == 'e' || last_char == 'E') || (last_char == '+' || last_char == '-'))
						return TTYPE_ERROR;
					char* tmp1;
					double tmp2 = strtod(glob_Token.data.data, &tmp1);//(double)glob_Token.data.data;
					STRING tmp3;
					char* pole;
					sprintf(pole,"%f",tmp1);
					strInitRaw(&tmp3, pole);
					strCopy(glob_Token.data.data,&tmp3);
					return TTYPE_DEC_ NUMBER;
				}
                break;
            default:
                return TTYPE_ERROR;
                break;
        }
	}
	return TTYPE_ERROR;
}

//error wrapper
enum_RetVal getToken(){
	enum_RetVal retval = get_Token();
	
	if(retval == TTYPE_ERROR)
		ChuckSaid(E_LEXICAL, "Neplatny lexem");
		
	return retval;
}

enum_RetVal get_Next_Token()
{
	// backup token data and file position
	struct_Token backup_token = glob_Token;
	STRING data;
	strCopy(&glob_Token.data, &data);

	unsigned int pos = ftell(glob_FileHandler);
	
	// call get_Token();
	enum_RetVal retval = get_Token();
	
	// set back file position and token data	
	fseek (glob_FileHandler, pos, SEEK_SET);
	
	strFree(&glob_Token.data);
	backup_token.data = data;
	glob_Token = backup_token;

	return retval;
}

enum_RetVal getNextToken(){
	enum_RetVal retval = get_Next_Token();
	
	if(retval == TTYPE_ERROR)
		ChuckSaid(E_LEXICAL, "Neplatny lexem");
		
	return retval;
}