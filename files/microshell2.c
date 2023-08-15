/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anfreire <anfreire@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:41:31 by anfreire          #+#    #+#             */
/*   Updated: 2023/04/18 13:45:58 by anfreire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int	ft_putstr_fd2(char *str, char *arg)
{
	while (*str)
		write(2, str++, 1);
	if (arg)
		while(*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
	return (1);
}

int ft_execute(char *argv[], int i, int tmp_fd, char *env[])
{
	//overwrite ; or | or NULL with NULL to use the array as input for execve.
	//we are here in the child so it has no impact in the parent process.
	argv[i] = NULL;
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(argv[0], argv, env);
	return (ft_putstr_fd2("error: cannot execute ", argv[0]));
}

int	main(int argc, char *argv[], char *env[])
{
	int	i;
	int fd[2];
	int tmp_fd;
	(void)argc;	// is needed in exam, because the exam tester compiles with -Wall -Wextra -Werror

	i = 0;
	tmp_fd = dup(STDIN_FILENO);
	while (argv[i] && argv[i + 1]) //check if the end is reached
	{
		argv = &argv[i + 1];	//the new argv start after the ; or |
		i = 0;
		//count until we have all informations to execute the next child;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (strcmp(argv[0], "cd") == 0) //cd
		{
			if (i != 2)
				ft_putstr_fd2("error: cd: bad arguments", NULL);
			else if (chdir(argv[1]) != 0)
				ft_putstr_fd2("error: cd: cannot change directory to ", argv[1]);
		}
		else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0)) //exec in stdout
		{
			if (fork() == 0)
			{
				if (ft_execute(argv, i, tmp_fd, env))
					return (1);
			}
			else
			{
				close(tmp_fd);
				while(waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if(i != 0 && strcmp(argv[i], "|") == 0) //pipe
		{
			pipe(fd);
			if ( fork() == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_execute(argv, i, tmp_fd, env))
					return (1);
			}
			else
			{
				close(fd[1]);
				close(tmp_fd);
				tmp_fd = fd[0];
			}
		}
	}
	close(tmp_fd);
		// not needed in exam, but necessary if you want to use this tester:
		// https://github.com/Glagan/42-exam-rank-04/blob/master/microshell/test.sh
	return (0);
}



My version:
/*
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	error(char *str, char *arg)
{
	while (*str)
		write (2, str++, 1);
	if (arg && *arg)
		while (*arg)
			write (2, arg++, 1);
	write (2, "\n", 1);
	return (1);
}

int	ft_exec(char **av, int i, int tmp_fd, char **env)
{
	av[i] = NULL;
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(av[0], av, env);
	return (error("error: cannot execute ", av[0]));
}
void	check_cd(char **av, int i)
{
	if (i != 2)
		error("error: cd: bad arguments", NULL);
// This condition evaluates as false in case we can't change dir
//chdir function is a build in func that changes directory to the given path
	else if (chdir(av[1]) != 0)
		error("error: cd: cannot change directory to ", av[1]);
}

void	isolated_cmd(char **av, int i, int t_fd, char **env)
{
	if (fork() == 0)
	{
		if (ft_exec(av, i, t_fd, env))
			return ;
	}
	else
	{
		close (t_fd);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		t_fd = dup(STDIN_FILENO);
	}
}

void	pipe_manager(int *fd, int *t_fd, char **av, char **env, int i)
{
	pipe(fd);
	if ( fork() == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (ft_exec(av, i, *t_fd, env))
			return ;
	}
	else
	{
		close(fd[1]);
		close(*t_fd);
		*t_fd = fd[0];
	}
}

int	main (int ac, char **av, char **env)
{
	int	i;
	int	fd[2];
	int	t_fd;
	(void)ac;

	i = 0;
	//Dup one duplicates a file descriptor and returns the duplicate one
	//Dup2 takes two arguments the first to duplicate that to the second 
	t_fd = dup(STDIN_FILENO);
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (strcmp(av[0], "cd") == 0)
			check_cd(av, i);
		else if (i != 0 && av[i] == NULL || strcmp(av[i], ";") == 0)
			isolated_cmd(av, i, t_fd, env);
		else if (i != 0 && strcmp(av[i], "|") == 0)
			pipe_manager(fd, &t_fd, av, env, i);
	}
	close(t_fd);
}*/