/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haarab <haarab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:36:58 by haarab            #+#    #+#             */
/*   Updated: 2023/09/12 18:37:15 by haarab           ###   ########.fr       */
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