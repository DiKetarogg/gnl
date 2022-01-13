/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzoltan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 04:16:00 by jzoltan           #+#    #+#             */
/*   Updated: 2022/01/13 02:29:19 by jzoltan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

void	ft_string_free(struct s_ft_string *string)
{
	size_t	str_i;

	str_i = -1;
	while (++str_i < string->strs_count)
		free(string->strs[str_i]);
	free(string->strs);
	free(string);
}

static int	ft_string_add_str(struct s_ft_string *string)
{
	char	**strs;
	size_t	i;

	if (!(string->strs_count % 16))
	{
		strs = (char **)malloc(sizeof(char *) * (string->strs_count + 16));
		if (!(strs))
			return (0);
		i = -1;
		while (++i < string->strs_count)
			strs[i] = string->strs[i];
		free(string->strs);
		string->strs = strs;
	}
	string->strs[string->strs_count] = (char *)malloc(sizeof(char)
			* string->app_size);
	if (!string->strs[string->strs_count])
		return (0);
	++string->strs_count;
	return (1);
}

//	i = -1;
//	while (++i < string->app_size + 1)
//		string->strs[string->strs_count][i] = 0;

struct s_ft_string	*ft_string_init(size_t app_size)
{
	struct s_ft_string	*string;

	string = (struct s_ft_string *)malloc(sizeof(struct s_ft_string));
	if (!string)
		return (0);
	string->strs = 0;
	string->strs_count = 0;
	string->app_size = app_size;
	string->str_id = 0;
	string->c_id = 0;
	return (string);
}

char	*ft_string_to_chars(struct s_ft_string *s)
{
	size_t const	size = sizeof(char) * (((s->strs_count == s->str_id)
				+ s->str_id) * s->app_size + s->c_id + 1);
	char			*chars;
	char			*ptr;
	size_t			i;
	size_t			j;

	chars = malloc(size);
	ptr = chars;
	if (!chars)
		return (0);
	i = 0;
	chars[size - 1] = 0;
	while (i != s->str_id)
	{
		j = 0;
		while (j != s->app_size)
			*ptr++ = s->strs[i][j++];
		++i;
	}
	j = 0;
	while (j != s->c_id)
		*ptr++ = s->strs[i][j++];
	return (chars);
}

int	ft_string_append_char(struct s_ft_string *string, char ch)
{
	if (string->str_id == string->strs_count)
		if (!ft_string_add_str(string))
			return (0);
	string->strs[string->str_id][string->c_id] = ch;
	++string->c_id;
	if (string->c_id == string->app_size)
	{
		string->c_id = 0;
		++string->str_id;
	}
	return (1);
}
