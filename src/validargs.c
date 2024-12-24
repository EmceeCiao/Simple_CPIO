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

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    // To be implemented. 
    // Defining Necessary Flags for BitMap 
    int h_flag = 0; 
    int s_flag = 0;
    int d_flag = 0; 
    int c_flag = 0;   
    int p_flag = 0; 
     

    // Removing Trivial Case of no args passed in!  
    if (argc == 1){ 
        return -1; 
    } 
    // Now we need to check the first argument passed in, to do this pointer arithmetic is needed on **argv to get it to the first argument
    argv += 1; //argv now points to the first argument and not the program name  
    
    //First argument breaks into 3 cases, it can either be -h, -s, or -d. 
    //Let's handle the case of -h, if it's passed in first instant sucess 
    if(str_Eql(*argv, "-h")){ 
        global_options = 0x1; 
        return 0; 
    } 
    //Case of "-s"
    else if(str_Eql(*argv, "-s")){ 
        s_flag = 0x2; //Setting s_flag and not global options as we want the bitmap incase further arguments are wrong! 
    }  
    //Case of "-d"
    else if(str_Eql(*argv, "-d")){  
        d_flag = 0x4;
    }
    //Case of No Proper First Argument 
    else{ 
        return -1;
    }

    //Now let's handle the rest of the inputs and update our bitmap as we go!  
    //Note: We'll use the bitmap and argc at the end in order to figure out conflicts, cleaner to just iterate and see what happens 
    int counter = 1; 
    
    //The conditional is argc - 1 as we're not counting the "0th arg" of the program name
    while(counter < argc -1){  
        argv++;  
        //Split into the cases here, so it's either -p or -c flag   
        if(str_Eql(*argv, "-p")){   
            //Catching case of repeated p_flags being passed in
            if(p_flag){ 
                return -1;
            }
            p_flag = 1;    
            argv++;  
            counter++; 
            if(counter < argc -1){ 
                //This is to handle it being present whether it be NULL or whatever   
                int check = path_init(*argv);  
                if(check == -1){ 
                    return -1; 
                }
                counter++; 
            }else{ 
                //This is to handle it not being present with the -p flag which should throw error 
                return -1; 
            } 

            /* THE CODE BELOW IS DEPRECATED and from when we didn't understand path_init being called*/
            //If the DIR following -p is NULL --> we don't have to care, this interaction is handled in path_init
            //We don't have a /p and nothing argument which is crazy...
            // if(*argv == NULL){ 
            //     return -1; 
            // }else{ 
            //     //We store what's in argv! But we need a strcpy like function for this LOL 
            //     //str_copy(*argv, name_buf);  
            //     counter += 2; 
            // }
        }else if(str_Eql(*argv, "-c")){  
            //What if there's multiple c_flags passed in (handles repeated flags case)
            if(c_flag){
                return -1; 
            }
            counter += 1;  
            c_flag = 0x8; 
        }else{ 
            return -1; 
        }
    }   
    //If p_flag was not provided we should call path_init with NULL as I defined the behavior for this as "." 
    if(p_flag == 0){ 
        path_init(NULL); 
    }
    //s_flag && c_flag is the only error that hasn't been caught, validate args was fine 
    if((s_flag && c_flag)){  
        return -1; 
    }else{ 
        global_options = s_flag | d_flag | c_flag | h_flag; 
        return 0; 
    }
    //abort();
}  

/*  
Helper String functions made for valid args and the rest of the assignment
Functions being made: 
    1) str_Eql to act as strcmp -> Will compare character by character, returning 1 if same, 0 if not 
    2) str_size to act as strlen -> Will get length of string without null terminator by doing pointer arithmetic till null
*/
int str_Eql(char *str1, char *str2){ 
    int pointer = 0;   
    //Catch case of Null Pointer so it doesn't segfault 
    if(str1 == NULL || str2 == NULL){ 
        return 0; 
    }
    while(*(str1 + pointer) == *(str2 + pointer)){ 
        if(*(str1 + pointer) == '\0' || *(str2 + pointer) == '\0'){ 
            break;
        }else{ 
            pointer++;
        }
    }  
    //Last Character should be the same! 
    if (*(str1 + pointer) == *(str2 + pointer)){ 
        return 1; 
    }else{ 
        return 0; 
    }

} 
int str_size(char *str){  
    //Count character by character until we reach a terminating character, so size is minus 1 of what it actually is with null terminatory
    if (str == NULL){ 
        return 0; 
    }
    int i = 0; 
    char curr = *(str + i); 
    while (curr != '\0'){ 
        i++; 
        curr = *(str + i); 
    } 
    return i; 
}  

int str_copy(char *copying, char *pasting){ 
    if (copying != NULL){  
        int i = 0; 
        char curr = *(copying + i); 
        while (curr != '\0'){ 
            *(pasting + i) = curr;  
            i++;  
            curr = *(copying + i); 
        }  
        //Make sure last character is NUll otherwise this would go on forever
        *(pasting + i + 1)  = '\0';  
        return 0; 
    }else{ 
        return -1;  
    } 
} 
