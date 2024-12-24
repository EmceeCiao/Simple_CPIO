//File Header used to define the functions I will be creating for this assignment 

//Functions to be used in validargs and elsewhere: 
int str_Eql(char *str1, char *str2); 
int str_size(char *str); 
int str_copy(char *copying, char *pasting);  
//Serialize Functions 
void start_transmission(); //Needs implementing -> Done
void end_transmission();  //Needs implementing -> Done
void print_magic(); //Needs implementing -> Done 
void print_depth(int depth); //Needs implementing  
void print_size(long size); //Needs implementing  
// void print_st_size(off_t size); 
// void print_st_mode(mode_t mode);  
void print_st_fields(mode_t mode, off_t size);  
void print_name(char* name);   

//Deserialize Functions for verifying data 
int verify_magic();   
int verify_depth(int given_depth); 
//Deserialize Functions for storing information that will be needed! 
off_t verify_size();  
mode_t verify_mode();  
int name_buf_init(int characters);  
int name_buf_append(int characters); 