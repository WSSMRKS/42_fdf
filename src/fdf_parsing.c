/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:00:36 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/07 21:49:16 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

int	ft_input_handler(int argc, char **argv, t_vars *vars)
{
	int	fd;

	if (argc != 2)
		return (-1);
	else
	{
		if (ft_strnstr(argv[1], ".fdf", strlen(argv[1])) == NULL)
			ft_exit_err_msg("Error: Argument not a '.fdf' file.\n", vars);
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			ft_exit_err_msg("Error: No such file or directory.\n", vars);
		else
		{
			vars->img = NULL;
			vars->map_data = ft_calloc(sizeof(t_map_data), 1);
			if (!vars->map_data)
				ft_abort_err_msg("fatal: malloc error.\n", vars, 0);
			ft_init_t_map_data(vars->map_data);
			ft_parse_input(vars, fd);
		}
		close(fd);
	}
	return (0);
}

void	ft_malloc_lst(void **lst, t_vars *vars, int ymax)
{
	t_line_lst	*tlst;

	tlst = ft_calloc(sizeof(t_line_lst), 1);
	if (!tlst)
		ft_abort_err_msg("fatal: malloc error.\n", vars, ymax);
	ft_init_t_line_lst(tlst);
	*lst = tlst;
}

void	ft_parse_input(t_vars *vars, int fd)
{
	char		*line;
	char		*free_line;
	t_line_lst	*lines;
	t_line_lst	*lines_head;
	int			i;

	i = 0;
	line = ft_get_next_line(fd);
	if (!line)
		ft_exit_err_msg("Error: Empty file.\n", vars);
	ft_malloc_lst((void *)&lines, vars, 0);
	lines_head = lines;
	while (line)
	{
		free_line = line;
		lines->split_line = ft_split(line, ' ');
		free(free_line);
		ft_malloc_lst(&lines->next, vars, 0);
		lines = lines->next;
		i++;
		line = ft_get_next_line(fd);
	}
	if (lines_head->split_line)
		ft_fill_map(vars, lines_head, i);
}

void	ft_assign_map(t_vars *vars, t_line_lst *lines, int y, int x)
{
	
}

void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i)
{
	int			y;
	int			x;
	t_line_lst	*tmp;

	vars->map = ft_calloc(sizeof(t_point *), i);
	if (!vars->map)
		ft_abort_err_msg("fatal: malloc error.\n", vars, 0);
	y = 0;
	x = 0;
	while (lines->split_line[x])
		x++;
	vars->map_data->map_width = x;
	while (lines && lines->split_line)
	{
		ft_assign_map(vars, lines, y, x);
		vars->map[y] = ft_calloc(sizeof(t_point), vars->map_data->map_width);
		if (!vars->map[y])
			ft_abort_err_msg("fatal: malloc error.\n", vars, y);
		x = 0;
		while (x < vars->map_data->map_width)
		{
			if (!lines->split_line[x])
				break ;	
			vars->map[y][x].z = Z_SCALE * ft_validate_args(vars,
					lines->split_line[x], &vars->map_data->valid);
			free(lines->split_line[x]);
			lines->split_line[x] = NULL;
			ft_calc_points(x, y, vars, 0);
			x++;
		}
		if (lines->split_line[x] || x < vars->map_data->map_width)
		{
			while (lines->split_line[x])
				free(lines->split_line[x++]);
			ft_lstclear_fdf(&lines, free);
			ft_abort_err_msg("Error: Map error.\n", vars, y + 1);
		}
		free(lines->split_line);
		y++;
		tmp = lines;
		lines = lines->next;
		free(tmp);
	}
	free(lines);
	if(vars->map_data->valid == 0)
		ft_abort_err_msg("Error: Map error.\n", vars, y);
	vars->map_data->map_height = y;
}

int		ft_validate_args(t_vars *vars, char *str, int *valid)
{
	int		i;
	long	nbr;

	nbr = 0;
	i = 0;
	if (!str || (str[i] == '-' && str[i + 1] == '\0'))
		*valid = 0;
	if (*valid != 0 && str[i] == '-')
		i++;
	while (*valid != 0 && str[i] != '\0')
	{
		if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '\n' && i != 0))
			i++;
		else
			*valid = 0;
	}
	if (valid != 0)
		nbr = ft_atol(str);
	if (valid != 0 && str[i] == '\0' && i <= 11 && nbr >= -2147483648
		&& nbr <= 2147483647)
	{
		if (nbr < vars->map_data->size_z_min)
			vars->map_data->size_z_min = nbr;
		return ((int) nbr);
	}
	else
		return (*valid = 0);
}

void	ft_calc_points(int x, int y, t_vars *vars, int delta_y)
{
	float	alpha;

	alpha = 30 * (PI / (float)180);
	vars->map[y][x].x = vars->map_data->raster_x * (10 * x) * (float)cos(alpha)
		+ vars->map_data->raster_x * (10 * y) * (float)cos(alpha + 2)
		+ vars->map_data->raster_x * vars->map[y][x].z * cos(alpha - 2);
	vars->map[y][x].y = delta_y + vars->map_data->raster_y * (10 * x)
		* (float)sin(alpha)
		+ vars->map_data->raster_y * (10 * y) * (float)sin(alpha + 2)
		+ vars->map_data->raster_y * vars->map[y][x].z * sin(alpha - 2);
	if (vars->map[y][x].x > vars->map_data->size_x_max)
		vars->map_data->size_x_max = vars->map[y][x].x;
	if (vars->map[y][x].x < vars->map_data->size_x_min)
		vars->map_data->size_x_min = vars->map[y][x].x;
	if (vars->map[y][x].y > vars->map_data->size_y_max)
		vars->map_data->size_y_max = vars->map[y][x].y;
	if (vars->map[y][x].y < vars->map_data->size_y_min)
		vars->map_data->size_y_min = vars->map[y][x].y;
}

void	ft_size_map(t_vars *vars)
{
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	if (vars->map_data->img_width > (WIDTH - EDGE) || vars->map_data->img_height > HEIGHT)
	{
		vars->map_data->raster_y = (HEIGHT - EDGE) / (float)vars->map_data->img_height;
		vars->map_data->raster_x = (WIDTH - EDGE) / (float)vars->map_data->img_width;
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	else if (vars->map_data->img_height > 0 && vars->map_data->img_width > 0)
	{
		vars->map_data->raster_y = ((HEIGHT - EDGE) / (float)vars->map_data->img_height);
		vars->map_data->raster_x = ((WIDTH - EDGE) / (float)vars->map_data->img_width);
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	ft_recalc_map(vars);
}

void	ft_recalc_map(t_vars *vars)
{
	int	x;
	int	y;
	int	delta_y;

	y = 0;
	delta_y = 0;
	vars->map_data->size_x_max = 0;
	vars->map_data->size_x_min = 0;
	vars->map_data->size_y_max = 0;
	vars->map_data->size_y_min = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			if (vars->map_data->size_z_min < 0)
				vars->map[y][x].z = vars->map[y][x].z + abs(vars->map_data->size_z_min);
			ft_calc_points(x, y, vars, 0);
			x++;
		}
		y++;
	}
	if (vars->map_data->size_y_min < 0)
		delta_y = -vars->map_data->size_y_min;
	y = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			ft_calc_points(x, y, vars, delta_y);
			x++;
		}
		y++;
	}
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min - delta_y;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	if (vars->map_data->img_height == 0)
		vars->map_data->img_height = EDGE;
	if (vars->map_data->img_width == 0)
		vars->map_data->img_width = EDGE;
	vars->map_data->screen_height = vars->map_data->img_height + EDGE;
	vars->map_data->screen_width = vars->map_data->img_width + EDGE;
}
