#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //this include is for all the function calls I will be using thats built in C, such as isdigit(), isalpha(), isalnum(), 
int end = 0;
struct TokenizerT_ {
	int pindex; //pointer to the index of current array
	char *str; //string input
};


typedef struct TokenizerT_ TokenizerT;



TokenizerT *TKCreate( char * ts ) {
	//starts to create allocation for tokenizer
	TokenizerT *function = (TokenizerT*)malloc(sizeof(TokenizerT));
	function->str =(char*)malloc(sizeof(char)*(1+(strlen(ts)))); //allocates space for the array of str
	strcpy(function->str, ts); //copies string from user input into string input field
	function->pindex = 0;//set our currrent pointer position to 0 so we can use later to traverse the whole string
  return function;
}



void TKDestroy( TokenizerT * tk ) {
	free(tk->str);
	free(tk);
	return;
}

char *getFloat(TokenizerT * tk, char* tmp, int charIndex){
	int ecount = 0; // makes sure e notation will only appear once
	tmp[charIndex] = tk->str[tk->pindex];
	charIndex++;
	tk->pindex++;
	while(isdigit(tk->str[tk->pindex]) || tk->str[tk->pindex] == 'e'){
		if(tk->str[tk->pindex] == 'e'){
			if(ecount == 1){
				break;
			}
			ecount = 1;
			if(isdigit(tk->str[tk->pindex + 1]) || tk->str[tk->pindex + 1] == '-'){
				if(isdigit(tk->str[tk->pindex + 2])){
					tmp[charIndex] = tk->str[tk->pindex];
					charIndex++;
					tk->pindex++;
					tmp[charIndex] = tk->str[tk->pindex];
					charIndex++;
					tk->pindex++;
				}else{
					break;
				}
			}else{
				break;
			}
		}else{
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
		}
	}
	printf("float \"%s\"\n", tmp);
	return tmp;
}

char *getHex(TokenizerT * tk, char* tmp, int charIndex){
	tmp[charIndex] = tk->str[tk->pindex];
	charIndex++;
	tk->pindex++;
	while(isdigit(tk->str[tk->pindex]) || tk->str[tk->pindex] == 'a' || tk->str[tk->pindex] == 'b' || tk->str[tk->pindex] == 'c' || tk->str[tk->pindex] == 'd' || tk->str[tk->pindex] == 'e' || tk->str[tk->pindex] == 'f'){
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
	}
	printf("hex integer \"%s\"\n", tmp);
	return tmp;
}

char *getOctal(TokenizerT * tk, char* tmp, int charIndex){
	tmp[charIndex] = tk->str[tk->pindex];
	charIndex++;
	tk->pindex++;
	while(tk->str[tk->pindex] == '0' || tk->str[tk->pindex] == '1' || tk->str[tk->pindex] == '2' || tk->str[tk->pindex] == '3' || tk->str[tk->pindex] == '4' || tk->str[tk->pindex] == '5' || tk->str[tk->pindex] == '6' || tk->str[tk->pindex] == '7'){
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
	}
	printf("octal integer \"%s\"\n", tmp); 
	return tmp;
}
char *getNumber(TokenizerT * tk, char* tmp, int charIndex){
	while(isdigit(tk->str[tk->pindex])){
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
	}
	if(tk->str[tk->pindex] == '.' && isdigit(tk->str[tk->pindex + 1])){ // checks for float
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
		tmp = getFloat(tk, tmp, charIndex);
		return tmp;
	}
	printf("decimal \"%s\"\n", tmp);
	return tmp;
}

char *getWord(TokenizerT * tk, char* tmp, int charIndex){
	while(isalnum(tk->str[tk->pindex])){
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
	}
	printf("word \"%s\"\n", tmp);
	return tmp;
}

char *getOp(TokenizerT * tk, char* tmp, int charIndex){
	tmp[charIndex] = tk->str[tk->pindex];
	if(tk->str[tk->pindex] == '>'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '>'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			if(tk->str[tk->pindex] == '='){
				tmp[charIndex] = tk->str[tk->pindex];
				charIndex++;
				tk->pindex++;
				printf("right-shift assignment \"%s\"\n", tmp);	
			}else{
				charIndex++;
				tk->pindex++;
				printf("shift right \"%s\"\n", tmp);
			}
		}else if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			printf("greater or equal \"%s\"\n", tmp);
		}else{
			printf("greater than \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '<'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '<'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			if(tk->str[tk->pindex] == '='){
				tmp[charIndex] = tk->str[tk->pindex];
				charIndex++;
				tk->pindex++;
				printf("left shift assignment \"%s\"\n", tmp);	
			}else{
				charIndex++;
				tk->pindex++;
				printf("left shift \"%s\"\n", tmp);
			}
		}else if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("less or equal \"%s\"\n", tmp);
		}else{
			printf("less than \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '='){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("equals \"%s\"\n", tmp);
		}else{
			printf("simple assignment \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '!'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("not equals \"%s\"\n", tmp);
		}else{
			printf("negate \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '&'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '&'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("logical and \"%s\"\n", tmp);
		}else if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("bitwise and assignment \"%s\"\n", tmp);
		}else{
			printf("address or bitwise and \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '|'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '|'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("logical or \"%s\"\n", tmp);
		}else if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("bitwise inclusive or assignment \"%s\"\n", tmp);
		}else{
			printf("bitwise or \"%s\"\n", tmp);
		} 
	}else if(tk->str[tk->pindex] == '^'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("bitwise exclusive or assignment \"%s\"\n", tmp);
		}else{
			printf("bitwise exclusive or \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '+'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '+'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("inc \"%s\"\n", tmp);
		}else if(tk->str[tk->pindex] =='='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("addition assignment \"%s\"\n", tmp);
		}else{
			printf("add \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '-'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '-'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("dec \"%s\"\n", tmp);
		}else if(tk->str[tk->pindex] =='='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("subtraction assignment \"%s\"\n", tmp);
		}else if(tk->str[tk->pindex] == '>'){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("structure pointer \"%s\"\n", tmp);
		}else{
			printf("minus \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '*'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("multiplication assignment \"%s\"\n", tmp);
		}else{
			printf("multiply or indirect \"%s\"\n", tmp);
		}

	}else if(tk->str[tk->pindex] == '/'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("division assignment \"%s\"\n", tmp);
		}else{
			printf("divide \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '%'){
		charIndex++;
		tk->pindex++;
		if(tk->str[tk->pindex] == '='){
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			printf("remainder assignment \"%s\"\n", tmp);
		}else{
			printf("modulus \"%s\"\n", tmp);
		}
	}else if(tk->str[tk->pindex] == '('){
		charIndex++;
		tk->pindex++;
		printf("left parentheses \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == ')'){
		charIndex++;
		tk->pindex++;
		printf("right parentheses \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == '['){
		charIndex++;
		tk->pindex++;
		printf("left brace \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == ']'){
		charIndex++;
		tk->pindex++;
		printf("right brace \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == '{'){
		charIndex++;
		tk->pindex++;
		printf("left bracket \"%s\"\n", tmp);				
	}else if(tk->str[tk->pindex] == '}'){
		charIndex++;
		tk->pindex++;
		printf("right bracket \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == '~'){
		charIndex++;
		tk->pindex++;
		printf("1's comp \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == '.'){
		charIndex++;
		tk->pindex++;
		printf("structure member \"%s\"\n", tmp);
	}else if(tk->str[tk->pindex] == ','){
		charIndex++;
		tk->pindex++;
		printf("comma \"%s\"\n", tmp);
	}else{
		charIndex++;
		tk->pindex++;
		printf("bad token \"%s\"\n", tmp);
	}

	return tmp;
}


char *TKGetNextToken( TokenizerT * tk ) {
	
	if(tk->str[tk->pindex] == ' ' || tk->str[tk->pindex] == '\t' || tk->str[tk->pindex] == '\v' || tk->str[tk->pindex] == '\f' || tk->str[tk->pindex] == '\n' || tk->str[tk->pindex] == '\r'){
		tk->pindex++;
		return NULL;
	}

	int charIndex = 0;
	char *tmp = (char*)malloc(sizeof(char)*strlen(tk->str));
	if(isalnum(tk->str[tk->pindex])){
		if(isalpha(tk->str[tk->pindex])){ //checks for word
			tmp[charIndex] = tk->str[tk->pindex];
			charIndex++;
			tk->pindex++;
			tmp = getWord(tk, tmp, charIndex);
			return tmp;
		}else{
			if(tk->str[tk->pindex] == '0' && isalnum(tk->str[tk->pindex + 1])){ //checks for hex
				if(tk->str[tk->pindex + 1] == 'x' || tk->str[tk->pindex + 1] == 'X'){
					if(isdigit(tk->str[tk->pindex + 2]) || tk->str[tk->pindex + 2] == 'a' || tk->str[tk->pindex + 2] == 'b' || tk->str[tk->pindex + 2] == 'c' || tk->str[tk->pindex + 2] == 'd' || tk->str[tk->pindex + 2] == 'e' || tk->str[tk->pindex + 2] == 'f'){ 
						tmp[charIndex] = tk->str[tk->pindex];
						charIndex++;
						tk->pindex++;
						tmp = getHex(tk, tmp, charIndex);
						return tmp;
					}
				}else if(tk->str[tk->pindex + 1] == '0' || tk->str[tk->pindex + 1] == '1' || tk->str[tk->pindex + 1] == '2' || tk->str[tk->pindex + 1] == '3' || tk->str[tk->pindex + 1] == '4' || tk->str[tk->pindex + 1] == '5' || tk->str[tk->pindex + 1] == '6' || tk->str[tk->pindex + 1] == '7'){ // checks for octal
					tmp[charIndex] = tk->str[tk->pindex];
					charIndex++;
					tk->pindex++;
					tmp = getOctal(tk, tmp, charIndex);
					return tmp;
				}
			}
		}
		//if not sends to number(decimal or floats)
		tmp[charIndex] = tk->str[tk->pindex];
		charIndex++;
		tk->pindex++;
		tmp = getNumber(tk, tmp, charIndex);
		return tmp;	
	}else{
		// C-Operators
		tmp = getOp(tk, tmp, charIndex);
	}

	return tmp;
}




int main(int argc, char **argv) {
	
	
	//Starts with error check if no string at all is inputted return print.

	if (argc ==1){
		printf("no String inputted\n");
		return 0;
	}
	if (argc ==3) {
		printf("Please input ONE set of strings \n");
		return 0;	

	}
		
	char* string= argv[1]; 
	TokenizerT *finaltok = TKCreate(string);  //creates the string array into stream
	char* datToke; //char array that is sent over after TKGetNextToken
	while(finaltok->str[finaltok->pindex] != '\0'){
		datToke = TKGetNextToken(finaltok);
	}
	TKDestroy(finaltok);
	free(datToke);
  return 0;
}
