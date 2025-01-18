#include "../headers/headers.h"

//TODO : REFACTOR Solution to their own files

//Type defs for ANSI characters
const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
const char UP_ARROW_CHARATER = '\33';

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
const uint32_t MAX_ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t MAX_ROWS_IN_TABLE = MAX_ROWS_PER_PAGE * MAX_PAGES_PER_TABLE;


InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

InputBuffer* read_input(InputBuffer* input_buffer) {
    if (input_buffer->buffer != NULL) {
        InputBuffer* new_buffer = new_input_buffer();
        new_buffer->prev = input_buffer;
        input_buffer = new_buffer;
    }

    ssize_t input_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    input_buffer->input_length = input_read - 1;
    input_buffer->buffer[input_read - 1] = 0;

    return input_buffer;
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

ExecuteResult    insert_row_to_table(Table* table, Statement* statement) {
    if (table->number_of_rows > MAX_ROWS_IN_TABLE) {
        return EXECUTE_TABLE_FULL;
    }

    void* page = row_slot(table, table->number_of_rows);
    serialize(&(statement->row_to_insert), page);
    table->number_of_rows += 1;

    return EXECUTE_SUCCESS;
}

void print_row(Row* row) {
    printf("(%d %s %s) \n", row->id, row->username, row->email);
}

ExecuteResult select_from_table(Table* table) {
    Row row;

    for (uint32_t i = 0; i < table->number_of_rows; i++) {
        deserialize(row_slot(table, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
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

ExecuteResult execute_statement(Statement* statement, Table* table) {
    switch(statement->type) {
        case (STATEMENT_INSERT):
            return insert_row_to_table(table, statement);
            break;
        case (STATEMENT_SELECT):
            return select_from_table(table);
            break;
    }
}



InputBuffer* get_previous_input(InputBuffer* input_buffer) {
    InputBuffer* previous_buffer;

    while (true) {
        print_prompt();
        if (input_buffer->prev != NULL) {
            previous_buffer = input_buffer->prev;
            close_input_buffer(input_buffer);
            printf("%s", previous_buffer->buffer);
            return previous_buffer;
        }
        return input_buffer;

        // if(input_buffer->buffer[0] == '\33' ) { // TODO: Recursive back tracking 
        //     get_previous_input(input_buffer);

        //     continue;
        // }

    }
}

int main(void) {
    InputBuffer* input_buffer = new_input_buffer();
    input_buffer->buffer_length = 0; 
    Table* table = new_table();

    while (true) {
        print_prompt();
        input_buffer = read_input(input_buffer);
        
        if(input_buffer->input_length == 0) {
            continue;
        }

        if(input_buffer->buffer[0] == UP_ARROW_CHARATER ) {
            input_buffer = get_previous_input(input_buffer);
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
                continue;
            case(PREPARE_COMMAND_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized command detected: %s \n", input_buffer->buffer);
                continue;
            case(PREPARE_COMMAND_NEGATIVE_NUMBER_ERROR):
                printf("Invalid id value. Cannot be negative number: %s \n", input_buffer->buffer);
                continue;
            case(PREPARE_COMMAND_STRING_OVERFLOW_ERROR):
                printf("Input values exceed column length \n");
                continue;
        }
        switch(execute_statement(&statement, table)) {
            case(EXECUTE_SUCCESS):
                printf("Executed statement\n");
                continue;
            case(EXECUTE_TABLE_FULL):
                printf("Execute failure: Table full \n");
                continue;
        };
    }

    return (0);
}