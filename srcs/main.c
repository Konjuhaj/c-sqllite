#include "../headers/headers.h"

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

void serialize(Row* src, void* dst) {
    memcpy(dst + ID_OFFSET, &(src->id), ID_SIZE);
    memcpy(dst + USERNAME_OFFSET, &(src->username), USERNAME_SIZE);
    memcpy(dst + EMAIL_OFFSET, &(src->email), EMAIL_SIZE);
}

void deserialize(void* src, Row* dst) {
    memcpy(&(dst->id), src + ID_OFFSET, ID_SIZE);
    memcpy(&(dst->username), src + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(dst->email), src + EMAIL_OFFSET, EMAIL_SIZE);
}

//TODO: Add function to determine row slot where to insert the data
//  Takes a table and row number 
//  returns the page with the offset on where to a insert data. Might require a malloc


//TODO: Add function to insert data into the table
//  use row_to_insert
//  call previous func to determine where to insert the data to. Use the serialize funcs to copy to data

//TODO: Add function to read data from the table.
//  Loops through the table and prints all the rows

//TODO: Add function to allocate new table

Table*  new_table() {
    Table *table = malloc(sizeof(Table));

    for (int i = 0; i <= MAX_PAGES_PER_TABLE; i++) {
        table->pages[i] = NULL;
    }
    table->number_of_rows = 0;
    return table;
}

//TODO: Add function to free table
void free_table(Table* table) {
    for (int i = 0; i <= MAX_PAGES_PER_TABLE; i++) {
        free(table->pages[i]);
    }
    free(table);
}

//Refactor solution and put functions to their own files

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "INSERT", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_read_in = sscanf(input_buffer->buffer, "INSERT %d %s %s",
             &(statement->row_to_insert.id), statement->row_to_insert.username, statement->row_to_insert.email);
        if (args_read_in != 3) {
            return (PREPARE_COMMAND_SYNTAX_ERROR);
        }
        return (PREPARE_COMMAND_SUCCESS);
    }
    else if (strncmp(input_buffer->buffer, "SELECT", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return(PREPARE_COMMAND_SUCCESS);
    }
    return(PREPARE_COMMAND_UNRECOGNIZED_STATEMENT);
}

void execute_statement(Statement* statement) { // Should also take tabl
    switch(statement->type) {
        case (STATEMENT_INSERT):
            printf("INSERT STATEMENT LOGIC HERE! \n"); //Replace with return and insert function call
            break;
        case (STATEMENT_SELECT):
            printf("SELECT STATEMENT LOGIC HERE! \n"); //Replace with return and select function call
            break;
    }
}


MetaCommandReult eval_meta_command(InputBuffer* input_buffer, Table* table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_table(table);
        exit(1);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

int main(void) {
    InputBuffer* input_buffer = new_input_buffer();
    input_buffer->buffer_length = 0; 
    Table* table = new_table();

    while (true) {
        print_prompt();
        read_input(input_buffer);
        
        if(input_buffer->input_length == 0) {
            continue;
        }

        if (input_buffer->buffer[0] == '.') {
            switch(eval_meta_command(input_buffer, table)) {
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
            case(PREPARE_COMMAND_SYNTAX_ERROR):
                printf("Syntax error: %s \n", input_buffer->buffer);
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