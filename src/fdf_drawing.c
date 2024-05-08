/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_drawing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:55:20 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/07 23:04:30 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

void	ft_draw_map(t_vars *vars, t_data *img)
{
	int	x;
	int	y;
	int	x_actual;
	int	y_actual;

	x_actual = 0;
	y_actual = 0;
	y = 0;
	x = 0;

	while (y < vars->m_dat->map_height)
	{
		x = 0;
		while (x < vars->m_dat->map_width)
		{
			x_actual = vars->map[y][x].x + -vars->m_dat->x_min + (EDGE / 2);
			y_actual = vars->map[y][x].y + (EDGE / 2);
			my_mlx_pixel_put(img, x_actual, y_actual, 0x00FFFFFF);
			x++;
		}
		y++;
	}
	ft_bresenham(vars, img);
}

void	ft_bresenham(t_vars *vars, t_data *img)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (y < vars->m_dat->map_height)
	{
		x = 0;
		while (x < vars->m_dat->map_width)
		{
			if (x + 1 < vars->m_dat->map_width)
				ft_case_bres(vars->map[y][x], vars->map[y][x + 1], img, vars);
			if (y + 1 < vars->m_dat->map_height)
				ft_case_bres(vars->map[y][x], vars->map[y + 1][x], img, vars);
			x++;
		}
		y++;
	}
}

void	ft_case_bres(t_point point1, t_point point2, t_data *img, t_vars *vars)
{
	t_point	act1;
	t_point	act2;

	act1.x = point1.x + -vars->m_dat->x_min + (EDGE / 2);
	act1.y = point1.y + (EDGE / 2);
	act2.x = point2.x + -vars->m_dat->x_min + (EDGE / 2);
	act2.y = point2.y + (EDGE / 2);

	// if (act1.x == act2.x || act1.y == act2.y)
	// 	ft_hv_line(act1, act2, img);
	if (ft_abs(act2.y - act1.y) < ft_abs(act2.x - act1.x))
	{
		if (act1.x > act2.x)
			ft_put_bres_45(act2, act1, img);
		else
			ft_put_bres_45(act1, act2, img);
	}
	else
	{
		if (act1.y > act2.y)
			ft_put_bres_90(act2, act1, img);
		else
			ft_put_bres_90(act1, act2, img);
	}
}

void	ft_put_bres_45(t_point point1, t_point point2, t_data *img)
{
	int	dx;
	int	dy;
	int	delta;
	int	x;
	int	y;
	int yi;

	x = 0;
	y = 0;
	delta = 0;
	dx = 0;
	dy = 0;
	yi = 1;

	dx = point2.x - point1.x;
	dy = point2.y - point1.y;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	delta = (2 * dy) - dx;
	x = point1.x;
	y = point1.y;
	while (x < point2.x)
	{
		if (delta > 0)
		{
			y = y + yi;
			delta = delta + (2 * (dy - dx));
		}
		else
			delta = delta + 2 * dy;
		my_mlx_pixel_put(img, x, y, 0x00FFFFFF);
		x++;
	}
}

void	ft_put_bres_90(t_point point1, t_point point2, t_data *img)
{
	int	dx;
	int	dy;
	int	delta;
	int	x;
	int	y;
	int xi;

	x = 0;
	y = 0;
	delta = 0;
	dx = 0;
	dy = 0;
	xi = 1;

	dx = point2.x - point1.x;
	dy = point2.y - point1.y;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	delta = (2 * dx) - dy;
	x = point1.x;
	y = point1.y;
	while (y < point2.y)
	{
		if (delta > 0)
		{
			x = x + xi;
			delta = delta + (2 * (dx - dy));
		}
		else
			delta = delta + 2 * dx;
		my_mlx_pixel_put(img, x, y, 0x00FFFFFF);
		y++;
	}
}

void	ft_hv_line(t_point act1, t_point act2, t_data *img)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	if (act1.x == act2.x && act1.y < act2.y)
	{
		y = act1.y;
		x = act1.x;
		while (y < act2.y)
			my_mlx_pixel_put(img, x, y++, 0x00FFFFFF);
	}
	else if (act1.x == act2.x)
	{
		x = act1.x;
		y = act2.y;
		while (y < act1.y)
			my_mlx_pixel_put(img, x, y++, 0x00FFFFFF);
	}
	else if (act1.y == act2.y && act1.x < act2.x)
	{
		x = act1.x;
		y = act1.y;
		while (x < act2.x)
			my_mlx_pixel_put(img, x++, y, 0x00FFFFFF);
	}
	else if (act1.y == act2.y)
	{
		x = act2.x;
		y = act1.y;
		while (x < act1.x)
			my_mlx_pixel_put(img, x++, y, 0x00FFFFFF);
	}
}
