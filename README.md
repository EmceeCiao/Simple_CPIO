![github repo badge: Language](https://img.shields.io/badge/Language-C-181717?color=red) ![github repo badge: Testing](https://img.shields.io/badge/Testing-Criterion-181717?color=orange)

# Simple_CPIO

A command line utility to serialize directories and files to a specific sequence of bytes and deserialize such a sequence to paste from one location to another. Similar to the functionality that's provided by Unix's ```cpio``` utility This was a project created for my System Fundamentals II class, requiring us to focus on I/O, strings, bit manipulation, and pointer manipulation. All data manipulation/reading was done byte by byte. 

## Design  

The serialized data format creates a sequence of records, each consisting of a header followed by some data or just a header, and is outputted to stdout. The header is composed of 16 bytes in total: 
```"Magic" (3 bytes): 0x0C, 0x0D, 0xED Type (1 byte) Depth (4 bytes) Size (8 bytes)``` 

The record types consist of  
```
START_OF_TRANSMISSION (type = 0)
END_OF_TRANSMISSION (type = 1)
START_OF_DIRECTORY (type = 2)
END_OF_DIRECTORY (type = 3)
DIRECTORY_ENTRY (type = 4)
FILE_DATA (type = 5)
```

This allows us to track the start and end of the serialized data as well as its type so that the deserialization of the data creates the correct files. START and END records simply consist of their headers and must be matched like a pair of parenthesis. Directory_Entry records contained metadata of 12 bytes (4 Bytes of permission info and 8 bytes for size) and the name of the file or directory. File_Data records contained the actual file data for the Directory_Entry preceding it. 

Deserialize reads this serialized data from stdin and creates the necessary files and directories.  

## Usage   
Simple_CPIO provides the following commands to run: 
```
Usage: bin/transplant -h [any other number or type of arguments]
    -h       Help: displays this help menu
Usage: bin/transplant -s|-d [-c] [-p DIR]

-s -> serializes data
-d -> deserializes data
-c -> Is an optional parameter that will clobber existing files (overwriting them) otherwise an error will be thrown if the deserialized file already exists
-p DIR -> provides the target directory to either serialize or deserialize into
```
## Building and Testing   

Simple_CPIO can be built using the provided make files and running ```make clean && make all```.  

The rsrc directory was used for testing the program as it was being built, and many similar commands like the following were run: 

```
$ bin/transplant -s -p rsrc/testdir > outfile

$ bin/transplant -d -p test_out < outfile
```

The testing framework used stood [criterion](https://github.com/Snaipe/Criterion), so this must be installed before attempting to run the tests using: 

```bin/transplant_tests```


