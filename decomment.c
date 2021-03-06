/*--------------------------------------------------------------------*/
/* decomment.c                                                        */
/* Author: Aidan Walsh                                                */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* enumeration of all of the states of the DFA */
enum Statetype {START, DQUOTE, QUOTE, ESCAPE, DESCAPE, FIRSTSLASH,
                FIRSTSTAR, NEXTSTAR}; 

/* implement the START state of the DFA. x is the current DFA character.
It always writes x except if it is a forward slash since it may be a
 comment. Return the next state. */
enum Statetype handleStartState(int x) {
    enum Statetype state;

    /* if we have quotes, we write and go to the double quote state */
    if (x == '"') {
        putchar(x); 
        state = DQUOTE; 
    }

    /* if we have quotes, we write and go to the single quote state */
    else if (x == '\'') {
        putchar(x); 
        state = QUOTE; 
    }

    /* if we have slash, it could be the start of a comment, so go to
       the first slash state */
    else if (x == '/') {
        state = FIRSTSLASH; 
    }

    /* write for all other cases and remain in state*/
    else {
        putchar(x); 
        state = START; 
    }
    return state; 

}

/* implement the DQUOTE state of the DFA. x is the current DFA
   character. It always writes x since in quotes everything in written.
   Return the next state. */
enum Statetype handleDQuoteState(int x) {
    enum Statetype state;

    /* if we have a backslash in quotes, we are escaping something so
       write and go to corresponding escape state */
    if (x == '\\') {
        putchar(x);
        state = DESCAPE; 
    }


    /* if we have quotes, we write and know it's the end of a pair of 
       quotes, so go back to start state */
    else if (x == '"') {
        putchar(x); 
        state = START; 
    }

    /* always write for all other characters when in quotes and remain
       in this state */
    else {
        putchar(x); 
        state = DQUOTE; 
    }
    return state; 
}

/* implement the DESCAPE state of the DFA. x is the current DFA
   character. This is for the rare case of an escape character in double
   quotes. This ensures that escaped character x is always written.
   Return the next state. */
enum Statetype handleDEscape(int x) {
    enum Statetype state;

    /* something is being escaped, so always write, then return to 
       double quote state where we came from */
    putchar(x); 
    state = DQUOTE;
    return state;  
}

/* implement the ESCAPE state of the DFA. x is the current DFA 
character. This is for the rare case of an escape character in single
 quotes. Always write x and return to previous state. */
enum Statetype handleEscape(int x) {
    enum Statetype state;

    putchar(x); 
    state = QUOTE; 
    return state; 
}

/* implement the QUOTE state of the DFA. x is the current DFA character.
Always write x since we are in quotes. Return the next state. */
enum Statetype handleQuoteState(int x) {
    enum Statetype state;

     /* if we have a backslash in quotes, we are escaping something
        so write and go to corresponding escape state */
    if (x == '\\') {
        putchar(x); 
        state = ESCAPE; 
    }

    /* if we have the same single quotes, we write and know it's the
       end of a pair of quotes, so go back to start state */
    else if (x == '\'') {
        putchar(x); 
        state = START; 
    }
    else {
        
        /* all other cases we just write and remain in this state */
        putchar(x); 
        state = QUOTE; 
    }
    return state; 
}
    
/* implement the FIRSTSLASH state of the DFA. x is the current DFA 
character. Most ways require writing x but also a forward slash if it
 is not a comment. Return the next state. */
enum Statetype handleFirstSlashState(int x) {
    enum Statetype state;

    /* if we read a star we are in a 
       comment and so write a blank space, go to the next stat */
    if (x == '*') {
        putchar(' ');
        state = FIRSTSTAR; 
    }
    /* quotes after slash so may have to write the backslash then the
       double quotes, and go to the quote state */
    else if (x == '"') {
        putchar('/'); 
        putchar(x); 
        state = DQUOTE; 
    }
    /* have quotes after slash, write the backslash then 
       the single quotes, and go to the double quote state */
    else if (x == '\'') {
        putchar('/'); 
        putchar(x); 
        state = QUOTE; 
    }
    /* may have slash after slash so 
       write the backslash then, go to first slash state */
    else if (x == '/') {
        putchar(x); 
        state = FIRSTSLASH; 
    }
    /* we write the backslash we just read since no
       comment, and write x */
    else {
        putchar('/');
        putchar(x); 
        state = START; 
    }

    return state; 
}

/* implement the FIRSTSTAR state of the DFA. x is the current DFA 
character. This is for the case that we have read a star after a forward
slash, so we are in a comment. We never write x unless it is a newline 
to keep track of the new lines. Return the next state. */
enum Statetype handleFirstStarState(int x) {
    enum Statetype state;
    /* if we find a *, go to next star state to
       see if it is followed by backslash */
    if (x == '*') {
        state = NEXTSTAR;
    }

    /* write newline to keep track of lines and remain in state */
    else if (x == '\n') {
        putchar(x); 
        state = FIRSTSTAR; 
    }

    /* remain in this state and do not write, in all other cases */
    else {
        state = FIRSTSTAR; 
    }
    return state; 
}

/* implement the NEXTSTAR state of the DFA. x is the current DFA 
character. This is for the case that we have read a star after being 
in a comment, so we are in a comment but may be reaching the end
of one and so try to detect that. We never write x unless it is a 
newline to keep track of the new lines. Return the next state. */
enum Statetype handleNextStarState(int x) {
    enum Statetype state;
    /* we read a backslash, we have reached 
       the end of a comment and so return to start state */
    if (x == '/') {
        state = START; 
    }

    /* corner case where another star so remain in state */
    else if (x == '*') {
        state = NEXTSTAR; 
    }

    /* write newline to keep track of lines and remain in state */
    else if (x == '\n') {
        putchar(x);
        state = FIRSTSTAR; 
    }

    /*go back to first star state */
    else {
        state = FIRSTSTAR; 
    }
    return state; 
    
}

/* read text from stdin. Write text to stdout such that it contains no
 comments given only by a forward slash and star. This program serves 
as one of the functions of the preprocessing stages that removes 
comments before conversion into machine language. Return 0 unless a
 comment is unterminated, in that case return EXIT FAILURE */
int main(void) {
    /* c is used to store our characters read, linecount keeps count 
    of lines, line_error keeps count 
    of where the comments begin in case there is an error, 
    and first_starcount is really a binary value 
    that shows when line_error should be adjusted. Start at 
    START state */
    int c; 
    int lineCount=1; 
    int lineError;
    int firstStarcount = 0; 
    enum Statetype state = START; 
    /* read and store into c until we reach end of file */
    while ((c = getchar())!= EOF) {
        /* increment line count when new line encountered */
        if(c == '\n') {
            lineCount++; 
        }
        
        switch(state) {
            /* when in start state, reset first_starcount so that 
               line_error can be readjusted */
            case START:
                state = handleStartState(c); 
                firstStarcount = 0; 
                break; 
            /* state for encountering double quote */
            case DQUOTE:
                state = handleDQuoteState(c);
                break; 
            /* state for encountering single quote */
            case QUOTE:
                state = handleQuoteState(c);
                break;
            /* state for first slash of potential comment */
            case FIRSTSLASH:
                state = handleFirstSlashState(c);
                break; 
            /* state for first star of potential comment */
            case FIRSTSTAR:
            /* if this is the first time that we have read a star, 
            then adjust line_error. Then change first_starcount */
                if (firstStarcount == 0) {
                    lineError = lineCount; 
                    firstStarcount = 1; 
                }
                state = handleFirstStarState(c);
                break; 
            /* state for encountering last star of a comment */
            case NEXTSTAR:
                state = handleNextStarState(c); 
                break; 
            /* state for escape character in single quote */
            case ESCAPE:
                state = handleEscape(c); 
                break; 
            /* state for escape character in double quote quote */
            case DESCAPE:
                state = handleDEscape(c); 
                break; 
        }
    }
    /* have a forward slash at the end of a file and so we write one */
    if (state == FIRSTSLASH) {
        putchar('/');
    }

    /* if we end in the FIRSTSTAR or NEXTSTAR state, we reject input*/
    if (state == FIRSTSTAR || state == NEXTSTAR) {
        fprintf(stderr, "Error: line %d: unterminated comment\n",
                lineError);
        exit(EXIT_FAILURE); 
    }
    
    else {
        return 0; 
    }
 
   
}
