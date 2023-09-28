/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emohamed <emohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 20:32:31 by emohamed          #+#    #+#             */
/*   Updated: 2023/09/28 08:34:12 by emohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
				&& str[i] <= 'Z'))
			return (1);
		i++;
	}
	return (0);
}

int	is_char_in2d(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if ((str[i][j] >= 'a' && str[i][j] <= 'z') || (str[i][j] >= 'A'
					&& str[i][j] <= 'Z'))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	lenght_of_the_2d(char **p)
{
	int	i;

	if (!p[0])
		return (0);
	i = 0;
	while (p[i])
	{
		i++;
	}
	return (i);
}

char	**split(char *s, char *delimiters)
{
	int		max_tokens;
	char	**tokens;
	int		token_count;
	char	*start;
	char	*end;
	int		in_quotes;
	char	quote_char;
	int		escaped;
	int		i;
	char	**new_tokens;

	max_tokens = ft_strlen(s) + 1;
	// tokens = (char **)malloc(max_tokens * sizeof(char *));
	tokens = malloc_((max_tokens * sizeof(char *)), NULL, 0, NULL);
	if (!tokens)
	{
		printf("allocation err\n");
		exit(1);
	}
	token_count = 0;
	start = s;
	end = s;
	in_quotes = 0;
	quote_char = '\0';
	escaped = 0;
	i = 0;
	while (end[i])
	{
		if (end[i] == '\'' || end[i] == '\"')
		{
			if (in_quotes && end[i] == quote_char)
			{
				in_quotes = 0;
				quote_char = '\0';
			}
			else if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = end[i];
			}
			escaped = 0;
		}
		else if (end[i] == '\\')
		{
			if (in_quotes)
			{
				escaped = !escaped;
			}
		}
		else if ((!in_quotes && ft_strchr(delimiters, end[i]) != NULL)
			|| (!in_quotes && (end[i] == ' ' || end[i] == '\t')))
		{
			if (start != end)
			{
				if (token_count >= max_tokens)
				{
					max_tokens *= 2;
					// new_tokens = (char **)malloc(max_tokens * sizeof(char *));
					new_tokens = malloc_((max_tokens * sizeof(char *)), NULL, 0, NULL);
					if (!new_tokens)
					{
						printf("allocation err\n");
						exit(1);
					}
					i = 0;
					while (i < token_count)
					{
						new_tokens[i] = tokens[i];
						i++;
					}
					// free(tokens);
					tokens = new_tokens;
				}
				// tokens[token_count] = (char *)malloc((end - start + 1) * sizeof(char));
				tokens[token_count] = malloc_(((end - start + 1) * sizeof(char)), NULL, 0, NULL);
				if (!tokens[token_count])
				{
					printf("allocation err\n");
					exit(1);
				}
				strncpy(tokens[token_count], start, end - start);
				tokens[token_count][end - start] = '\0';
				token_count++;
			}
			if (ft_strchr(delimiters, end[i]) != NULL)
			{
				if (token_count >= max_tokens)
				{
					max_tokens *= 2;
					// new_tokens = (char **)malloc(max_tokens * sizeof(char *));
					new_tokens = malloc_((max_tokens * sizeof(char *)), NULL, 0, NULL);
					if (!new_tokens)
					{
						printf("allocation err\n");
						exit(1);
					}
					i = 0;
					while (i < token_count)
					{
						new_tokens[i] = tokens[i];
						i++;
					}
					// free(tokens);
					tokens = new_tokens;
				}
				// tokens[token_count] = (char *)malloc(2 * sizeof(char));
				tokens[token_count] = malloc_((2 * sizeof(char)), NULL, 0, NULL);
				if (!tokens[token_count])
				{
					printf("allocation err\n");
					exit(1);
				}
				tokens[token_count][0] = end[i];
				tokens[token_count][1] = '\0';
				token_count++;
			}
			start = end + 1;
		}
		else if (escaped)
		{
			escaped = 0;
		}
		end++;
	}
	if (start != end)
	{
		if (token_count >= max_tokens)
		{
			max_tokens *= 2;
			// new_tokens = (char **)malloc(max_tokens * sizeof(char *));
			new_tokens = malloc_((max_tokens * sizeof(char *)), NULL, 0, NULL);
			if (!new_tokens)
			{
				printf("allocation err\n");
				exit(1);
			}
			i = 0;
			while (i < token_count)
			{
				new_tokens[i] = tokens[i];
				i++;
			}
			// free(tokens);
			tokens = new_tokens;
		}
		// tokens[token_count] = (char *)malloc((end - start + 1) * sizeof(char));
		tokens[token_count] = malloc_(((end - start + 1) * sizeof(char)), NULL, 0, NULL);
		if (!tokens[token_count])
		{
			printf("allocation err\n");
			exit(1);
		}
		strncpy(tokens[token_count], start, end - start);
		tokens[token_count][end - start] = '\0';
		token_count++;
	}
	tokens[token_count] = NULL;
	return (tokens);
}

char	**expand_quotes(char **tokens)
{
	int		i;
	int		num_quotes;
	char	**expanded_tokens;
	int		j;
	char	*current_token;
	int		token_length;
	char	*modified_token;
	int		k;
	int		l;

	i = 0;
	num_quotes = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '\"'))
		{
			num_quotes++;
		}
		i++;
	}
	// expanded_tokens = malloc((i + num_quotes + 1) * sizeof(char *));
	expanded_tokens = malloc_(((i + num_quotes + 1) * sizeof(char *)), NULL, 0, NULL);
	if (expanded_tokens == NULL)
	{
		return (NULL);
	}
	j = 0;
	i = 0;
	while (tokens[i])
	{
		current_token = tokens[i];
		if (ft_strchr(current_token, '\"'))
		{
			token_length = ft_strlen(current_token);
			// modified_token = malloc(token_length * sizeof(char));
			modified_token = malloc_((token_length * sizeof(char)), NULL, 0, NULL);
			if (modified_token == NULL)
			{
				return (NULL);
			}
			k = 0;
			l = 0;
			while (l < token_length)
			{
				if (current_token[l] != '\"')
				{
					modified_token[k] = current_token[l];
					k++;
				}
				l++;
			}
			modified_token[k] = '\0';
			expanded_tokens[j] = modified_token;
			j++;
		}
		else
		{
			expanded_tokens[j] = ft_strdup(current_token);
			if (expanded_tokens[j] == NULL)
			{
				return (NULL);
			}
			j++;
		}
		i++;
	}
	expanded_tokens[j] = NULL;
	return (expanded_tokens);
}

char	**expand_s_quotes(char **tokens)
{
	int		i;
	int		num_quotes;
	char	**expanded_tokens;
	int		j;
	char	*current_token;
	int		token_length;
	char	*modified_token;
	int		k;
	int		l;

	i = 0;
	num_quotes = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '\''))
		{
			num_quotes++;
		}
		i++;
	}
	// expanded_tokens = malloc((i + num_quotes + 1) * sizeof(char *));
	expanded_tokens = malloc_(((i + num_quotes + 1) * sizeof(char *)), NULL, 0, NULL);
	if (expanded_tokens == NULL)
	{
		return (NULL);
	}
	j = 0;
	i = 0;
	while (tokens[i])
	{
		current_token = tokens[i];
		if (ft_strchr(current_token, '\''))
		{
			token_length = ft_strlen(current_token);
			// modified_token = malloc(token_length * sizeof(char));
			modified_token = malloc_((token_length * sizeof(char)), NULL, 0, NULL);
			if (modified_token == NULL)
			{
				return (NULL);
			}
			k = 0;
			l = 0;
			while (l < token_length)
			{
				if (current_token[l] != '\'')
				{
					modified_token[k] = current_token[l];
					k++;
				}
				l++;
			}
			modified_token[k] = '\0';
			expanded_tokens[j] = modified_token;
			j++;
		}
		else
		{
			expanded_tokens[j] = ft_strdup(current_token);
			if (expanded_tokens[j] == NULL)
			{
				return (NULL);
			}
			j++;
		}
		i++;
	}
	expanded_tokens[j] = NULL;
	return (expanded_tokens);
}

char	**make_token(char *s)
{
	char	*special_chars;
	char	**tokens;
	char	**quote;

	special_chars = "<>|";
	tokens = split(s, special_chars);
	quote = expand_quotes(tokens);
	return (red_to_herdoc(tokens));
	// exit(1);
}

char	*alloc_s(char const *s, unsigned int start, int len)
{
	char	*stock;

	if (!s || !s[0])
		return (NULL);
	if (start >= ft_strlen(s))
		return ("");
	if (len > ft_strlen(s))
		len = ft_strlen(s);
	stock = ft_strdup(s + start);
	if (!stock)
		return (NULL);
	stock[len] = '\0';
	return (stock);
}

char	*ft_strndup(char const *str, size_t max)
{
	size_t	index;
	char	*dst;

	index = 0;
	// dst = malloc(sizeof(char) * (max + 1));
	dst = malloc_((sizeof(char) * (max + 1)), NULL, 0, NULL);
	if (!dst)
	{
		return (0);
	}
	while (str[index] && index < max)
	{
		dst[index] = str[index];
		index++;
	}
	dst[index] = '\0';
	return (dst);
}

int get_var_size(char *str) {
	int i = 0;
	while (str[i] && (ft_isalpha(str[i]))) {
		i++;
	}
	return i;
}

int is_var_inside_sq(char *arg, int index) {
	int count = 0;
	int count_sq = 0;
	int count_dq = 0;
	while (arg[count]) {
		if (arg[count] == '\'' && count_dq % 2 == 0)
			count_sq++;
		if (arg[count] == '\"')
			count_dq++;
		if (count == index && count_sq % 2 == 0)
			return 0;
		else if (count == index && count_sq % 2 != 0)
			return 1;
		count++;
	}
	return 0;
}

char *expand_var(char *arg, t_vars *vars) {
	int i = 0;
	char *key;
	char *dst1;
	char *dst2;
	char *dst3;
	char *exp_value;

	while (i < ft_strlen(arg)) {
		if (arg[i] == '$' && !is_var_inside_sq(arg, i)) {
			dst1 = ft_strndup(arg, i);
			if (arg[i + 1] && (arg[i + 1] == '?')) {
				i++;
				exp_value = ft_itoa(vars->exit_status);
			}else {				
				key = ft_strndup(&arg[i + 1], get_var_size(&arg[i + 1]));
				exp_value = ft_getenv(key, vars);
				if (!exp_value)
					exp_value = "";
			}
			dst3 = arg + ((i + 1) + get_var_size(&arg[i + 1]));
			dst2 = ft_strjoin(dst1, exp_value);
			arg = ft_strjoin(dst2, dst3);
			i = 0;
		}
		i++;
	}
	return arg;
}

t_info	**allocat_token(char **s, t_vars *vars)
{
	int		i;
	t_info	**inf;
	char	*trimmed_content;
	char	*var;

	i = 0;
	// inf = malloc(sizeof(t_info *) * (lenght_of_the_2d(s) + 1));
	inf = malloc_((sizeof(t_info *) * (lenght_of_the_2d(s) + 1)), NULL, 0, NULL);
	if (!inf)
	{
		printf("Err\n");
		exit(1);
	}
	while (s[i])
	{
		// inf[i] = malloc(sizeof(t_info));
		inf[i] = malloc_((sizeof(t_info)), NULL, 0, NULL);
		inf[i]->content = s[i];
		inf[i]->size = lenght_of_the_2d(s);
		inf[i]->content = expand_var(inf[i]->content, vars);
		
		// if (inf[i]->content[0] == '\''
		// 	&& inf[i]->content[ft_strlen(inf[i]->content) - 1] == '\'')
		// {
		// 	if (inf[i]->content[1] == '\''
		// 		&& inf[i]->content[ft_strlen(inf[i]->content) - 1] == '\'')
		// 	{
		// 		trimmed_content = ft_strtrim(inf[i]->content, "\'");
		// 		if (ft_strlen(trimmed_content) > 0)
		// 		{
		// 			if (trimmed_content[0] == '$'
		// 				&& ft_isalpha(trimmed_content[1]))
		// 			{
		// 				var = ft_getenv(trimmed_content + 1, vars);
		// 				if (!var)
		// 				{
		// 					return (0);
		// 				}
		// 				inf[i]->content = ft_strdup(var);
		// 				inf[i]->type = "ENV_EXPANDED";
		// 				inf[i]->lenght = ft_strlen(inf[i]->content);
		// 			}
		// 		}
		// 	}
		// }
	// if (inf[i]->content[0] == '\'' &&
   	// 	inf[i]->content[ft_strlen(inf[i]->content) - 1] == '\'' &&
    // 	ft_strlen(inf[i]->content) > 2)
	// 	{

    // 	trimmed_content = ft_strtrim(inf[i]->content, "'");
    // 	inf[i]->content = ft_strdup(trimmed_content);
    // 	inf[i]->type = "STR";
    // 	inf[i]->lenght = ft_strlen(inf[i]->content);
	// }

	// 	else if ((inf[i]->content[0] == '$' && ft_isalpha(inf[i]->content[1])))
	// 	{
	// 		var = ft_getenv(inf[i]->content + 1, vars);
	// 		if (!var)
	// 		{
	// 			return (0);
	// 		}
	// 		inf[i]->content = ft_strdup(var);
	// 		inf[i]->type = "ENV_EXPANDED";
	// 		inf[i]->lenght = ft_strlen(inf[i]->content);
	// 	}
	// 	else if (inf[i]->content[0] == '$' && inf[i]->content[1] == '?')
	// 	{
	// 		inf[i]->content = ft_itoa(vars->exit_status);
    // 		inf[i]->type = "EXIT_STATUS"; 
    // 		inf[i]->lenght = ft_strlen(inf[i]->content);
	// 	}
		 if (inf[i]->content[0] == '<')
			inf[i]->type = "RDIN";
		else if (inf[i]->content[0] == '>')
			inf[i]->type = "RDOUT";
		else if (inf[i]->content[0] == '|')
			inf[i]->type = "PIPE";
		else if (inf[i]->content[0] == '\"')
			inf[i]->type = "DBCOTE";
		if (inf[i]->content[0] == '\'')
			inf[i]->type = "SGCOTE";
		else if (is_char(s[i]))
			inf[i]->type = "STR";
		inf[i]->lenght = ft_strlen(inf[i]->content);
		i++;
	}
	return (inf);
}
