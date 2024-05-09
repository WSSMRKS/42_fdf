/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_parsing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:00:36 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 00:42:54 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

int	ft_validate_args(char *str, int *valid)
{
	int		i;
	long	nbr;

	nbr = 0;
	i = 0;
	if (!str || (str[i] == '-' && str[i + 1] == '\0'))
		*valid = 0;
	if (*valid != 0 && str[i] == '-')
		i++;
	while (*valid != 0 && str[i] != '\0' && ((str[i] >= '0' && str[i] <= '9')
			|| (str[i] == '\n' && str [i + 1] == '\0' && i != 0)))
		i++;
	if (str[i] != '\0')
		*valid = 0;
	if (valid != 0)
		nbr = ft_atol(str);
	if (valid != 0 && str[i] == '\0' && i <= 11 && nbr >= INT_MIN
		&& nbr <= INT_MAX)
		return ((int) nbr);
	return (0);
}

void	ft_calc_points(int x, int y, t_vars *vars, int delta_y)
{
	float	alpha;

	alpha = 30 * (PI / (float)180);
	vars->map[y][x].x = vars->m_dat->raster_x * (10 * x) * (float)cos(alpha)
		+ vars->m_dat->raster_x * (10 * y)
		* (float)cos(alpha + (PI * ((float)2 / 3)))
		+ vars->m_dat->raster_x * vars->map[y][x].z
		* (float)cos(alpha - (PI * ((float)2 / 3)));
	vars->map[y][x].y = delta_y + vars->m_dat->raster_y * (10 * x)
		* (float)sin(alpha)
		+ vars->m_dat->raster_y * (10 * y)
		* (float)sin(alpha + (PI * ((float)2 / 3)))
		+ vars->m_dat->raster_y * vars->map[y][x].z
		* (float)sin(alpha - (PI * ((float)2 / 3)));
	if (vars->map[y][x].x > vars->m_dat->x_max)
		vars->m_dat->x_max = vars->map[y][x].x;
	if (vars->map[y][x].x < vars->m_dat->x_min)
		vars->m_dat->x_min = vars->map[y][x].x;
	if (vars->map[y][x].y > vars->m_dat->y_max)
		vars->m_dat->y_max = vars->map[y][x].y;
	if (vars->map[y][x].y < vars->m_dat->y_min)
		vars->m_dat->y_min = vars->map[y][x].y;
}

void	ft_size_map(t_vars *vars)
{
	vars->m_dat->img_h
		= vars->m_dat->y_max - vars->m_dat->y_min;
	vars->m_dat->img_w
		= vars->m_dat->x_max - vars->m_dat->x_min;
	if (vars->m_dat->img_w > (WIDTH - EDGE)
		|| vars->m_dat->img_h > HEIGHT)
	{
		vars->m_dat->raster_y = (HEIGHT - EDGE) / (float)vars->m_dat->img_h;
		vars->m_dat->raster_x = (WIDTH - EDGE) / (float)vars->m_dat->img_w;
		if (vars->m_dat->raster_y > vars->m_dat->raster_x)
			vars->m_dat->raster_y = vars->m_dat->raster_x;
		else
			vars->m_dat->raster_x = vars->m_dat->raster_y;
	}
	else if (vars->m_dat->img_h > 0 && vars->m_dat->img_w > 0)
	{
		vars->m_dat->raster_y = ((HEIGHT - EDGE) / (float)vars->m_dat->img_h);
		vars->m_dat->raster_x = ((WIDTH - EDGE) / (float)vars->m_dat->img_w);
		if (vars->m_dat->raster_y > vars->m_dat->raster_x)
			vars->m_dat->raster_y = vars->m_dat->raster_x;
		else if (vars->m_dat->raster_y < vars->m_dat->raster_x)
			vars->m_dat->raster_x = vars->m_dat->raster_y;
	}
	ft_recalc_map(vars);
}

void	ft_recalc(t_vars *vars, int *y, int *x, int *delta_y)
{
	while (*y < vars->m_dat->map_height)
	{
		*x = 0;
		while (*x < vars->m_dat->map_width)
		{
			if (vars->m_dat->z_min < 0)
				vars->map[*y][*x].z = vars->map[*y][*x].z
					+ ft_abs(vars->m_dat->z_min);
			ft_calc_points(*x, *y, vars, 0);
			(*x)++;
		}
		(*y)++;
	}
	*y = 0;
	*delta_y = ft_abs(vars->m_dat->y_min);
	while (*y < vars->m_dat->map_height)
	{
		*x = 0;
		while (*x < vars->m_dat->map_width)
		{
			ft_calc_points(*x, *y, vars, *delta_y);
			(*x)++;
		}
		(*y)++;
	}
}

void	ft_recalc_map(t_vars *vars)
{
	int	x;
	int	y;
	int	delta_y;

	x = 0;
	y = 0;
	delta_y = 0;
	vars->m_dat->x_max = 0;
	vars->m_dat->x_min = 0;
	vars->m_dat->y_max = 0;
	vars->m_dat->y_min = 0;
	ft_recalc(vars, &y, &x, &delta_y);
	vars->m_dat->img_h
		= vars->m_dat->y_max - vars->m_dat->y_min - delta_y;
	vars->m_dat->img_w
		= vars->m_dat->x_max - vars->m_dat->x_min;
	if (vars->m_dat->img_h == 0)
		vars->m_dat->img_h = EDGE;
	if (vars->m_dat->img_w == 0)
		vars->m_dat->img_w = EDGE;
	vars->m_dat->screen_height = vars->m_dat->img_h + EDGE;
	vars->m_dat->screen_width = vars->m_dat->img_w + EDGE;
}
