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
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 *
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in global.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */ 

/* Personal Implementation of some record writing to make serilization easier! 
So Far will be implementing: 
    1) start_tranmssion(); -> is always same
    2) end_transmission(); -> is always same
    3) print_magic(); -> is always same  
    4) print_depth(): -> need to do some bit shifting for this! 
    )5 print_size() -> (How do we get size, well 16 from record + whatever from name + 12 from metadata?) 
        -> if it's anything other than this, it is either 16 + metadata size (Size of File?) 
        -> we get length of name using str_size in our case! 
*/ 
void start_transmission(){  
    //We print the magic bytes first 
    fputc(0x0c, stdout); 
    fputc(0x0d, stdout); 
    fputc(0xed, stdout);    
    //The below have been stuffed into the for loop with size printing! 
    // //next let's print the type 
    // fputc(0x00, stdout);  
    // //Now the depth which is 00 00 00 00
    // fputc(0x00, stdout); 
    // fputc(0x00, stdout); 
    // fputc(0x00, stdout); 
    // fputc(0x00, stdout); 
    //Now we just need to print the size  
    for (int i = 0; i < 12; i++){ 
        fputc(0x00, stdout); 
    } 
    fputc(0x10, stdout); 
}  
void end_transmission(){
    //Print magic bytes!  
    fputc(0x0c, stdout); 
    fputc(0x0d, stdout); 
    fputc(0xed, stdout);    
    fputc(0x01, stdout); 
    for(int i = 0; i < 11; i++){  
        fputc(0x00, stdout); 
    } 
    fputc(0x10, stdout); 
}
void print_magic(){  
    //Magic bytes are 0x0c, 0x0d, 0xed
    fputc(0x0c, stdout); 
    fputc(0x0d, stdout); 
    fputc(0xed, stdout);
}  

void print_depth(int depth){ 
    //In order to print depth properly we need to put it into big-endian format 
    //Since int depth is 4 bytes so our first shift is gonna be by 24, then 16, then 8 and then none  
    int auto_shift = 24; 
    for(int i = 0; i < 4; i++){ 
        fputc((depth >> auto_shift & 0xFF), stdout); 
        auto_shift -= 8; 
    } 
} 
void print_size(long size){  
    //Similar to print_depth but we bit shift by 56 this time! 
    int auto_shift = 56; 
    for(int i = 0; i < 8; i++){ 
        fputc((size >> auto_shift & 0xFF),stdout); 
        auto_shift -= 8; 
    }
}  
// //Below two were made for both understanding and in case the value mattered
// void print_st_size(off_t size){ 
//     int auto_shift = 56; 
//     for(int i = 0; i < 8; i++){ 
//         fputc((size >> auto_shift & 0xFF),stdout); 
//         auto_shift -= 8; 
//     }
// } 

// void print_st_mode(mode_t mode){  
//     int auto_shift = 24; 
//     for(int i = 0; i < 4; i++){ 
//         fputc((mode >> auto_shift & 0xFF), stdout); 
//         auto_shift -= 8; 
// }  
// } 
void print_st_fields(mode_t mode, off_t size){ 
    int auto_shift = 24; 
    for(int i = 0; i < 4; i++){ 
        fputc((mode >> auto_shift & 0xFF), stdout); 
        auto_shift -= 8;  
    }  
    auto_shift = 56;  
    for(int i = 0; i < 8; i++){ 
        fputc((size >> auto_shift & 0xFF),stdout); 
        auto_shift -= 8; 
    }
} 
void print_name(char *name){  
    int i = 0; 
    char curr = *(name + i); 
    while (curr != '\0'){ 
        i++;  
        fputc(curr, stdout); 
        curr = *(name + i); 
    } 
}  
int verify_magic(){  
    //Verifying the three magic bytes by grabbing the characters
    //int temp = fgetc(stdin);  
    //printf("%c", temp); 
    if(fgetc(stdin) != 0x0c){return -1;} 
    if((fgetc(stdin))!= 0x0d){return -1;} 
    if((fgetc(stdin))!= 0xed){return -1;}  
    if(ferror(stdin)){return -1;}
    return 0; 
}  
int verify_depth(int actual_depth){ 
    //Need to do the reverse of print_depth, we are given a depth, we get it and then we verify!   
    int curr_depth = 0; 
    int auto_shift = 24;  
    for(int i = 0; i < 4; i++){ 
        int temp = fgetc(stdin);  
        if(temp == EOF){ 
            return -1; 
        }
        curr_depth |= ((temp << auto_shift));  
        auto_shift-=8;  
    }   
    if(ferror(stdin)){return -1;}
    if(curr_depth == actual_depth){ 
        return 0; 
    }else{
        return -1; 
    } 
}
long verify_size(){ 
    off_t size = 0; 
    int auto_shift = 56; 
    for(int i = 0; i < 8; i++){ 
        size |= (fgetc(stdin) << auto_shift); 
        auto_shift -= 8; 
    } 
    if(ferror(stdin)){return -1;} 
    return size; 
} 
mode_t verify_mode(){ 
    mode_t mode = 0; 
    int auto_shift = 24; 
    for(int i = 0; i < 4; i++){  
        mode |= (fgetc(stdin) << auto_shift); 
        auto_shift -= 8;  
    }   
    if(ferror(stdin)){return -1;} 
    return mode; 
}   
int name_buf_init(int characters){ 
    if (characters > NAME_MAX){ 
        return -1; 
    }    
    int counter = 0;   
    while(counter < characters){   
        int c = 0;   
        c = fgetc(stdin); 
        //printf("%d ", (c =fgetc(stdin))); 
        *(name_buf + counter) = c; 
        counter++;  
    }  
    // printf("\n");  
    //*(name_buf + counter) = '\0'; 
    if(ferror(stdin)){return -1;} 
    return 0; 
} 
int name_buf_append(int characters){  
    if (characters + path_length + 2 > PATH_MAX){ 
        return -1; 
    }else{ 
        *(path_buf + path_length) = '/';  
        path_length += 1; 
        char *temp = path_buf + path_length; 
        if(str_copy(name_buf, temp)){ 
            return -1; 
        }else{ 
            path_length += characters; 
            return 0; 
        }
    }
} 
void name_buf_clear(){ 
    for (int i = 0; i < NAME_MAX; i++){ 
            *(name_buf + i) = '\0'; 
    } 
}
    

/*
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {  
    verify_magic(); 
    int type = fgetc(stdin);
    if (type != 0x02){return -1;}
    if(verify_depth(depth) == -1){return -1;} 
    long size = verify_size();   
    int control = 1;  
    while(control){
        verify_magic(); 
        type = fgetc(stdin);
        if (type != 0x03 && type != 0x04) return -1;
        verify_depth(depth); 

        size = 0; 
        size = verify_size(); 

        if (type == 0x03){ 
            break;
        }
        mode_t mode = 0;
        long meta_data_size = 0;

        mode = verify_mode(); 
        meta_data_size = verify_size();  

        name_buf_clear();
        name_buf_init(size - 28);   
        // printf("%s\n",name_buf);  
        struct stat stat_buf;  
        if (S_ISDIR(mode)) { 
            if (path_push(name_buf) == -1) return -1; 
            if (stat(path_buf, &stat_buf) == 0){ 
                if(deserialize_directory(depth + 1) == -1) return -1;  
                if(path_pop()); 
                if(stat_buf.st_size != meta_data_size){ 
                    fprintf(stderr, "Metadata Size In Header didn't Match Actual"); 
                    return -1;
                    }
            }else{  
            if (mkdir(path_buf, 0700) == -1) { 
                fprintf(stderr, "Making Directory Failed"); 
                return -1;}            
            if (deserialize_directory(depth + 1) == -1) { 
                fprintf(stderr, "Something must have gone wrong with deserialize_directory"); 
                return -1;}
            if (chmod(path_buf, mode & 0777) == -1) { 
                fprintf(stderr, "Something went wrong with changing permissions back on a directory"); 
                return -1;}
            if (path_pop() == -1) { 
                fprintf(stderr, "Something went wrong with popping"); 
                return -1;}
}
            // if (mkdir(path_buf, 0700) == -1) return -1;            
            // if (deserialize_directory(depth + 1) == -1) return -1;
            // if (chmod(path_buf, mode & 0777) == -1) return -1; 
            // if (path_pop() == -1) return -1;
        } else if (S_ISREG(mode)) {
            if (path_push(name_buf) == -1) { 
                return -1;} 
            //printf("%s\n 1:\n", path_buf); 
            if (deserialize_file(depth) == -1) return -1;  
            path_push(name_buf);  
            //printf("%s\n 2:\n", path_buf);  
            if(chmod(path_buf, mode & 0777) == -1){return -1;}
            path_pop();
            // if (chmod(path_buf, mode & 0777)) { 
            //     fprintf(stderr, "Something went wrong with changing permissions back on a directory"); 
            //     return -1;} 

        } else {
            return -1; 
        }
    }
    return 0;

//     verify_magic(); 
//     int type = fgetc(stdin);
//     if (type != 0x02){return -1;}
//     if(verify_depth(depth) == -1){return -1;} 
//     long size = verify_size();   
//     int control = 1;  
//     while(control){
//         verify_magic(); 
//         type = fgetc(stdin);
//         if (type != 0x03 && type != 0x04) return -1;
//         verify_depth(depth); 

//         size = 0; 
//         size = verify_size(); 

//         if (type == 0x03){ 
//             break;
//         }
//         mode_t mode = 0;
//         long meta_data_size = 0;

//         mode = verify_mode(); 
//         meta_data_size = verify_size();  
//         if(meta_data_size == -100000){ 
//             return -1;
//         } 
//         name_buf_clear();
//         name_buf_init(size - 28);   
//         // printf("%s\n",name_buf);  
//         struct stat stat_buf;  
//         if (S_ISDIR(mode)) { 
//             if (path_push(name_buf) == -1) return -1; 
//             if (stat(path_buf, &stat_buf) == 0){ 
//                 if(deserialize_directory(depth + 1) == -1) return -1;  
//                 if(path_pop()); 
//             }else{  
//             if (mkdir(path_buf, 0700) == -1) { 
//                 fprintf(stderr, "Making Directory Failed"); 
//                 return -1;}            
//             if (deserialize_directory(depth + 1) == -1) { 
//                 fprintf(stderr, "Something must have gone wrong with deserialize_directory"); 
//                 return -1;}
//             if (chmod(path_buf, mode & 0777) == -1) { 
//                 fprintf(stderr, "Something went wrong with changing permissions back on a directory"); 
//                 return -1;}
//             if (path_pop() == -1) { 
//                 fprintf(stderr, "Something went wrong with popping"); 
//                 return -1;}
// }
//             // if (mkdir(path_buf, 0700) == -1) return -1;            
//             // if (deserialize_directory(depth + 1) == -1) return -1;
//             // if (chmod(path_buf, mode & 0777) == -1) return -1; 
//             // if (path_pop() == -1) return -1;
//         } else if (S_ISREG(mode)) {
//             if (path_push(name_buf) == -1) { 
//                 return -1;}
//             if (deserialize_file(depth) == -1) (return -1;) 
//         } else {
//             return -1; 
//         }
//     }
//     return 0;
}

/*
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth) {
    // To be implemented. 
    FILE *fp;  
    if(((fp = fopen(path_buf, "r")) == NULL)){ 
        //we continue like normal, so we close open it again with writing permission   
        if((fp = fopen(path_buf, "w")) == NULL){ 
            fprintf(stderr, "Error in Deserialize_File when Opening File"); 
            return -1; 
        }
    }else{  
        if(global_options == 0xc){ 
            fclose(fp); 
            if((fp = fopen(path_buf, "w")) == NULL){  
            fprintf(stderr, "Error when Opening Path_Buf in Deserialize File"); 
            return -1; 
        } 
    }else{
     //File is existing and we need to throw error so 
        fclose(fp); 
        return -1; 
    }} 
    //We have the file open now for copying once we get the chance! 
    //First Read Magic Bytes and the type to make sure it's a file data record
    if(verify_magic() == -1){
        //printf("HERE");  
        return -1;} 
    if(fgetc(stdin) != 0x05){return -1;}   
    if(verify_depth(depth) == -1){return -1;} 
    off_t size = verify_size();  
    //Checking later on size and bytes printed! 
    off_t counter = 16; 
    int c;    
    while(counter < size){  
        if((c = fgetc(stdin)) == EOF){ 
            return -1; 
        }else{  
            fputc(c, fp);  
            counter += 1; 
        }
    }  
    fflush(fp); 
    fclose(fp); 
    //This should be the end of file_data, We didn't have to check metadata this time as we weren't doing Directory_entry  
    if(path_pop() == -1){return -1;}
    if(ferror(stdin)){ 
        fprintf(stderr, "There was some error recongized by ferror. See ERRNO."); 
        return -1; 
    }else{return 0;}  
     //abort();
}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_directory(int depth) {
    // To be implemented.    
    //Setting up pointers and structs that will be needed
    DIR *dir; 
    struct stat stat_buf; 
    
    //We need to check that it's a directory! If we fail to open we can immediately return -1; 
    dir = opendir(path_buf); 
    if(dir == NULL){  
        //perror("Failure to open DIR in Serialize Directory: ");
        return -1; 
    }
    //After knowing it's a directory then we can do each subsequent entry with the dirent struct!

    //First need to print the start of a directory since it is serialize directory   
    print_magic(); 
    fputc(0x02, stdout); 
    print_depth(depth);  
    print_size(16);     

    //Ok we have the directory open, we now need to create the struct to traverse!  
        //As we traverse we readdir and do case by case basis, we will use the S.ISREG or S.ISDIR macros! 
        //As we go to new structs, before we call their respective function we do have to print the directory entry info 
        //This requires us to get the metadata and the name..., name we can copy and paste/iterate through and we know it based on size.. 
            //since names are not null terminated we must be precise with this! 
        //Should double check that name doesn't overdo the push... 
    struct dirent *de; 
    while((de = readdir(dir)) != NULL){  
        //We need to check the name and potentially store the length of the name 
        //For metadata printing we need print_st_size and print_st_mode  
        //First let's check if it's .. or ., if it is we can skip 
        //Second we want to initalize to stat_buf struct 
        //Third we can see if it's a directory or file 
        //Fourth we print the record using all the info and push the name on (checking if pushing name gives error) 
        //Call the respective functions! based on what it was  
        if (str_Eql(de->d_name, "..")){ 
            continue;
        } 
        if(str_Eql(de->d_name, ".")){ 
            continue; 
        } 
        //Now we initalize stat_buf but we have to push first 
        if(path_push(de->d_name) == -1){
            //perror("Name of File or Directory is too long: "); 
            return -1; 
        }
        if(stat(path_buf, &stat_buf) == -1){ 
            //perror("Stat on path_buf failed!: "); 
            return -1; 
        };  
        //Let's get the length of the path while we are at it! 
        int num_name_bytes = str_size(de->d_name); 
        //Realistically we are just printing directory entry so we can print the first half before we even check what it is 

        print_magic(); 
        fputc(0x04, stdout); 
        print_depth(depth);   
        print_size(num_name_bytes + 12 + 16);  
        print_st_fields(stat_buf.st_mode, stat_buf.st_size);  
        print_name(de->d_name); 

        if(S_ISDIR(stat_buf.st_mode)){ 
            //We do serialize directory 
            if(serialize_directory(depth + 1)){  
                return -1; 
            } 
        }else if(S_ISREG(stat_buf.st_mode)){ 
            //We do serialize File  
            if(serialize_file(depth, stat_buf.st_size)){ 
                return -1; 
            } 
        }else{ 
            //perror("It is not a File or a Directory according to stat_buf: "); 
            return -1; 
        } 
        if (ferror(stdout) != 0){ 
            return -1; 
        }
    }
    //End off with end of directory once the function stops running   
    print_magic(); 
    fputc(0x03, stdout); 
    print_depth(depth); 
    print_size(16); 
    //Do one last final check of ferror and also close directory!  
    closedir(dir);  
    if(ferror(stdout) != 0){ 
        return -1; 
    }else{  
        if(path_pop() == -1){return -1;}
        return 0; 
    }
    //abort();
}

/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size) {
    // To be implemented.    
    //Serialize File is going to be printing 
    //First need to print the magic header 
    print_magic();  
    //After printing the magic header I need to print the FILE_DATA RECORD 
    fputc(0x05,stdout); 
    print_depth(depth); 
    print_size((size + (long)(16)));    
    
    //Open File, read it and then print it out!     
    //Need to check error for opening File!   
    FILE *fp; 
    if((fp = fopen(path_buf, "r")) == NULL){  
        //perror("Serialize File Failed to Open: ");
        return -1; 
    }else{  
        int c; 
        while((c = fgetc(fp)) != EOF){ 
            fputc(c, stdout);
        } 
    } 
    fclose(fp);  
    fflush(stdout); 
    //Error Checking for fputc 
    if (ferror(stdout) != 0){
        return -1; 
    }else{  
        if(path_pop() == -1){return -1;}
        else{ 
            return 0; 
        }
    } 
    //abort();
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int serialize() {
    // To be implemented.  

    //Start the transmission first
    start_transmission();   
    
    //Now need to iterate through properly and call serialize_file or serialize_directory as I go!   
    //Some statbuf struct is needed somewhere 

    //NEEDS TO BE IMPLEMENTED   
        //1) Check if it is a file or directory 
        //2) Print whatever info is needed and path_push or path_pop as needed 
        //3) return back here and keep checking 
    //End the transmission now that we are done   
    if(serialize_directory(1) == -1){ 
        return -1; 
    }
    end_transmission();  
    
    //Make sure to flush everything out 
    fflush(stdout);  

    //Check to see if perror or error has occured when writing   
    if (ferror(stdout) != 0){
        return -1; 
    }else{  
        return 0; 
    } 
    //abort();
}

/** 
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    // To be implemented. 
    //Need to check start tranmssion  
    struct stat stat_buf;  
    FILE *fp; 
    if(verify_magic() == -1){return -1;} 
    if(fgetc(stdin) != 0x0){return -1;}  
    if(verify_depth(0) == -1){return -1;} 
    long size = verify_size();   
    if(size != 16){ 
        return -1; 
    }  
    //Once we verified the transmissions is now time to see whether the user gave an existing directiory 
    //or if we have to open it ourselves 
    if(stat(path_buf, &stat_buf) == 0){ 
        //We know the file exists so we don't have to open it and we can call deserialize 
        if(deserialize_directory(1) == -1){return -1;}
    }else{
    //Need to call deserialize after making so we do 
    if(mkdir(path_buf, 0700) == -1){ 
        fprintf(stderr, "Problem making directory for initalized path_buf in deserialize"); 
        return -1;} 
    if(deserialize_directory(1) == -1){return -1;}   
    }  

    //Need to call end transmission 
    if(verify_magic() == -1){return -1;} 
    if(fgetc(stdin) != 0x01){return -1;} 
    if(verify_depth(0) == -1){return -1;} 
    size = verify_size(); 
    if(size != 16){ 
        return -1; 
    } 
    return 0; 
    //abort();
}
