#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME_LENGTH 32
#define EMAIL_LENGTH 255
#define MAX_PAGES_PER_TABLE 100

typedef struct {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
    void* prev;
} InputBuffer;

typedef struct {
    int id;
    char username[USERNAME_LENGTH + 1]; 
    char email[EMAIL_LENGTH + 1];
} Row;


typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandReult;

typedef enum {
    PREPARE_COMMAND_SUCCESS,
    PREPARE_COMMAND_UNRECOGNIZED_STATEMENT,
    PREPARE_COMMAND_STRING_OVERFLOW_ERROR,
    PREPARE_COMMAND_NEGATIVE_NUMBER_ERROR,
    PREPARE_COMMAND_SYNTAX_ERROR
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef struct {
    uint32_t number_of_rows;
    void*   pages[MAX_PAGES_PER_TABLE];
} Table;


//Function to get the size of an attribute in a struct without mallocing the struct
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)


//  UTIL FUNCTIONS

void close_input_buffer(InputBuffer* input_buffer);

void print_prompt();

void free_table(Table* table);


// PROCESSOR FUNCTIONS

PrepareResult validate_statement(InputBuffer* input_buffer, Statement* statement);

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

MetaCommandReult eval_meta_command(InputBuffer* input_buffer, Table* table);