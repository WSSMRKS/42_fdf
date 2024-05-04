/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:59:14 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/04 22:00:15 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	mlx_key_handler(int keycode, t_vars *vars)
{
	static int	old_key;
	int			y;

	y = 0;
	if (keycode == 65307 || (old_key == 65507 && keycode == 99))
	{
		mlx_destroy_window(vars->mlx, vars->win);
		mlx_destroy_image(vars->mlx, vars->img->img);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
		while (y < vars->map_data->map_height)
		{
			free(vars->map[y++]);
		}
		free(vars->map);
		free(vars->map_data);
		exit(1);
	}
	old_key = keycode;
	return (0);
}
