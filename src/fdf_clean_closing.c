/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_clean_closing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:03:46 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/07 22:26:51 by wssmrks          ###   ########.fr       */
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
			free(vars->map[y++]);
		free(vars->map);
	}
	if (vars->m_dat)
		free(vars->m_dat);
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
	while (y < vars->m_dat->map_height)
	{
		free(vars->map[y++]);
	}
	free(vars->map);
	free(vars->m_dat);
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

void	ft_exit_err_msg(char *msg, t_vars *vars)
{
	ft_printf_err(msg);
	if (vars->m_dat)
		free(vars->m_dat);
	exit(EXIT_FAILURE);
}

void	ft_abort_err_msg(char *msg, t_vars *vars, int ymax)
{
	ft_printf_err(msg);
	ft_abort(vars, ymax);
}
