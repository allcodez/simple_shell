#include "shell.h"

/**
 * print_alias - Print, add, remove, or show aliases.
 * @data: Struct for the program's data.
 * @alias: Name of the alias to be printed.
 *
 * This function prints the requested alias or all aliases if no specific alias is provided.
 * It iterates through the alias list and prints the aliases that match the provided name.
 *
 * @return: Zero if successful, or a non-zero number if the alias is not found.
 */
int print_alias(data_of_program *data, char *alias)
{
    int i, j, alias_length;
    char buffer[250] = {'\0'};

    if (data->alias_list)
    {
        alias_length = str_length(alias);
        for (i = 0; data->alias_list[i]; i++)
        {
            if (!alias || (str_compare(data->alias_list[i], alias, alias_length)
                && data->alias_list[i][alias_length] == '='))
            {
                for (j = 0; data->alias_list[i][j]; j++)
                {
                    buffer[j] = data->alias_list[i][j];
                    if (data->alias_list[i][j] == '=')
                        break;
                }
                buffer[j + 1] = '\0';
                buffer_add(buffer, "'");
                buffer_add(buffer, data->alias_list[i] + j + 1);
                buffer_add(buffer, "'\n");
                _print(buffer);
            }
        }
    }

    return (0);
}

/**
 * get_alias - Get the value of an alias.
 * @data: Struct for the program's data.
 * @name: Name of the requested alias.
 *
 * This function retrieves the value of the requested alias.
 * It iterates through the alias list and checks for a match with the provided name.
 *
 * @return: The value of the alias if found, or NULL if not found.
 */
char *get_alias(data_of_program *data, char *name)
{
    int i, alias_length;

    /* Validate the arguments */
    if (name == NULL || data->alias_list == NULL)
        return (NULL);

    alias_length = str_length(name);

    for (i = 0; data->alias_list[i]; i++)
    {
        /* Iterates through the alias list and checks for a match with the provided name */
        if (str_compare(name, data->alias_list[i], alias_length) &&
            data->alias_list[i][alias_length] == '=')
        {
            /* Returns the value of the alias when a match is found */
            return (data->alias_list[i] + alias_length + 1);
        }
    }

    /* Returns NULL if the alias is not found */
    return (NULL);
}

/**
 * set_alias - Set or override an alias.
 * @alias_string: Alias to be set in the form (name='value').
 * @data: Struct for the program's data.
 *
 * This function sets or overrides an alias.
 * It iterates through the alias list and checks if the alias already exists.
 * If the alias exists, it is replaced with the new alias.
 * If the alias value is another alias, it resolves the value and sets the alias accordingly.
 *
 * @return: Zero if successful, or a non-zero number if an error occurs.
 */
int set_alias(char *alias_string, data_of_program *data)
{
    int i, j;
    char buffer[250] = {'0'}, *temp = NULL;

    /* Validate the arguments */
    if (alias_string == NULL || data->alias_list == NULL)
        return (1);

    /* Iterate through the alias string to find the '=' character */
    for (i = 0; alias_string[i]; i++)
    {
        if (alias_string[i] != '=')
            buffer[i] = alias_string[i];
        else
        {
            /* Search if the value of the alias is another alias */
            temp = get_alias(data, alias_string + i + 1);
            break;
        }
    }

    /* Iterate through the alias list and check if the alias already exists */
    for (j = 0; data->alias_list[j]; j++)
    {
        if (str_compare(buffer, data->alias_list[j], i) &&
            data->alias_list[j][i] == '=')
        {
            /* If the alias already exists, free the memory */
            free(data->alias_list[j]);
            break;
        }
    }

    /* Add the alias */
    if (temp)
    {
        /* If the alias value is another alias */
        buffer_add(buffer, "=");
        buffer_add(buffer, temp);
        data->alias_list[j] = str_duplicate(buffer);
    }
    else
    {
        /* If the alias does not exist */
        data->alias_list[j] = str_duplicate(alias_string);
    }

    return (0);
}
