#include "shell.h"

/**
 * _getline - Read a line from the prompt.
 * @data: Struct for the program's data.
 *
 * This function reads a line from the prompt and performs necessary checks and splitting.
 * It handles logical operators such as && and ||, as well as line termination characters.
 *
 * @return: The number of bytes read.
 */
int _getline(data_of_program *data)
{
    char buff[BUFFER_SIZE] = {'\0'};
    static char *array_commands[10] = {NULL};
    static char array_operators[10] = {'\0'};
    ssize_t bytes_read, i = 0;

    /* Check if there are no more commands in the array */
    /* and check the logical operators */
    if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
        (array_operators[0] == '|' && errno == 0))
    {
        /* Free the memory allocated in the array if it exists */
        for (i = 0; array_commands[i]; i++)
        {
            free(array_commands[i]);
            array_commands[i] = NULL;
        }

        /* Read from the file descriptor into buff */
        bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
        if (bytes_read == 0)
            return (-1);

        /* Split lines for line termination characters */
        i = 0;
        do {
            array_commands[i] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
            /* Check and split for logical operators (&& and ||) */
            i = check_logic_ops(array_commands, i, array_operators);
        } while (array_commands[i++]);
    }

    /* Obtain the next command (command 0) and remove it from the array */
    data->input_line = array_commands[0];
    for (i = 0; array_commands[i]; i++)
    {
        array_commands[i] = array_commands[i + 1];
        array_operators[i] = array_operators[i + 1];
    }

    return (str_length(data->input_line));
}


/**
 * check_logic_ops - Check and split commands based on logical operators.
 * @array_commands: Array of commands.
 * @i: Index in the array_commands to be checked.
 * @array_operators: Array of logical operators for each previous command.
 *
 * This function checks each command in the array for logical operators (&& and ||).
 * If a logical operator is found, it splits the command and updates the arrays accordingly.
 *
 * @return: The index of the last command in the array_commands.
 */
int check_logic_ops(char *array_commands[], int i, char array_operators[])
{
    char *temp = NULL;
    int j;

    /* Check for logical operators in the command line */
    for (j = 0; array_commands[i] != NULL && array_commands[i][j]; j++)
    {
        if (array_commands[i][j] == '&' && array_commands[i][j + 1] == '&')
        {
            /* Split the line when '&&' is found */
            temp = array_commands[i];
            array_commands[i][j] = '\0';
            array_commands[i] = str_duplicate(array_commands[i]);
            array_commands[i + 1] = str_duplicate(temp + j + 2);
            i++;
            array_operators[i] = '&';
            free(temp);
            j = 0;
        }
        if (array_commands[i][j] == '|' && array_commands[i][j + 1] == '|')
        {
            /* Split the line when '||' is found */
            temp = array_commands[i];
            array_commands[i][j] = '\0';
            array_commands[i] = str_duplicate(array_commands[i]);
            array_commands[i + 1] = str_duplicate(temp + j + 2);
            i++;
            array_operators[i] = '|';
            free(temp);
            j = 0;
        }
    }
    return (i);
}
