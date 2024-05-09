/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:59:14 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 01:26:57 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

void	ft_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	ft_mlx_key_handler(int keycode, t_vars *vars)
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
		while (y < vars->m_dat->map_height)
		{
			free(vars->map[y++]);
		}
		free(vars->map);
		free(vars->m_dat);
		exit(EXIT_SUCCESS);
	}
	old_key = keycode;
	return (0);
}

int	ft_mlx_close(t_vars *vars)
{
	int			y;

	y = 0;
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_image(vars->mlx, vars->img->img);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	while (y < vars->m_dat->map_height)
	{
		free(vars->map[y++]);
	}
	free(vars->map);
	free(vars->m_dat);
	exit(EXIT_FAILURE);
	return (0);
}
