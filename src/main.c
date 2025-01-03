#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "debug.h"
#include "funcs.h"
#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{    
    int ret;
    if(validargs(argc, argv)){
        USAGE(*argv, EXIT_FAILURE); 
    }
    if(global_options & 0x1){
        USAGE(*argv, EXIT_SUCCESS); 
    }  

    if(global_options & 0x02){ 
        if(serialize() == -1){ 
            fprintf(stderr, "Serialize Failed!");  
            return EXIT_FAILURE; 
        }else{ 
            return EXIT_SUCCESS; 
        }
    } 
    if(global_options & 0x04){ 
        if(deserialize() == -1){ 
            fprintf(stderr, "Deserialize Failed");  
            return EXIT_FAILURE; 
        }else{ 
            return EXIT_SUCCESS; 
        }
    }   

    return EXIT_SUCCESS;      
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
