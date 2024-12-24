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

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable 
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) { 
    // To be implemented.  
    //If name is NULL as in there was no proper DIR Passed and we are calling path_init, then just incase, we will assume it's "." 
    if(name == NULL){ 
        str_copy(".", path_buf); 
        path_length = 1; 
        return 0; 
    }
    //Since str_size returns size without null terminating byte we will minus 1 from PathMax as we need the space for that byte  
    int name_size = str_size(name); 
    if(name_size > (PATH_MAX-1)){ 
        //Too big return error 
        return -1; 
    }else{ 
        path_length = name_size;   
        if(str_copy(name, path_buf)){  
            //Copying Failed 
            return -1; 
        }   
        //Handling edgecase of last character  a slash according to Piazza Post 65  
        if(*(path_buf + path_length - 1) == '/'){  
            //If we are replacing the last character we have to change the length 
            *(path_buf + path_length - 1) = '\0'; 
            path_length = path_length - 1; 
        }
        //Copying was successful please return success
        return 0; 
    }
    //abort(); 
}

/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 * 
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {
    // To be implemented.  
    int name_size = str_size(name);  
    //the path_name will be larger than what it can be so return error   
    //this is the path_length + 1 for the null terminator and then we have the name_size + 1 for the '/' 
    if(path_length + name_size + 1 + 1 > PATH_MAX){ 
        return -1; 
    }else{ 
        //We need to get to the first null character and start editing that so  
        *(path_buf + path_length) = '/';  
        path_length += 1; 
        char *temp = path_buf + path_length; 
        if(str_copy(name, temp)){ 
            return -1; 
        }else{ 
            path_length += name_size; 
            return 0; 
        }
    }
    //abort();
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {
    // To be implemented.   
    if(path_length == 0){ 
        return -1; 
    }
    //We have to first find where the / is to elimnate the rest of it by replacing with null characters   
    int temp = path_length - 1;   
    int difference = 0; 
    while((*(path_buf + temp) != '/') && temp > 0){  
        *(path_buf + temp) = '\0';  
        temp -= 1;  
        difference += 1; 
    }   
    //Got to make sure to take out the seperator 
    *(path_buf + temp) = '\0'; 
    //The path_length is gonna be subtracted from the difference and -1;   
    path_length = path_length - difference - 1;   
    return 0;  
    //abort();
}
