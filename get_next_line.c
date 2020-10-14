/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtrancos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 13:34:48 by jtrancos          #+#    #+#             */
/*   Updated: 2020/02/21 12:53:19 by jtrancos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Con esta funcion buscamos las lineas y las guardamos una a una.
** 1. Chequeamos el stack buscando la primera linea.
** 2. Hacemos un substr guardando la linea.
** 3. Hacemos copia de stack en temp y duplicamos la linea con strdup en stack.
** 4. Liberamos temp y devolvemos 1 si ha ido bien.
*/

static int	check_stack(char **stack, char **line)
{
	char	*temp;

	if (ft_strchr(*stack, '\n'))
	{
		*line = ft_substr(*stack, 0, ft_strlen(*stack) -
				ft_strlen(ft_strchr(*stack, '\n')));
		temp = *stack;
		*stack = ft_strdup(ft_strchr(temp, '\n') + 1);
		free(temp);
		return (1);
	}
	else
		return (0);
}

/*
** Esta funcion lee las lineas del archivo
** 1. Alocamos memoria para buf
** 2. Mientras que haya texto, lee y une el stack con el buf. Luego libera.
** 3. Si llega al final del texto hace un strdup de buf.
** 4. Si llega al final de linea, rompe el bucle.
** Devuelve el numero de bytes leidos.
*/

static int	read_line(int fd, char **stack)
{
	char	*buf;
	int		bytes;
	char	*temp;

	buf = malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buf)
		return (-1);
	while ((bytes = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[bytes] = '\0';
		if (stack[fd])
		{
			temp = stack[fd];
			stack[fd] = ft_strjoin(temp, buf);
			free(temp);
		}
		else
			stack[fd] = ft_strdup(buf);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	free(buf);
	return (bytes);
}

/*
** Esta funcion lee el archivo.
** 1. Hacemos las pruebas a errores y si falla damos -1.
** 2.
*/

int			get_next_line(int fd, char **line)
{
	static char		*stack[4096];
	int				ret;

	if (fd < 0 || read(fd, stack[fd], 0) == -1 || !line || BUFF_SIZE <= 0)
		return (-1);
	if (stack[fd])
		if (check_stack(&stack[fd], line))
			return (1);
	ret = read_line(fd, stack);
	if (ret == 0 && !stack[fd])
	{
		*line = ft_strdup("");
		return (0);
	}
	if (check_stack(&stack[fd], line))
		return (1);
	if (!ft_strchr(stack[fd], '\n'))
	{
		*line = ft_strdup(stack[fd]);
		free(stack[fd]);
		stack[fd] = NULL;
		return (0);
	}
	return (0);
}
