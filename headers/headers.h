#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME_LENGTH 32
#define EMAIL_LENGTH 255

//Function to get the size of an attribute in a struct without mallocing the struct
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//define constants for table schema; 
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_SIZE + ID_OFFSET;
const uint32_t EMAIL_OFFSET = USERNAME_SIZE + USERNAME_OFFSET;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;



// Define constants for table
const uint32_t PAGE_SIZE = 4096;
#define MAX_PAGES_PER_TABLE 100
const uint32_t MAX_ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t MAX_ROWS_IN_TABLE = MAX_ROWS_PER_PAGE * MAX_PAGES_PER_TABLE



struct InputBuffer_t {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
};

typedef struct {
    uint32_t number_of_rows;
    void*   pages[MAX_PAGES_PER_TABLE];
} Table;

typedef struct {
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
