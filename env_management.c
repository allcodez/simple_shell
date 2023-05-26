#include "shell.h"

/**
 * env_get_key - Retrieve the value of an environment variable.
 * @key: The name of the environment variable.
 * @data: Struct for the program's data.
 * Return: A pointer to the value of the variable, or NULL if it doesn't exist.
 */
char *env_get_key(char *key, data_of_program *data)
{
    int i, key_length = 0;

    /* Validate the arguments */
    if (key == NULL || data->env == NULL)
        return (NULL);

    /* Obtain the length of the variable requested */
    key_length = str_length(key);

    for (i = 0; data->env[i]; i++)
    {
        /* Iterate through the environ and check for a match */
        if (str_compare(key, data->env[i], key_length) && data->env[i][key_length] == '=')
        {
            /* Return the value of the key NAME= when found */
            return (data->env[i] + key_length + 1);
        }
    }

    /* Return NULL if the variable was not found */
    return (NULL);
}

/**
 * env_set_key - Overwrite the value of an environment variable or create it if it doesn't exist.
 * @key: The name of the variable to set.
 * @value: The new value.
 * @data: Struct for the program's data.
 * Return: 1 if the parameters are NULL, 2 if there is an error, or 0 if successful.
 */
int env_set_key(char *key, char *value, data_of_program *data)
{
    int i, key_length = 0, is_new_key = 1;

    /* Validate the arguments */
    if (key == NULL || value == NULL || data->env == NULL)
        return (1);

    /* Obtain the length of the variable requested */
    key_length = str_length(key);

    for (i = 0; data->env[i]; i++)
    {
        /* Iterate through the environ and check for a match */
        if (str_compare(key, data->env[i], key_length) && data->env[i][key_length] == '=')
        {
            /* If the key already exists, free the entire variable as it will be recreated below */
            is_new_key = 0;
            free(data->env[i]);
            break;
        }
    }

    /* Create a string of the form key=value */
    data->env[i] = str_concat(str_duplicate(key), "=");
    data->env[i] = str_concat(data->env[i], value);

    if (is_new_key)
    {
        /* If the variable is new, it is created at the end of the current list and we need
           to put a NULL value in the next position */
        data->env[i + 1] = NULL;
    }

    return (0);
}

/**
 * env_remove_key - Remove a key from the environment.
 * @key: The key to remove.
 * @data: Struct for the program's data.
 * Return: 1 if the key was removed, 0 if the key does not exist.
 */
int env_remove_key(char *key, data_of_program *data)
{
    int i, key_length = 0;

    /* Validate the arguments */
    if (key == NULL || data->env == NULL)
        return (0);

    /* Obtain the length of the variable requested */
    key_length = str_length(key);

    for (i = 0; data->env[i]; i++)
    {
        /* Iterate through the environ and check for a match */
        if (str_compare(key, data->env[i], key_length) && data->env[i][key_length] == '=')
        {
            /* If the key exists, remove it */
            free(data->env[i]);

            /* Move the other keys one position down */
            i++;
            for (; data->env[i]; i++)
            {
                data->env[i - 1] = data->env[i];
            }

            /* Put a NULL value at the new end of the list */
            data->env[i - 1] = NULL;
            return (1);
        }
    }

    return (0);
}

/**
 * print_environ - Print the current environment.
 * @data: Struct for the program's data.
 * Return: Nothing.
 */
void print_environ(data_of_program *data)
{
    int j;

    for (j = 0; data->env[j]; j++)
    {
        _print(data->env[j]);
        _print("\n");
    }
}
