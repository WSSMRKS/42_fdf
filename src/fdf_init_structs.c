/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_init_structs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:05:31 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 00:42:25 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

void	ft_init_t_line_lst(t_line_lst *lines)
{
	lines->line = NULL;
	lines->split_line = NULL;
	lines->next = NULL;
}

void	ft_init_t_m_dat(t_m_dat *m_dat)
{
	m_dat->x_max = 0;
	m_dat->x_min = 0;
	m_dat->y_max = 0;
	m_dat->y_min = 0;
	m_dat->z_max = 0;
	m_dat->z_min = 0;
	m_dat->valid = 1;
	m_dat->raster_x = 1;
	m_dat->raster_y = 1;
	m_dat->map_height = 1;
	m_dat->map_width = 1;
	m_dat->img_w = 100;
	m_dat->img_h = 100;
	m_dat->screen_height = 100;
	m_dat->screen_width = 100;
}

void	ft_init_vars(t_vars *vars)
{
	vars->img = NULL;
	vars->map = NULL;
	vars->m_dat = NULL;
	vars->mlx = NULL;
	vars->win = NULL;
}

void	ft_init_bres(t_bresenham *bres)
{
	bres->x = 0;
	bres->y = 0;
	bres->delta = 0;
	bres->dx = 0;
	bres->dy = 0;
	bres->yi = 1;
	bres->xi = 1;
}
