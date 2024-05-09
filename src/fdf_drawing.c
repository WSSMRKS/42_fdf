/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_drawing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:55:20 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 00:49:31 by wssmrks          ###   ########.fr       */
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
			ft_mlx_pixel_put(img, x_actual, y_actual, 0x00FFFFFF);
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
	t_bresenham	bres;

	ft_init_bres(&bres);
	bres.dx = point2.x - point1.x;
	bres.dy = point2.y - point1.y;
	if (bres.dy < 0)
	{
		bres.yi = -1;
		bres.dy = -bres.dy;
	}
	bres.delta = (2 * bres.dy) - bres.dx;
	bres.x = point1.x;
	bres.y = point1.y;
	while (bres.x < point2.x)
	{
		if (bres.delta > 0)
		{
			bres.y = bres.y + bres.yi;
			bres.delta = bres.delta + (2 * (bres.dy - bres.dx));
		}
		else
			bres.delta = bres.delta + 2 * bres.dy;
		ft_mlx_pixel_put(img, bres.x, bres.y, 0x00FFFFFF);
		bres.x++;
	}
}

void	ft_put_bres_90(t_point point1, t_point point2, t_data *img)
{
	t_bresenham	bres;

	ft_init_bres(&bres);
	bres.dx = point2.x - point1.x;
	bres.dy = point2.y - point1.y;
	if (bres.dx < 0)
	{
		bres.xi = -1;
		bres.dx = -bres.dx;
	}
	bres.delta = (2 * bres.dx) - bres.dy;
	bres.x = point1.x;
	bres.y = point1.y;
	while (bres.y < point2.y)
	{
		if (bres.delta > 0)
		{
			bres.x = bres.x + bres.xi;
			bres.delta = bres.delta + (2 * (bres.dx - bres.dy));
		}
		else
			bres.delta = bres.delta + 2 * bres.dx;
		ft_mlx_pixel_put(img, bres.x, bres.y, 0x00FFFFFF);
		bres.y++;
	}
}
