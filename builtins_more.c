#include "shell.h"

/**
 * builtin_exit - Exit the program with a specified status.
 * @data: Struct for the program's data.
 * Return: 0 if successful, or a different number if specified in the arguments.
 */
int builtin_exit(data_of_program *data)
{
    int i;

    if (data->tokens[1] != NULL)
    {
        /* Check if the argument is a number */
        for (i = 0; data->tokens[1][i]; i++)
        {
            if ((data->tokens[1][i] < '0' || data->tokens[1][i] > '9') && data->tokens[1][i] != '+')
            {
                errno = 2;
                return (2);
            }
        }

        errno = _atoi(data->tokens[1]);
    }

    free_all_data(data);
    exit(errno);
}

/**
 * builtin_cd - Change the current directory.
 * @data: Struct for the program's data.
 * Return: 0 if successful, or a different number if specified in the arguments.
 */
int builtin_cd(data_of_program *data)
{
    char *dir_home = env_get_key("HOME", data);
    char *dir_old = NULL;
    char old_dir[128] = {0};
    int error_code = 0;

    if (data->tokens[1])
    {
        if (str_compare(data->tokens[1], "-", 0))
        {
            dir_old = env_get_key("OLDPWD", data);
            if (dir_old)
                error_code = set_work_directory(data, dir_old);
            _print(env_get_key("PWD", data));
            _print("\n");

            return (error_code);
        }
        else
        {
            return (set_work_directory(data, data->tokens[1]));
        }
    }
    else
    {
        if (!dir_home)
            dir_home = getcwd(old_dir, 128);

        return (set_work_directory(data, dir_home));
    }

    return (0);
}

/**
 * set_work_directory - Set the working directory.
 * @data: Struct for the program's data.
 * @new_dir: Path to be set as the working directory.
 * Return: 0 if successful, or a different number if specified in the arguments.
 */
int set_work_directory(data_of_program *data, char *new_dir)
{
    char old_dir[128] = {0};
    int err_code = 0;

    getcwd(old_dir, 128);

    if (!str_compare(old_dir, new_dir, 0))
    {
        err_code = chdir(new_dir);
        if (err_code == -1)
        {
            errno = 2;
            return (3);
        }
        env_set_key("PWD", new_dir, data);
    }

    env_set_key("OLDPWD", old_dir, data);
    return (0);
}

/**
 * builtin_help - Display help information about built-in commands.
 * @data: Struct for the program's data.
 * Return: 0 if successful, or a different number if specified in the arguments.
 */
int builtin_help(data_of_program *data)
{
    int i, length = 0;
    char *messages[6] = {NULL};

    messages[0] = HELP_MSG;

    /* Validate arguments */
    if (data->tokens[1] == NULL)
    {
        _print(messages[0] + 6);
        return (1);
    }

    if (data->tokens[2] != NULL)
    {
        errno = E2BIG;
        perror(data->command_name);
        return (5);
    }

    messages[1] = HELP_EXIT_MSG;
    messages[2] = HELP_ENV_MSG;
    messages[3] = HELP_SETENV_MSG;
    messages[4] = HELP_UNSETENV_MSG;
    messages[5] = HELP_CD_MSG;

    for (i = 0; messages[i]; i++)
    {
        /* Print the length of the string */
        length = str_length(data->tokens[1]);
        if (str_compare(data->tokens[1], messages[i], length))
        {
            _print(messages[i] + length + 1);
            return (1);
        }
    }

    /* If there is no match, print error and return -1 */
    errno = EINVAL;
    perror(data->command_name);
    return (0);
}

/**
 * builtin_alias - Add, remove, or show aliases.
 * @data: Struct for the program's data.
 * Return: 0 if successful, or a different number if specified in the arguments.
 */
int builtin_alias(data_of_program *data)
{
    int i = 0;

    /* If there are no arguments, print all aliases */
    if (data->tokens[1] == NULL)
        return (print_alias(data, NULL));

    while (data->tokens[++i])
    {
        /* If there are arguments, set or print each alias */
        if (count_characters(data->tokens[i], "="))
            set_alias(data->tokens[i], data);
        else
            print_alias(data, data->tokens[i]);
    }

    return (0);
}
