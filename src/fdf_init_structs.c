/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_init_structs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:05:31 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/04 22:05:59 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

void	ft_init_t_line_lst(t_line_lst *lines)
{
	lines->line = NULL;
	lines->split_line = NULL;
	lines->next = NULL;
}

void	ft_init_t_map_data(t_map_data *map_data)
{
	map_data->size_x_max = 0;
	map_data->size_x_min = 0;
	map_data->size_y_max = 0;
	map_data->size_y_min = 0;
	map_data->size_z_max = 0;
	map_data->size_z_min = 0;
	map_data->valid = 1;
	map_data->raster_x = 1;
	map_data->raster_y = 1;
	map_data->map_height = 1;
	map_data->map_width = 1;
	map_data->img_width = 100;
	map_data->img_height = 100;
	map_data->screen_height = 100;
	map_data->screen_width = 100;
}

void	ft_init_vars(t_vars *vars)
{
	vars->img = NULL;
	vars->map = NULL;
	vars->map_data = NULL;
	vars->mlx = NULL;
	vars->win = NULL;
}
