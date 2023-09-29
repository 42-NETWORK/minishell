/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haarab <haarab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:33:44 by emohamed          #+#    #+#             */
/*   Updated: 2023/09/29 20:54:46 by haarab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup_against(const char *s1)
{
	int		i;
	char	*p;

	i = 0;
	// p = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	p = malloc_((ft_strlen(s1) + 1), NULL, 0, NULL);
	if (!p)
		return (NULL);
	while (s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*p;

	i = 0;
	// p = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	p = malloc_((ft_strlen(s1) + 1), NULL, 0, NULL);
	if (!p)
		return (NULL);
	while (s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
