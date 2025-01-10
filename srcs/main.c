#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InputBuffer_t {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
};
//define columns user name with size 32
//defube column email with size 255

//define typedef struct row

typedef struct InputBuffer_t InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandReult;

typedef enum {
    PREPARE_COMMAND_SUCCESS,
    PREPARE_COMMAND_UNRECOGNIZED_STATEMENT
    //Add syntax error message
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    //Row struct that needs to be inserted 
} Statement;


InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void read_input(InputBuffer* input_buffer) {
    ssize_t input_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    input_buffer->input_length = input_read - 1;
    input_buffer->buffer[input_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt() {
    printf("db> ");
}

MetaCommandReult eval_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(1);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "INSERT", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        //Add logic to read three arguments
        //Add logic to check if there are more than tree arguments
        return (PREPARE_COMMAND_SUCCESS);
    }
    else if (strncmp(input_buffer->buffer, "SELECT", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return(PREPARE_COMMAND_SUCCESS);
    }
    return(PREPARE_COMMAND_UNRECOGNIZED_STATEMENT);
}

void execute_statement(Statement* statement) {
    switch(statement->type) {
        case (STATEMENT_INSERT):
            printf("INSERT STATEMENT LOGIC HERE! \n");
            break;
        case (STATEMENT_SELECT):
            printf("SELECT STATEMENT LOGIC HERE! \n");
            break;
    }
}

int main(void) {
    InputBuffer* input_buffer = new_input_buffer();
    input_buffer->buffer_length = 0; 

    while (true) {
        print_prompt();
        read_input(input_buffer);
        
        if(input_buffer->input_length == 0) {
            continue;
        }

        if (input_buffer->buffer[0] == '.') {
            switch(eval_meta_command(input_buffer)) {
                case(META_COMMAND_SUCCESS):
                    continue;
                case(META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized meta command: %s \n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch(prepare_statement(input_buffer, &statement)) {
            case(PREPARE_COMMAND_SUCCESS):
                break;
            case(PREPARE_COMMAND_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized command detected: %s \n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed statement\n");
    }

    return (0);
}