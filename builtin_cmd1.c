/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourgeo <hbourgeo@student.19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 08:37:25 by hbourgeo          #+#    #+#             */
/*   Updated: 2022/09/19 08:37:25 by hbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	my_export(t_cmdlist *cmd, t_prompt *p)
{
	int	i;
	int	error;

	g_status = 0;
	if (!cmd->full_cmd[1])
	{
		export_print_env(p);
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (cmd->full_cmd[i])
	{
		if (ft_strchr(cmd->full_cmd[i], '=') && export_error(cmd->full_cmd[i]))
			error = 1;
		else
			export_var(cmd->full_cmd[i], p);
		i++;
	}
	if (error == 1)
		g_status = 1;
	return (EXIT_SUCCESS);
}

int	my_cd(t_cmdlist *cmd, t_prompt *p)
{
	g_status = 0;
	if (!cmd->full_cmd[1] || cmd->full_cmd[1][0] == '~')
	{
		cd_to_home(cmd, p);
		return (EXIT_SUCCESS);
	}
	if (cmd->full_cmd[2])
		return (ft_error(TOOARGS, 1, "cd", NULL));
	if (chdir(cmd->full_cmd[1]))
		return (ft_error(PERROR, 1, "cd: ", cmd->full_cmd[1]));
	update_pwd(p);
	return (EXIT_SUCCESS);
}

int	my_unset(t_cmdlist *cmd, t_prompt *p)
{
	int	i;
	int	error;

	i = 1;
	g_status = 0;
	if (cmd->full_cmd[1] && cmd->full_cmd[1][0] == '-')
		return (ft_error(OPT, 1, "unset: ", cmd->full_cmd[1]));
	while (cmd->full_cmd[i])
	{
		if (unset_error(cmd->full_cmd[i]))
			error = 1;
		else if (my_getenv(cmd->full_cmd[i], p->env,
				ft_strlen(cmd->full_cmd[i])))
			drop_env(p, cmd->full_cmd[i]);
		i++;
	}
	if (error == 1)
		g_status = 1;
	return (EXIT_SUCCESS);
}

int	my_env(t_cmdlist *cmd, t_prompt *p)
{
	int	i;

	i = 0;
	g_status = 0;
	if (cmd->full_cmd[1] && cmd->full_cmd[1][0] == '-')
		return (ft_error(OPT, 1, "env: ", cmd->full_cmd[1]));
	else if (cmd->full_cmd[1])
		return (ft_error(TOOARGS, 1, "env", NULL));
	while (p->env[i])
	{
		printf("%s\n", p->env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	my_exit(t_cmdlist *cmd, t_prompt *p)
{
	if (!cmd->full_cmd[1])
	{
		g_status = (g_status >> 8) & 0xff;
		ft_putstr_fd("exit\n", 2);
		p->is_exit = 1;
		return (EXIT_SUCCESS);
	}
	if (cmd->full_cmd[1] && ft_isnum_sign(cmd->full_cmd[1]) == 0)
	{
		ft_putstr_fd("exit\n", 2);
		p->is_exit = 1;
		return (ft_error(NUMARGS, 255, "exit", cmd->full_cmd[1]));
	}
	if (cmd->full_cmd[2])
		return (ft_error(TOOARGS, 1, "exit", NULL));
	g_status = ft_atoi(cmd->full_cmd[1]);
	ft_putstr_fd("exit\n", 2);
	p->is_exit = 1;
	return (EXIT_SUCCESS);
}
