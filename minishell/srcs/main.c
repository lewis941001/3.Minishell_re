/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwchoi <jiwchoi@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 13:00:58 by jiwchoi           #+#    #+#             */
/*   Updated: 2021/11/14 19:00:04 by jiwchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_print(t_cmd *cmd, char *line)
{
	int	i;

	printf("%s\n", line);
	printf("--------------------\n");
	i = 1;
	while (cmd)
	{
		printf("%d\n", i++);
		while (*cmd->argv)
			printf("[%s]\n", *cmd->argv++);
		printf("redirect\n");
		while (cmd->redirect)
		{
			printf("[%d %s]\n", cmd->redirect->type, cmd->redirect->file);
			cmd->redirect = cmd->redirect->next;
		}
		printf("\n");
		cmd = cmd->next;
	}
}

char	**find_envp_path()
{
	char	**envp_path;
	char 	*path;

   	path = getenv("PATH");
	if(path == NULL)
		printf("Failed to get path\n");
	envp_path = ft_split(path, ':');

	return (envp_path);
}

int exit_function(char *line)
{
	if (ft_strncmp(line, "exit", 4) == 0 && ft_strlen(line) == 4)
	{	
		//printf("%s\n", line);
		free(line);
		exit (1);
	}
	return (0);
}

int cmd_num(t_cmd *cmd)
{
	int i = 0;
	
	while(cmd)
	{
		cmd = cmd->next;
		++i;
	}
	return (i);
}

char *exe_parse(char **env, char *command_split)
{
	int i;
	char *env_path;
	//char *command_path;
	char *add_slash;

	struct stat buf;

	i = 0;
	while(env[i])
	{
		env_path = ft_strdup(env[i]);
		//free(env[i]); //>> 빨리 없애버리고 싶었으나 명령어가 안들어오면 free된 것이라고 함
		add_slash = ft_strjoin(env_path, "/");
		free(env_path);
		env_path = ft_strjoin(add_slash, command_split);

		//printf("env_path :%s\n", env_path);
		if(stat(env_path, &buf) == 0)
		{
			//printf("여기가 출력이 안되는데 함수가 실행이 되네?");
			//printf("%s", env_path);
			return (env_path);
		}
		++i;
	}
	printf("not invalid command\n");
	return (0);
}


int exe_just(t_cmd *cmd_lst, char **env)
{
	char *env_path;
	int status;
	pid_t pid;
	static int i;

	env_path = exe_parse(env, cmd_lst->argv[0]);
	if (env_path == 0)
	{
		printf("명령어가 없어서 함수 나가는중\n");
		return (1);
	}

	pid = fork(); // 이함수 안쓰면 waitpid == -1 로 해서 error
	if (pid == 0)
	{
		// printf("pid %d", pid[i]);
		i++;	
		//printf(" --------자식\n");
			execve(env_path, cmd_lst->argv, NULL);
	}
	else
	{
		// printf("pid %d", pid);
		waitpid(pid, &status, 0);
	}
	
	return (1);
}

// void exe_main(t_cmd *cmd, char **env)
// {
// 	if (cmd->next == NULL)
// 	{
// 		exe_just(cmd, env);
// 		return ;
// 	}
// 	char *env_path;
// 	int status;
// 	pid_t pid1;

// 	//fd[1] write
// 	//fd[0] read
// 	int in = 0;
// 	int out = 1;
	
// 	env_path = exe_parse(env, cmd->argv[0]);

// 	pipe(cmd->fd);
// 	out = cmd->fd[1];

// 	pid1 = fork();	
// 	if(pid1 == 0)
// 	{
// 		if (in != 0)
// 		{
// 			dup2 (in, 0);
// 			close (in);
// 		}
// 		if (out != 1)
// 		{
// 			dup2 (out, 1);
// 			close (out);
// 		}
// 		// close(cmd->fd[0]);
// 		// dup2(cmd->fd[1], STDOUT_FILENO);
// 		// close(cmd->fd[1]);
// 		execve(env_path, cmd->argv, NULL);
// 	}
// 	else
// 	{
// 		// close(cmd->fd[0]);
// 		// close(cmd->fd[1]);
// 		waitpid(pid1, &status, 0);
// 	}
// 	close(cmd->fd[1]);
// 	in = cmd->fd[0];

// 	t_cmd *cmd_next = cmd->next;

// 	// printf("%s", *(cmd->argv));
// 	pid1 = fork();

// 	env_path = exe_parse(env, cmd->next->argv[0]);
// 	if(pid1 == 0)
// 	{	
// 		if (in != 0)
// 		{
// 			dup2 (in, 0);
// 			close (in);
// 		}
// 		if (out != 1)
// 		{
// 			dup2 (out, 1);
// 			close (out);
// 		}
// 		dprintf(1, "here\n");
// 		execve(env_path, cmd_next->argv, NULL);
// 	}
// 	else 
// 	{
// 		// dup2(fd_out, 1);
// 		waitpid(pid1, &status, 0);
// 	}
	
// }

// void exe_main(t_cmd *cmd, char **env)
// {

// 	if (cmd->next == NULL)
// 	{
// 		exe_just(cmd, env);
// 		return ;
// 	}
//     pid_t pid, wpid;
//     int status;
// 	char *env_path;
//     int in;
//     int out;
//     int fd[2];
//     in = 0;
//     out = 1;
	
//     while(cmd != NULL)
//     {
//         if (cmd->next != NULL)
//         {
//             pipe(fd);
//             out = fd[1];
//         }
// 		env_path = exe_parse(env, cmd->argv[0]);
//         pid = fork();
//         if (pid == -1)
//             exit(EXIT_FAILURE);
//         else if (pid == 0)
//         {
//             if (in != 0)
//             {
//                 dup2 (in, 0);
//                 close (in);
//             }
//             if (out != 1)
//             {
//                 dup2 (out, 1);
//                 close (out);
//             }
//             if (execve(env_path, cmd->argv, NULL) == -1)
//                 perror("Could not execve");
//             exit(EXIT_SUCCESS);
//         }
//         else if (pid > 0)
//         {
//             //do {
//                 wpid = waitpid(pid, &status, WUNTRACED);
//             //} while (!WIFEXITED(status) && !WIFSIGNALED(status));
//         }
//         close(fd[1]);
//         in = fd[0];
//         // move current position
// 		cmd = cmd->next;
// 	}
// }



void exe_unset(t_env **env_lst, t_cmd *cmd)
{
	t_env *prev_env;
	t_env *move;
	(void)env_lst;
	(void)cmd;
	
	printf("%s %s\n", cmd->argv[0], cmd->argv[1]);

	prev_env = NULL;
	// prev_env->next = *env_lst;
	int i = 0;
	while (cmd->argv[i])
	{
		move = *env_lst;
	while (move)
	{	
		if (ft_strncmp(move->key, cmd->argv[i], ft_strlen(cmd->argv[i])) == 0
		&& ft_strncmp(move->key, cmd->argv[i], ft_strlen(move->key)) == 0)
		{
			printf("check : %s %s\n", move->key, move->value);
			if (prev_env == NULL)
			{
				printf("NULL ? %s %s\n", move->key, move->value);
				(*env_lst) = (*env_lst)->next;
				// prev_env = move->next;
			}
			else
			{
				printf("else : %s %s\n", move->key, move->value);
				prev_env->next = move->next;
			}
			// move->next = NULL;
			// free(move);
			break;
		}
		prev_env = move;
		move = move->next;
	}
		i++;
	}	
}

void exe_exit(t_cmd *cmd)
{
	if (cmd->argv[1])
	{
		write(1, cmd->argv[1], ft_strlen(cmd->argv[1]));
		write(1, "\n", 1);
	}
}

int is_built(char *cmd)
{
	if ((ft_strncmp((cmd), "pwd", 3) == 0 && ft_strlen(cmd) == 3)
	|| (ft_strncmp((cmd), "env", 3) == 0 && ft_strlen(cmd) == 3)
	|| (ft_strncmp((cmd), "export", 6) == 0 && ft_strlen(cmd) == 6)
	|| (ft_strncmp((cmd), "unset", 5) == 0 && ft_strlen(cmd) == 5)
	|| (ft_strncmp((cmd), "cd", 2) == 0 && ft_strlen(cmd) == 2)
	|| (ft_strncmp((cmd), "echo", 4) == 0 && ft_strlen(cmd) == 4)
	|| (ft_strncmp((cmd), "exit", 4) == 0 && ft_strlen(cmd) == 4))
		return (1);
	return (0);
}

void exe_builtin(t_cmd *cmd, t_env **env_lst)
{
	if (ft_strncmp((cmd->argv[0]), "pwd", 3) == 0 && ft_strlen(cmd->argv[0]) == 3)
		exe_pwd();
	else if (ft_strncmp((cmd->argv[0]), "env", 3) == 0 && ft_strlen(cmd->argv[0]) == 3)
		print_env(*env_lst);
	else if (ft_strncmp((cmd->argv[0]), "export", 6) == 0 && ft_strlen(cmd->argv[0]) == 6)
		exe_export(env_lst, cmd);
	else if (ft_strncmp((cmd->argv[0]), "unset", 5) == 0 && ft_strlen(cmd->argv[0]) == 5)
		exe_unset(env_lst, cmd);
	else if (ft_strncmp((cmd->argv[0]), "cd", 2) == 0 && ft_strlen(cmd->argv[0]) == 2)
		exe_cd(cmd, *env_lst);
	else if (ft_strncmp((cmd->argv[0]), "echo", 4) == 0 && ft_strlen(cmd->argv[0]) == 4)
		exe_echo(cmd);
	else if (ft_strncmp((cmd->argv[0]), "exit", 4) == 0 && ft_strlen(cmd->argv[0]) == 4)
		exe_exit(cmd);
}

void exe_process(t_cmd **cmd, char **env, t_env **env_list)
{
	pid_t pid;
	// int i = 0;
	int in = 0;
	int out = 1;
	char *env_path;
	int status;

	// if ((*cmd)->next == NULL)
	// {
	// 	exe_just(*cmd, env);
	// 	return ;
	// }

	int flag = cmd_num(*cmd);
	while (*cmd)
	{
		if (is_built((*cmd)->argv[0]) && flag == 1) //명령어가 하나이고, 명령어 길이가 1이다.
		{
			printf("only builtin\n");
			exe_builtin(*cmd, env_list);
			(*cmd) = (*cmd)->next;
		}
		else
		{
			if ((*cmd)->next != NULL)		//다음이 있다.
			{
				printf("here pipe\n");
				pipe((*cmd)->fd);
				out = (*cmd)->fd[1];
			}
			env_path = exe_parse(env, (*cmd)->argv[0]);
			pid = fork();
			if (pid == 0)
			{
				printf("cmd: %s\n", (*cmd)->argv[0]);
				if (in != 0) // 처음 빼고 , 파이프 있을 때
				{	
					dup2(in, 0);
					close(in);
				}
				if (out != 1 && (*cmd)->next != NULL) // 마지막에 빼고, 파이프 있을 때
				{
					dup2(out, 1);
					close(out);
				}

				// func
				if (is_built((*cmd)->argv[0])) //내가 만든 함수
				{
					printf("here_is_built\n");
					exe_builtin(*cmd, env_list); //실행
					exit(0);					//종료
				}
				else
				{
					if (execve(env_path, (*cmd)->argv, NULL) == -1)
						perror("cmd not found");
				}
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
			}
			// here !		//ls //ls | cat
			// printf("here_end\n");
			if((*cmd)->next != NULL) //마지막이면 close 해주면 안됨
				close(out); // 마지막일때는 굳이 할 필요는 없는 듯?
			in = (*cmd)->fd[0]; //연결하는 과정은 있든 없든 상관없음
			printf("here_end\n");
			(*cmd) = (*cmd)->next; //다음으로 넘기기
			// ++i;
		}
	}
	// printf("here\n");
}

// void exe(t_cmd *cmd, char **env, t_env **env_lst)
// {
// 	while(cmd)
// 	{	
// 		if (!made_built(cmd->argv[0]))
// 			exe_made(&cmd, env);
// 		else
// 		{	
// 			if (ft_strncmp((cmd->argv[0]), "pwd", 3) == 0 && ft_strlen(cmd->argv[0]) == 3)
// 				exe_pwd();
// 			else if (ft_strncmp((cmd->argv[0]), "env", 3) == 0 && ft_strlen(cmd->argv[0]) == 3)
// 				print_env(*env_lst);
// 			else if (ft_strncmp((cmd->argv[0]), "export", 6) == 0 && ft_strlen(cmd->argv[0]) == 6)
// 				exe_export(env_lst, cmd);
// 			else if (ft_strncmp((cmd->argv[0]), "unset", 5) == 0 && ft_strlen(cmd->argv[0]) == 5)
// 				exe_unset(env_lst, cmd);
// 			else if (ft_strncmp((cmd->argv[0]), "cd", 2) == 0 && ft_strlen(cmd->argv[0]) == 2)
// 				exe_cd(cmd, *env_lst);
// 			else if (ft_strncmp((cmd->argv[0]), "echo", 4) == 0 && ft_strlen(cmd->argv[0]) == 4)
// 				exe_echo(cmd);
// 			else if (ft_strncmp((cmd->argv[0]), "exit", 4) == 0 && ft_strlen(cmd->argv[0]) == 4)
// 				exe_exit(cmd);
// 		}
// 		cmd = cmd->next;
// 	}
// 	//env_lst 를 free해주거나 꺼내거나
// }


int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_cmd	*cmd;
	char **env;

	(void)argc;
	(void)argv;
	cmd = NULL;
	t_env *env_lst = NULL;
	
	exe_env(envp, &env_lst);
	env = find_envp_path();
	while(1)
	{
		line = readline("minishell$ ");
		//exit_function(line);
		if(line)
		{
			if (parse_line(&cmd, line, env_lst))
				return (EXIT_FAILURE);
			if (ft_strlen(line) > 0)
			{
				exe_process(&cmd, env, &env_lst);
				add_history (line);
			}
		}
		free(line);
		line = NULL;
		cmd_clear(&cmd);
	}
	return (EXIT_SUCCESS);
}
