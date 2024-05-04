/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_clean_closing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:03:46 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/04 22:05:11 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

int	ft_abort(t_vars *vars, int ymax)
{
	int			y;

	y = 0;
	if (vars->map)
	{
		while (y < ymax)
		{
			free(vars->map[y++]);
		}
		free(vars->map);
	}
	free(vars->map_data);
	exit(1);
	return (0);
}

int	mlx_close(t_vars *vars)
{
	int			y;

	y = 0;
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
	return (0);
}

void	ft_lstclear_fdf(t_line_lst **lst, void (*del)(void *))
{
	t_line_lst	*tlst;
	t_line_lst	*nlst;

	if (lst)
	{
		tlst = *lst;
		while (tlst)
		{
			nlst = tlst->next;
			ft_lstdelone_fdf(tlst, (del));
			tlst = nlst;
		}
		*lst = NULL;
	}
}

void	ft_lstdelone_fdf(t_line_lst *lst, void (*del)(void *))
{
	int	i;

	i = 0;
	if (lst)
	{
		if (del && lst->line)
			(del)(lst->line);
		if (del && lst->split_line)
		{
			if (lst->split_line[i])
			{
				while (lst->split_line[i] != NULL)
					(del)(lst->split_line[i++]);
			}
			(del)(lst->split_line);
		}
		free(lst);
		lst = NULL;
	}
}
