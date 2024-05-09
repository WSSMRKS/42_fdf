/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:00:36 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 00:43:12 by wssmrks          ###   ########.fr       */
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
		if (ft_strnstr(argv[1], ".fdf", ft_strlen(argv[1])) == NULL)
			ft_exit_err_msg("Error: Argument not a '.fdf' file.\n", vars);
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			ft_exit_err_msg("Error: No such file or directory.\n", vars);
		else
		{
			vars->img = NULL;
			vars->m_dat = ft_calloc(sizeof(t_m_dat), 1);
			if (!vars->m_dat)
				ft_abort_err_msg("fatal: malloc error.\n", vars, 0);
			ft_init_t_m_dat(vars->m_dat);
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

void	ft_assign_arg_map(t_vars *vars, t_line_lst *lines, int y, int x)
{
	vars->map[y] = ft_calloc(sizeof(t_point), vars->m_dat->map_width);
	if (!vars->map[y])
		ft_abort_err_msg("fatal: malloc error.\n", vars, y);
	x = 0;
	while (x < vars->m_dat->map_width)
	{
		if (!lines->split_line[x])
			break ;
		vars->map[y][x].z = Z_SCALE
			* ft_validate_args(lines->split_line[x], &vars->m_dat->valid);
		if (vars->map[y][x].z < vars->m_dat->z_min)
			vars->m_dat->z_min = vars->map[y][x].z;
		free(lines->split_line[x]);
		lines->split_line[x] = NULL;
		ft_calc_points(x, y, vars, 0);
		x++;
	}
	if (lines->split_line[x] || x < vars->m_dat->map_width)
	{
		while (lines->split_line[x])
			free(lines->split_line[x++]);
		ft_lstclear_fdf(&lines, free);
		ft_abort_err_msg("Error: Map error.\n", vars, y + 1);
	}
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
	vars->m_dat->map_width = x;
	while (lines && lines->split_line)
	{
		ft_assign_arg_map(vars, lines, y, x);
		free(lines->split_line);
		y++;
		tmp = lines;
		lines = lines->next;
		free(tmp);
	}
	free(lines);
	if (vars->m_dat->valid == 0)
		ft_abort_err_msg("Error: Map error.\n", vars, y);
	vars->m_dat->map_height = y;
}
