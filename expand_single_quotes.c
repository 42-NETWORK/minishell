/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_single_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emohamed <emohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 10:58:11 by emohamed          #+#    #+#             */
/*   Updated: 2023/09/30 02:29:24 by emohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	count_single_quotes(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '\''))
		{
			count++;
		}
		i++;
	}
	return (count);
}

char	*remove_single_quotes(const char *token)
{
	int		token_length;
	char	*modified_token;
	int		k;
	int		l;

	token_length = ft_strlen(token);
	modified_token = malloc_((token_length * sizeof(char)), NULL, 0, NULL);
	if (modified_token == NULL)
	{
		return (NULL);
	}
	k = 0;
	l = 0;
	while (l < token_length)
	{
		if (token[l] != '\'')
		{
			modified_token[k] = token[l];
			k++;
		}
		l++;
	}
	modified_token[k] = '\0';
	return (modified_token);
}

char	**expand_s_quotes(char **tokens)
{
	int		num_quotes;
	int		total_tokens;
	int		i;
	char	**expanded_tokens;
	int		j;
	char	*current_token;
	char	*modified_token;

	num_quotes = count_single_quotes(tokens);
	total_tokens = 0;
	i = 0;
	while (tokens[i])
	{
		total_tokens++;
		i++;
	}
	expanded_tokens = malloc_(((total_tokens + num_quotes + 1)
				* sizeof(char *)), NULL, 0, NULL);
	if (expanded_tokens == NULL)
	{
		return (NULL);
	}
	i = 0;
	j = 0;
	while (tokens[i])
	{
		current_token = tokens[i];
		if (ft_strchr(current_token, '\''))
		{
			expanded_tokens[j] = remove_single_quotes(current_token);
		}
		else
		{
			expanded_tokens[j] = ft_strdup(current_token);
		}
		if (expanded_tokens[j] == NULL)
		{
			return (NULL);
		}
		i++;
		j++;
	}
	expanded_tokens[j] = NULL;
	return (expanded_tokens);
}
