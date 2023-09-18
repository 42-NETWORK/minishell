/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_herdo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emohamed <emohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 19:08:39 by emohamed          #+#    #+#             */
/*   Updated: 2023/09/18 09:55:18 by emohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


char **red_to_herdoc(char **tokens) 
{
    int len = lenght_of_the_2d(tokens) + 1;
    char **new_tokens = (char **)malloc(sizeof(char *) * len);

    int i = 0;
    int j = 0;

    while (tokens[i]) 
    {
        if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "<") == 0) {
            new_tokens[j] = (char *)malloc(sizeof(char) * (strlen(tokens[i]) + 1));
            strcpy(new_tokens[j], tokens[i]);
            while (tokens[i + 1] && (strcmp(tokens[i], tokens[i + 1]) == 0)) 
            {
                strcat(new_tokens[j], tokens[i + 1]);
                i++;
            }
            j++;
        }
        else 
        {
            new_tokens[j] = (char *)malloc(strlen(tokens[i]) + 1);
            strcpy(new_tokens[j], tokens[i]);
            j++;
        }
        i++;
    }

    new_tokens[i] = NULL;


    return new_tokens;
}