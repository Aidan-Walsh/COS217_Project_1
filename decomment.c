/*--------------------------------------------------------------------*/
/* decomment.c                                                        */
/* Author: Aidan Walsh                                                */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum Statetype {START, DQUOTE, QUOTE, ESCAPE, DESCAPE, FIRSTSLASH, FIRSTSTAR, NEXTSTAR}; 


enum Statetype handleStartState(int x) {
    enum Statetype state;
    if (x == '"') {
        putchar(x); 
        state = DQUOTE; 
    }
    else if (x == '\'') {
        putchar(x); 
        state = QUOTE; 
    }
    else if (x == '/') {
        state = FIRSTSLASH; 
    }
    else {
        putchar(x); 
        state = START; 
    }
    return state; 

}
/* enum Statetype handleReg_CharacterState(int x) {
    enum Statetype state;
    
    
} */
enum Statetype handleDQuoteState(int x) {
    enum Statetype state;
    if (x == '\\') {
        putchar(x);
        state = DESCAPE; 
    }
    else if (x == '"') {
        putchar(x); 
        state = START; 
    }
    else {
        putchar(x); 
        state = DQUOTE; 
    }
    return state; 
}
enum Statetype handleDEscape(int x) {
    enum Statetype state;
    putchar(x); 
    state = DQUOTE;
    return state;  
}
enum Statetype handleEscape(int x) {
    enum Statetype state;
    putchar(x); 
    state = QUOTE; 
    return state; 
}
enum Statetype handleQuoteState(int x) {
    enum Statetype state;
    if (x == '\\') {
        putchar(x); 
        state = ESCAPE; 
    }
    else if (x == '\'') {
        putchar(x); 
        state = START; 
    }
    else {
        putchar(x); 
        state = QUOTE; 
    }
    return state; 
}
    

enum Statetype handleFirstSlashState(int x) {
    enum Statetype state;
    if (x == '*') {
        putchar(' ');
        state = FIRSTSTAR; 
    }
    else if (x == '"') {
        putchar('/'); 
        putchar(x); 
        state = DQUOTE; 
    }
    else if (x == '\'') {
        putchar('/'); 
        putchar(x); 
        state = QUOTE; 
    }
    else if (x == '/') {
        putchar(x); 
        state = FIRSTSLASH; 
    }
    else {
        putchar('/');
        putchar(x); 
        state = START; 
    }

    return state; 
}
enum Statetype handleFirstStarState(int x) {
    enum Statetype state;
    if (x == '*') {
        state = NEXTSTAR;
    }
    else if (x == '\n') {
        putchar(x); 
        state = FIRSTSTAR; 
    }
    else {
        state = FIRSTSTAR; 
    }
    return state; 
}
enum Statetype handleNextStarState(int x) {
    enum Statetype state;
    if (x == '/') {
        state = START; 
    }
    else if (x == '*') {
        state = NEXTSTAR; 
    }
    else if (x == '\n') {
        putchar(x);
        state = FIRSTSTAR; 
    }
    else {
        state = FIRSTSTAR; 
    }
    return state; 
    
}
int main(void) {
    int c; 
    int lineCount=1; 
    enum Statetype state = START; 
    while ((c = getchar())!= EOF) {
        if(c == '\n') {
            lineCount++; 
            printf("new");
        }
        
        switch(state) {
            case START:
                state = handleStartState(c); 
                break; 
         /*   case REG_CHARACTER:
                state = handleReg_CharacterState(c); 
                break;  */
            case DQUOTE:
                state = handleDQuoteState(c);
                break; 
            case QUOTE:
                state = handleQuoteState(c);
                break;
            case FIRSTSLASH:
                state = handleFirstSlashState(c);
                break; 
            case FIRSTSTAR:
                state = handleFirstStarState(c);
                break; 
            case NEXTSTAR:
                state = handleNextStarState(c); 
                break; 
            case ESCAPE:
                state = handleEscape(c); 
                break; 
            case DESCAPE:
                state = handleDEscape(c); 
                break; 
        }
    }
    if (state == FIRSTSLASH) {
        putchar('/');
    }
    if (state == FIRSTSTAR || state == NEXTSTAR) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", lineCount);
        exit(EXIT_FAILURE); 
    }
    else {
        return 0; 
    }
 
   
}