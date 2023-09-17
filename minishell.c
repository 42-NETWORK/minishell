/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haarab <haarab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 13:10:25 by emohamed          #+#    #+#             */
/*   Updated: 2023/09/16 22:47:08 by haarab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_cd(char **args, t_vars *vars)
{
    if (args[1])
        chdir(args[1]);
	else if (!args[1])
		chdir(ft_getenv("HOME", vars));
}

void fell_env_struct(t_vars *vars)
{
    int i = 0;
    char *key;
    while (vars->envp[i])
	{
        key = ft_split(vars->envp[i], '=')[0];
        vars->env[i].key = key;
        if (ft_strchr(vars->envp[i], '='))
		{
            vars->env[i].is_equal = 1;
            vars->env[i].value = ft_strchr(vars->envp[i], '=') + 1;
        }
        i++;
    }
}

char *ft_getenv(char *key, t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->env_number) {
		if (!ft_strncmp(vars->env[i].key, key, ft_strlen(key))) {
			return (vars->env[i].value);
		}
		i++;
	}
	return NULL;
}

char *get_path(t_vars *vars, char *cmd)
{
	int i = 0;
	char **path;
	char *res;
	
	if (ft_strchr(cmd, '/')) 
		return cmd;
	res = ft_getenv("PATH", vars);
	if(res == NULL)
		return (NULL);
	char *fullCmd;
	path = ft_split(res, ':');
	struct stat file_info;
	while (path[i]) {
		fullCmd = ft_strjoin(ft_strjoin(path[i], "/"), cmd);
		if (stat(fullCmd, &file_info) == 0) {
			return fullCmd;
		}
		i++;
	}
	return (NULL);
}

void run(char *cmd, char **args, t_vars *vars, char *str)
{
	char *cwd = getcwd(NULL, 1024);
	

    if (ft_strncmp(cmd, "echo", ft_strlen("echo")) == 0 && (ft_strchr(str, '>') == 0 && ft_strchr(str, '<') == 0  && ft_strchr(str, '|') == 0 ))
    {
		if (ft_strncmp(args[1], "$?", ft_strlen("$?")) == 0)
		{
			printf ("%d\n", vars->exit_status);
		}
		// printf ("%s\n", args[1]);
		else
        	run_echo(args, vars);
    }
	
    else if (ft_strncmp(cmd, "cd", ft_strlen("cd")) == 0)
    {
        run_cd(args, vars);
    }
	
	
    else if (ft_strncmp(cmd, "pwd", ft_strlen("pwd")) == 0)
    {
		printf("%s\n", cwd);
		vars->exit_status = 0;
    }

	else if (ft_strncmp(cmd, "export", ft_strlen("export")) == 0)
	{
		if (!args[1])
			export_cmd(vars, NULL, NULL);
		int i = 1;
		while (args[i])
		{
			export_cmd(vars, args[i], args);
			i++;
		}
	}
	
	else if (ft_strncmp(cmd, "env", ft_strlen("env")) == 0)
	{
		env_cmd(vars);
	}
	else if (ft_strncmp(cmd, "unset", ft_strlen("unset")) == 0)
	{
		int i = 1;
		while (args[i])
		{
			check_unset(args, vars, i);
			i++;
		}
		
	}
    else if (ft_strncmp(cmd, "exit", ft_strlen("exit")) == 0)
        exit (1);
	
	else
	{
		pipeline(args, vars);
		// int fdin = dup(STDIN_FILENO);
		// int fdou = dup(STDOUT_FILENO);
		int i = 0;
		while (i < vars->n_commandes)
		{
			if (vars->cmds[i].is_nex_pip)
			{
				// exec_cmds(vars, i);
				pipe_commands(vars, i);
			}
			else
			{
				exec_cmds(vars, i);
			// 	printf ("hamza\n");
			}
			i++;
		}
		// dup2(fdin, 0);
		// dup2(fdou, 1);
		// i = 0;
		// while(args[i])
		// {
		// 	printf("cmd ===    %s\n", args[i]);
		// 	i++;
		// }

		// int x = 0;
		// while(vars->cmds[x].cmds_args)
		// {
		// 	i = 0;
		// 	while(vars->cmds[x].cmds_args[i])
		// 	{
		// 		printf(" args === %s\n", vars->cmds[x].cmds_args[i]);
		// 		i++;
		// 	}
		// 	x++;
		// }
		// x++;
		// i = 0;
		// while(vars->cmds[i].smbol)
		// {
		// 	printf(" smbol ===%s\n", vars->cmds[i].smbol);
		// 	i++;
		// }
		// if (!ft_strchr(str, '|'))
		// {
		// 	exec_cmds(vars, cmd, e_cmd);
		// }
		// else
		// {
		// 	setup_redirs(args, vars);
		// 	pipeline(e_cmd, vars);
		// }
	}
}

void	setup_redirs(char **args, t_vars *vars)
{
	int i =0;

	while (args[i] && strcmp(args[i], "|"))
	{
		// printf("***%s***\n", args[i]);
		if(!strcmp(args[i], ">"))
		{
			++i;
			int fd = open(args[i], O_CREAT | O_TRUNC | O_RDWR, 0644);
			dup2(fd, 1);
		}
		if (!strcmp(args[i], "<"))
		{
			++i;
			int fd = open(args[i], O_RDONLY , 0644);
			dup2(fd, 0);
		}
		i++;
	}	
}

void exec_cmds(t_vars *vars, int i) 
{
	int id;
	char *path;
	path = get_path(vars, vars->cmds[i].cmd);
	
	// setup_redirs(red, vars);
	// printf(">>%s<<\n", path);
	id = fork();
	if (id == 0) 
	{
		if (path == NULL)
		{
			ft_putstr_fd("minishell : ", 2);
			ft_putstr_fd(vars->cmds[i].cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
			
		}
		execve(path, vars->cmds[i].cmds_args, vars->envp);
		perror("execve");
		exit(126);
	}
	wait(&vars->exit_status);
	vars->exit_status = WEXITSTATUS(vars->exit_status);
}

char	**get_cmds(t_info **info)
{
	int		len = 0;
	int		i;
	char	*cmd = NULL;

	len = (*info)->size;
	i = 0;
	while (i < len)
	{
		cmd = ft_strjoin(cmd, info[i]->content);
		cmd = ft_strjoin(cmd, " ");
		i++;
	}
	return (ft_split(cmd, ' '));
}


void pipe_commands(t_vars *vars, int i) 
{
	int status;
	char *path;
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        exit(1);
    }


	pid_t child1;
	child1 = fork();
	if (child1 == -1) 
	{
		perror("fork");
		exit(1);
	}
	if (child1 == 0) 
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		// if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		// {
		// 	perror("dup2");
		// 	exit(1);
		// }
		close(pipe_fd[1]);
		path = get_path(vars, vars->cmds[i].cmd);
		execve(path, vars->cmds[i].cmds_args, vars->envp);
		exit(0);
	} 
	else 
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		// if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		// {
		// 	perror("dup2");
		// 	exit(1);
		// }
		close(pipe_fd[0]);
		path = get_path(vars, vars->cmds[i + 1].cmd);
		// int id  = fork();
		// if (id == -1) 
		// {
		// 	perror("fork");
		// 	exit(1);
		// }
		// if (id  == 0)	
		// 	execve(path, vars->cmds[i + 1].cmds_args, vars->envp);
		// else
		// 	waitpid(id, &status, 0);
		waitpid(child1, &status, 0);
	}
}

char *swap(char **str)
{
    int  i = 0;
    char *s = NULL;
    while(str[i])
    {

      s = ft_strjoin(s, str[i]);
      i++;
      if (str[i])
        s = ft_strjoin(s, " ");
    }
    return s;
}

int lenght_of_2d_splited(char **p)
{
    int i = 0;
    while((ft_strncmp(p[i], "|", ft_strlen("|")) != 0))
    {
        i++;
    }
    return i;
}


void  pipeline(char **ptr, t_vars *vars)
{
	int i = 0;
	int j = 0;
	int l = 0;
	while (ptr[i])
	{
		if (((ft_strncmp(ptr[i], "|", ft_strlen("|")) != 0)
			&& (ft_strncmp(ptr[i], "<", ft_strlen("<")) != 0)
				&& (ft_strncmp(ptr[i], ">", ft_strlen(">")) != 0)))
		{
			if (((ft_strncmp("|", ptr[i + 1], ft_strlen(ptr[i + 1])) == 0)
				|| (ft_strncmp("<", ptr[i + 1], ft_strlen(ptr[i + 1])) == 0) 
					|| (ft_strncmp(">", ptr[i + 1], ft_strlen(ptr[i + 1])) == 0)
						|| ptr[i + 1] == NULL))
				l++;
		}
		i++;
	}
	vars->cmds = malloc(sizeof(t_cmds) * (l + 1));
	i = 0;
	int h = 0;
	int b = 0;
	while (i < l)
	{
		while (ptr[b] && (ft_strncmp("|", ptr[b], ft_strlen(ptr[b]))
			&& ft_strncmp("<", ptr[b], ft_strlen(ptr[b]))
				&& ft_strncmp(">", ptr[b], ft_strlen(ptr[b]))))
		{
			b++;
		}
		if ((!ft_strncmp("|", ptr[b], ft_strlen(ptr[b]))
			|| !ft_strncmp("<", ptr[b], ft_strlen(ptr[b]))
				|| !ft_strncmp(">", ptr[b], ft_strlen(ptr[b]))))
			b++;
		vars->cmds[i].cmds_args = malloc(sizeof(char *) * (b + 1));
		i++;
	}
	i = 0;
	b = 0;
	int k = 0;
	while (i < l)
	{
		k = 0;
		vars->cmds[i].cmd = ptr[b];
		while (ptr[b] && ft_strncmp("|", ptr[b], ft_strlen(ptr[b])))
		{
			vars->cmds[i].cmds_args[k] = ptr[b];
			b++;
			k++;
		}
		if (ptr[b] && !ft_strncmp("|", ptr[b], ft_strlen(ptr[b])))
		{
			// vars->cmds[i].smbol = ptr[b];
			vars->cmds[i].is_nex_pip = 1;
			k++;
		}
		else
			vars->cmds[i].is_nex_pip = 0;
		vars->cmds[i].cmds_args[k] = NULL;
		b++;
		i++;
	}
	vars->n_commandes = l;
}
	


void sigintHandler(int signal) 
{
	 if (signal == SIGINT)
    {
        // printf("\n");
        rl_on_new_line();
        // rl_replace_line("", 0);
        rl_redisplay();
    }
}

int main(int c, char **v, char **env)
{
	char **str;
	t_vars vars;
	char **cmds;
	
	cmds = NULL;
	v += c;
	vars.exit_status = 0;
	vars.envp = env;
    vars.env = malloc(sizeof(t_env) * (count_argiment(vars.envp)));
    fell_env_struct(&vars);
	vars.env_number = count_argiment(vars.envp);
    char *input;
    t_info **tokens = NULL; 
	signal(SIGINT, sigintHandler);
    while(1)
    {
        input = read_input();
        if (input == NULL)
            return (vars.exit_status);
		if (!input[0])
			continue;
		str =  make_token(input);
        // str = ft_split(input, ' ');
		if(str)
        {
			tokens = allocat_token(str, &vars);
			if(!tokens)
				continue;
			cmds = get_cmds(tokens);
			if (!cmds)
				return (0);
			syn_err(cmds, &vars);
			vars.count_argiment = lenght_of_the_2d(str);
			int fdin = dup(STDIN_FILENO);
			int fdou = dup(STDOUT_FILENO);
			// if (ft_strchr(input, '|') == NULL)
			run(tokens[0]->content, cmds, &vars, input);
			// else 
			// 	pipeline(cmds, &vars);
			dup2(fdin, 0);
			dup2(fdou, 1);
			// table(str, tokens);
        }
		// if (str[0] != NULL)
		// {
		// 	char* command = str[0];
		// 	char **args = str;
		// 	vars.count_argiment = count_argiment(args);
		// 	run(command, args, &vars);
		// }
    }
	
    return 0;
}