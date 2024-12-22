#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define RED   "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[35m"
char *keywords[] = {"int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
                    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"};
static const char *operators = "+-*/%=!:<>|&,;(){}[]";

int is_keyword(char str[]) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;  // Found a keyword
        }
    }
    return 0;  // Not a keyword
}

int is_operator(char ch) {
    return (strchr(operators, ch) != NULL);  // Returns 1 if operator, 0 otherwise
}
int check_type_number(char str[])
    {
        if(str[0] == '0' && isdigit(str[1]))
        {
            if(str[1] <= '7' && str[2] <= '7' && str[3] <= '7' && strlen(str) <= 4)
            {
              printf(GREEN"Octal : "RESET"%s\n",str);
            }
            else
            {
              printf(RED"It is not an octal number\n");
              exit(0);
            }
        }
        else if(str[1] == 'x' || str[1] == 'X')
        {
            if((str[2] <= 'f' || str[2] <= 'F') && (str[3] <= 'f' || str[3] <= 'F' )&& (str[4] <= 'f' || str[4] <= 'F' )&&( str[5] <= 'f' || str[5] <= 'F') && strlen(str) <= 6 )
            {
            printf(GREEN"Hexadecimal : "RESET"%s\n",str);
            }
            else
            {
            printf(RED"It is not an hexadecimal number\n");
            exit(0);
            }
        }
        else if(str[0] == '0' && (str[1] == 'b'|| str[1] == 'B'))
        {
            int flag=1;
            for(int i=2;str[i];i++)
            {
            if(str[i] != '0' && str[i] !='1')
            flag=0;
            break;
            }
            if(flag)
            printf(GREEN"Binary : "RESET"%s\n",str);
            else{
            printf(RED"It is not a binary number\n");
            exit(0);
           }
        }
        else if(strchr(str,'.')!=NULL)
        {    
            int count_d=0;
            int count_f=0;
            int count_a_z = 0;
            int n=strlen(str);
            for(int i=0;str[i];i++)
            {
               if(str[i] == 'f')
               {
                count_f++;
               }
               if(str[i] == '.')
               {
                count_d++;
               }
               if((str[i] >= 'a' && str[i] < 'f') || (str[i] >= 'g' && str[i] <= 'z'))
               {
                 count_a_z++;
               }
            }
            if(count_f >= 2 || count_d >= 2 || count_a_z >= 1)
            {
                printf(RED"It is not a float value\n");
                exit(0);
            }
            else if((strchr(str,'f')!=NULL))
            {
                 printf(GREEN"float : "RESET"%s\n",str);
            }
            else if((str[n-1] >= 'a' && str[n-1] < 'f') || (str[n-1] >= 'g' && str[n-1] <= 'z'))
            {
                printf(RED"It is not a float value\n");
                exit(0);
            }
            else if((strchr(str,'.')!=NULL))
            {
                printf(GREEN"float : "RESET"%s\n",str);
            }
        }
        else if(isdigit(str[0]) && isdigit(str[1]))
        {
            printf(GREEN"Numeric constant : "RESET"%s\n",str);
        }
    }

int validate_parentheses(FILE *ptr) {
    int round = 0;  // Counter for '(' and ')'
    int curly = 0;  // Counter for '{' and '}'
    int square = 0; // Counter for '[' and ']'
    char ch;

    while ((ch = fgetc(ptr)) != EOF) {
        if (ch == '(') {
            round++;
        } else if (ch == ')') {
            round--;
            if (round < 0) {  // Closing parenthesis without matching opening
                printf(RED"Error: Unmatched closing parenthesis ')'\n"RESET);
                return 0;
            }
        } else if (ch == '{') {
            curly++;
        } else if (ch == '}') {
            curly--;
            if (curly < 0) {  // Closing curly brace without matching opening
                printf(RED"Error: Unmatched closing brace '}'\n"RESET);
                return 0;
            }
        } else if (ch == '[') {
            square++;
        } else if (ch == ']') {
            square--;
            if (square < 0) {  // Closing square bracket without matching opening
                printf(RED"Error: Unmatched closing bracket ']'\n"RESET);
                return 0;
            }
        }
    }

    // Check for any unmatched opening parentheses
    if (round > 0) {
        printf(RED"Error: Unmatched opening parenthesis '('\n"RESET);
        return 0;
    }
    if (curly > 0) {
        printf(RED"Error: Unmatched opening brace '{'\n"RESET);
        return 0;
    }
    if (square > 0) {
        printf(RED"Error: Unmatched opening bracket '['\n"RESET);
        return 0;
    }

    return 1;  // All parentheses are matched
}
int main() {
    FILE *ptr = fopen("example.c", "r");
    if (ptr == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }


   // rewind(ptr);  // Ensure the file pointer is at the start
    if (!validate_parentheses(ptr)) {
        fclose(ptr);
        return 1;  // Exit if there are unmatched parentheses
    }

     rewind(ptr);
    char buffer[100];
    int i = 0;
    char ch;
    int code_flag;
    while ((ch = fgetc(ptr)) != EOF) {
        if(ch == '#')
        {
          while(( ch=fgetc(ptr)!='\n') && ch !=EOF);  
        }
        else if (isalpha(ch)) {
            // Collect an identifier or keyword
            buffer[i++] = ch;
            while ((ch = fgetc(ptr)) != EOF && (isalnum(ch) || ch == '_')) {  //isalnum(ch) used to check if a given character is alphanumeric, meaning it’s either a letter (A-Z or a-z) or a digit (0-9).
                buffer[i++] = ch; 
            }
            buffer[i] = '\0';  // Null-terminate the string
            i = 0;             // setting i =0 for next token

            if (is_keyword(buffer)) {
                printf(RED"Keyword: "RESET"%s\n", buffer);
            } else {
                printf(CYAN"Identifier: "RESET"%s\n", buffer);
                
            }
            // If we read a non-alphanumeric character, put it back for further processing
           ungetc(ch, ptr);

        } else if(isdigit(ch)) {
           // Collect a number
             buffer[i++]=ch;
               while ((ch = fgetc(ptr)) != EOF && ch != ';') {
                buffer[i++] = ch;
               }
            buffer[i] = '\0';
            i= 0;
             check_type_number(buffer);
            // Put back the non-digit character
            ungetc(ch, ptr);
        }
        else if(ch=='/'){
          char ch1=fgetc(ptr);
          if(ch1 == '*'){
            while(( (ch=fgetc(ptr))!='/')  && ch !=EOF);
          }
          else if( ch1 =='/' ){
            while(( ch=fgetc(ptr)!='\n') && ch !=EOF);
          }
          else  {
                    ungetc(ch1, ptr);
                    printf(BLUE"Operator: "RESET"/\n");
                }
         }
    else if (ch == '\'' || ch == '"') 
    {
    char quoteType = ch;      // Store the type of quote (' or ") to distinguish literals
    int i = 0;                // Reset buffer index
    char ch1;
    
    while ((ch1 = fgetc(ptr)) != EOF) 
    {
        if (ch1 == quoteType)  // Closing quote found
        {
            buffer[i] = '\0';

            if (quoteType == '\'') 
            {
                    printf(GREEN"Character Constant: "RESET"'%s'\n", buffer);
            } 
            else  // For string literal
            {
                printf(GREEN"Literal: "RESET"\"%s\"\n", buffer);
            }
            break;
        } 
        else if(ch1 == ';' || ch1 == EOF)  // Missing closing quote
        {
            printf(RED"Error: Missing closing %c for literal\n"RESET, quoteType);
            exit(0);
        }
        else 
        {
            buffer[i++] = ch1;
        }
    }
}
    else if(is_operator(ch)){
            buffer[i++] = ch;
            while ((ch = fgetc(ptr)) != EOF && is_operator(ch)){
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            i= 0;
            printf(BLUE"Operator: "RESET"%s\n", buffer);
            ungetc(ch,ptr);  
        }
       
    }

    fclose(ptr);
    return 0;
}
