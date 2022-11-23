/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 18:27:18 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/19 18:43:25 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	secure_malloc_error(char **arr, int len)
{
	while (len--)
	{
		free(arr[len]);
		arr[len] = NULL;
	}
	free(arr);
	arr = NULL;
	return (0);
}

void	get_array_size(char const *s, char c, int *arrsize)
{
	int	i;

	i = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c)
		{
			(*arrsize)++;
			while (s[i] && s[i] == c)
				i++;
			continue ;
		}
		i++;
	}
	if (s[i - 1] != c)
		(*arrsize)++;
}

int	fill_array(const char *s, char c, char **result, int arrsize)
{
	int	start;
	int	end;
	int	i;

	end = 0;
	i = 0;
	while (i < arrsize - 1)
	{
		start = end;
		while (s[start] == c)
			start++;
		end = start;
		while (s[end] && s[end] != c)
			end++;
		result[i] = ft_calloc((end - start + 1), sizeof(char));
		if (!result[i])
			return (secure_malloc_error(result, end - start + 1));
		else
			ft_strlcpy(result[i], s + start, end - start + 1);
		i++;
	}
	return (i);
}

char	**ft_split_and_add_file(char const *s, char c, char *file)
{
	int		arrsize;
	int		place_for_file;
	char	**result;

	arrsize = 0;
	result = NULL;
	get_array_size(s, c, &arrsize);
	if (arrsize)
	{
		arrsize += 1;
		result = ft_calloc(arrsize + 1, sizeof(char *));
		if (!result)
			return (NULL);
		place_for_file = fill_array(s, c, result, arrsize);
		result[place_for_file] = file;
	}
	else
		result[0] = NULL;
	return (result);
}
