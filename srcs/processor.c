#import "../headers/headers.h"

PrepareResult validate_statement(InputBuffer* input_buffer, Statement* statement) {
    strtok(input_buffer->buffer, " ");

    char* id_value = strtok(NULL, " ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");
    int id = atoi(id_value);
    if (id_value == NULL || email == NULL || username == NULL) {
        return (PREPARE_COMMAND_SYNTAX_ERROR);
    }

    if (id < 0 ) {
        return (PREPARE_COMMAND_NEGATIVE_NUMBER_ERROR);
    }

    if (strlen(username) > USERNAME_LENGTH || strlen(email) > EMAIL_LENGTH) {
        return (PREPARE_COMMAND_STRING_OVERFLOW_ERROR);
    }
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);
    statement->row_to_insert.id = id;

   return PREPARE_COMMAND_SUCCESS;
}


PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "INSERT", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return validate_statement(input_buffer, statement);
    }
    else if (strncmp(input_buffer->buffer, "SELECT", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return(PREPARE_COMMAND_SUCCESS);
    }
    return(PREPARE_COMMAND_UNRECOGNIZED_STATEMENT);
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