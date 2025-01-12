#include "../headers/headers.h"

//TODO : REFACTOR Solution to their own files

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

void*   row_slot(Table* table, uint32_t row_number) {
    uint32_t page_num = row_number / MAX_ROWS_PER_PAGE;
    void* page = table->pages[page_num];

    if (page == NULL) {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    //TODO : READ More on these :
    uint32_t row_offset = row_number % MAX_ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    return page + byte_offset;
}

void    insert_row_to_table(Table* table, Statement* statement) {
    void* page = row_slot(table, table->number_of_rows);
    serialize(&(statement->row_to_insert), page);
    table->number_of_rows += 1;
}

void print_row(Row* row) {
    printf("(%d %s %s) \n", row->id, row->username, row->email);
}

void select_from_table(Table* table) {
    Row row;

    for (uint32_t i = 0; i < table->number_of_rows; i++) {
        deserialize(row_slot(table, i), &row);
        print_row(&row);
    }
}

Table*  new_table() {
    Table *table = malloc(sizeof(Table));

    for (int i = 0; i <= MAX_PAGES_PER_TABLE; i++) {
        table->pages[i] = NULL;
    }
    table->number_of_rows = 0;
    return table;
}

void free_table(Table* table) {
    for (int i = 0; i <= MAX_PAGES_PER_TABLE; i++) {
        free(table->pages[i]);
    }
    free(table);
}

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

void execute_statement(Statement* statement, Table* table) {
    switch(statement->type) {
        case (STATEMENT_INSERT):
            insert_row_to_table(table, statement);
            break;
        case (STATEMENT_SELECT):
            select_from_table(table);
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
        execute_statement(&statement, table);
        printf("Executed statement\n");
    }

    return (0);
}