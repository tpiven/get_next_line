/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpiven <tpiven@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 14:50:06 by tpiven            #+#    #+#             */
/*   Updated: 2017/12/22 22:01:23 by tpiven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list	*ft_fdlist(t_list **list, int fd)
{
	t_list		*temp;

	temp = *list;
	while (temp)
	{
		if ((int)temp->content_size == fd)
			return (temp);
		temp = temp->next;
	}
	temp = ft_lstnew(NULL, 0);
	temp->content_size = (size_t)(fd);
	ft_lstadd(&(*list), temp);
	*list = temp;
	return (temp);
}

static char		*ft_newstr(char *str, char *buffer)
{
	char		*s;

	if (str == NULL)
		s = ft_strdup(buffer);
	else
	{
		s = (char*)malloc(sizeof(char) * (ft_strlen(str) + BUFF_SIZE + 1));
		s = ft_strcpy(s, str);
		s = ft_strcat(s, buffer);
		ft_strdel(&str);
	}
	return (s);
}

static char		*ft_str(char *str, int start)
{
	char		*s;

	s = ft_strsub(str, start, ft_strlen(ft_strchr(str, '\n')));
	ft_strdel(&str);
	return (s);
}

static int		ft_get_line(char **str, char **line)
{
	int			i;

	i = 0;
	if (!(*str))
		return (0);
	if (!ft_strchr(*str, '\n'))
	{
		*line = ft_strdup(*str);
		ft_strdel(&(*str));
		return (1);
	}
	while ((*str)[i] != '\n')
		i++;
	*line = ft_strsub(*str, 0, i);
	*str = ft_str(*str, i + 1);
	if (ft_strlen(*str) == 0)
		ft_strdel(&(*str));
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	char			buffer[BUFF_SIZE + 1];
	static t_list	*list;
	t_list			*temp;
	int				ret;

	if (fd < 0 || !line || (read(fd, buffer, 0)) < 0)
		return (-1);
	temp = ft_fdlist(&list, fd);
	if (temp->content && ft_strchr((char*)temp->content, '\n'))
		return (ft_get_line((char**)&(temp->content), &(*line)));
	while ((ret = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		buffer[ret] = '\0';
		temp->content = ft_newstr((char*)(temp->content), buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (ft_get_line((char**)&(temp->content), &(*line)));
}
