#include "shell.h"

/**
 * builtins_list - Search for a match in the list of built-in commands and execute the associated function.
 * @data: Struct for the program's data.
 *
 * This function searches for a match between the given command and the list of built-in commands.
 * If a match is found, it executes the associated function and returns its return value.
 * If no match is found, it returns -1.
 *
 * @return: The return value of the executed function if there is a match, or -1 if there is no match.
 **/
int builtins_list(data_of_program *data)
{
    int iterator;
    builtins options[] = {
        {"exit", builtin_exit},
        {"help", builtin_help},
        {"cd", builtin_cd},
        {"alias", builtin_alias},
        {"env", builtin_env},
        {"setenv", builtin_set_env},
        {"unsetenv", builtin_unset_env},
        {NULL, NULL}
    };

    /* Walk through the structure */
    for (iterator = 0; options[iterator].builtin != NULL; iterator++)
    {
        /* If there is a match between the given command and a built-in command */
        if (str_compare(options[iterator].builtin, data->command_name, 0))
        {
            /* Execute the function and return its return value */
            return (options[iterator].function(data));
        }
    }

    /* If there is no match, return -1 */
    return (-1);
}
