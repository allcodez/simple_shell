#include "shell.h"

/**
 * builtin_env - Display the current environment variables.
 * @data: Struct for the program's data.
 *
 * This function displays the environment variables where the shell is running.
 * If no arguments are provided, it prints all environment variables.
 * If an argument in the form "name=value" is provided, it temporarily modifies the value of the variable,
 * displays the updated environment, and then reverts the value back to its original state.
 *
 * @return: Zero if successful, or a non-zero number if an error occurs.
 */
int builtin_env(data_of_program *data)
{
    int i;
    char cpname[50] = {'\0'};
    char *var_copy = NULL;

    /* If no arguments */
    if (data->tokens[1] == NULL)
        print_environ(data);
    else
    {
        for (i = 0; data->tokens[1][i]; i++)
        {
            /* Checks if '=' character exists */
            if (data->tokens[1][i] == '=')
            {
                /* Temporarily modify the value of the variable */
                var_copy = str_duplicate(env_get_key(cpname, data));
                if (var_copy != NULL)
                    env_set_key(cpname, data->tokens[1] + i + 1, data);

                /* Print the environment */
                print_environ(data);

                if (env_get_key(cpname, data) == NULL)
                {
                    /* Print the variable if it does not exist in the environment */
                    _print(data->tokens[1]);
                    _print("\n");
                }
                else
                {
                    /* Revert to the old value of the variable */
                    env_set_key(cpname, var_copy, data);
                    free(var_copy);
                }
                return (0);
            }
            cpname[i] = data->tokens[1][i];
        }
        errno = 2;
        perror(data->command_name);
        errno = 127;
    }
    return (0);
}

/**
 * builtin_set_env - Set or modify an environment variable.
 * @data: Struct for the program's data.
 *
 * This function sets or modifies an environment variable.
 * It requires two arguments in the form "name=value".
 * If a third argument is provided, an error is displayed.
 *
 * @return: Zero if successful, or a non-zero number if an error occurs.
 */
int builtin_set_env(data_of_program *data)
{
    /* Validate arguments */
    if (data->tokens[1] == NULL || data->tokens[2] == NULL)
        return (0);

    if (data->tokens[3] != NULL)
    {
        errno = E2BIG;
        perror(data->command_name);
        return (5);
    }

    env_set_key(data->tokens[1], data->tokens[2], data);

    return (0);
}

/**
 * builtin_unset_env - Remove an environment variable.
 * @data: Struct for the program's data.
 *
 * This function removes an environment variable.
 * It requires one argument, the name of the variable to be removed.
 * If more than one argument is provided, an error is displayed.
 *
 * @return: Zero if successful, or a non-zero number if an error occurs.
 */
int builtin_unset_env(data_of_program *data)
{
    /* Validate arguments */
    if (data->tokens[1] == NULL)
        return (0);

    if (data->tokens[2] != NULL)
    {
        errno = E2BIG;
        perror(data->command_name);
        return (5);
    }

    env_remove_key(data->tokens[1], data);

    return (0);
}
