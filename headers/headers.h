#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME_LENGTH 32
#define EMAIL_LENGTH 255

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//define constants for table schema; 
    // id size (row id), 
    // usename size (row username), 
    // email size (row email), 
    // id offset,
    //  username  offset (id size + id offset), 
    //  email offset (username size + username offset)
    // row size is all of the aboce sizes combined 

// Define constants for table
//     constant for page size 4096
//     MACRO for max pages 100
//     constant for max rows per page. Page size divided by row size
//     constant for max rows in table. rows per page times max amount of pages




struct InputBuffer_t {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
};

// Typedef struct table 
//     number of rows (unsigned int 32)
//     number of pages (void pointer to a pages array of size table max pages)

typedef struct Row_t {
    int id;
    char username[USERNAME_LENGTH];
    char email[EMAIL_LENGTH];
} Row;

typedef struct InputBuffer_t InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandReult;

typedef enum {
    PREPARE_COMMAND_SUCCESS,
    PREPARE_COMMAND_UNRECOGNIZED_STATEMENT
    PREPARE_COMMAND_SYNTAX_ERROR
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;
