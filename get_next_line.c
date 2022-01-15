/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzoltan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 04:15:48 by jzoltan           #+#    #+#             */
/*   Updated: 2022/01/14 15:13:03 by jzoltan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

typedef struct s_list	*t_list;

static void	list_init(t_list *l, int fd, size_t app_size)
{
	t_list	list;

	if (*l)
		return ;
	list = malloc(sizeof(struct s_list));
	if (!list)
		return ;
	list->root = 0;
	list->next = 0;
	list->fd = fd;
	list->b_index = BUFFER_SIZE;
	list->b_end = BUFFER_SIZE;
	list->string = ft_string_init(app_size);
	if (!list->string)
	{
		free(list);
		list = 0;
	}
	*l = list;
	return ;
}

static t_list	list_get_node(t_list node, int fd)
{
	if (!node)
		return (0);
	while (node->next != 0)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	if (node->fd == fd)
		return (node);
	node->next = malloc(sizeof(struct s_list));
	if (!node->next)
		return (0);
	node->next->root = node;
	node->next->next = 0;
	node->next->fd = fd;
	node->next->b_index = BUFFER_SIZE;
	node->next->b_end = BUFFER_SIZE;
	node->next->string = ft_string_init(4096);
	if (!node->next->string)
		free(node->next);
	return (node->next);
}

static void	list_pop(t_list *root, t_list node)
{
	if (node->root)
	{
		node->root->next = node->next;
		if (node->next)
			node->next->root = node->root;
	}
	else if (node->next)
	{
		node->next->root = 0;
		*root = node->next;
	}
	else
		*root = 0;
	ft_string_free(node->string);
	free(node);
}

static int	get_next_line_internal(t_list node)
{
	int	r;

	r = 1;
	while (r > 0)
	{
		while (node->b_index != node->b_end)
		{
			ft_string_append_char(node->string, node->buffer[node->b_index]);
			if (node->buffer[node->b_index++] == '\n')
				return (1);
		}
		if (node->b_index == BUFFER_SIZE)
		{
			r = read(node->fd, &node->buffer, BUFFER_SIZE);
			if (r < 0)
				return (0);
			node->b_end = (r > -1) * r;
			node->b_index = 0;
		}
		else
			break ;
	}
	return (node->string->str_id != 0 || node->string->c_id != 0);
}

char	*get_next_line(int fd)
{
	static t_list	list = 0;
	t_list			node;
	char			*ret;

	list_init(&list, fd, 4096);
	node = list_get_node(list, fd);
	if (!node)
		return (0);
	if (!get_next_line_internal(node))
	{
		list_pop(&list, node);
		return (0);
	}
	ret = ft_string_to_chars(node->string);
	node->string->c_id = 0;
	node->string->str_id = 0;
	if (node->b_end == node->b_index)
		list_pop(&list, node);
	return (ret);
}
