/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/23 16:40:40 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "../libft.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#define PI 3.14159265

typedef struct s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_point {
	int	x;
	int	y;
	int	z;
}				t_point;

typedef struct s_vars {
	void	*mlx;
	void	*win;
	t_data	*img;
	t_point	***map;
}				t_vars;

typedef struct s_line_lst {
	char		*line;
	char		**split_line;
	void		*next;
}				t_line_lst;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	mlx_close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_image(vars->mlx, vars->img->img);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	exit(1);
	return (0);
}

int	mlx_key_handler(int keycode, t_vars *vars)
{
	static int	old_key;

	ft_printf("%d\n", keycode);
	if (keycode == 65307 || (old_key == 65507 && keycode == 99))
	{
		mlx_destroy_window(vars->mlx, vars->win);
		mlx_destroy_image(vars->mlx, vars->img->img);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
		exit(1);
	}
	old_key = keycode;
	return (0);
}

void	init_errors(void)
{
	strerror(1);
}

void	ft_calc_points(t_point *point, int x, int y)
{
	int	alpha;

	alpha = 30 * (PI / 180);
	point->x = x * (int)cos(alpha) + y * cos(alpha + 2) + point->z
		* cos(alpha - 2);
	point->y = x * (int)sin(alpha) + y * sin(alpha + 2) + point->z
		* sin(alpha - 2);
}

void ft_fill_map(t_vars vars, t_line_lst *lines, int i)
{
	int			len_x;
	int			y;
	int			x;
	t_line_lst	*tmp;
	t_point		***map;

	map = vars.map;
	len_x = 0;
	y = 0;
	map = malloc(sizeof(t_point **) * (i + 1));
	while (lines->split_line[len_x])
		len_x++;
	while (lines)
	{
		map[y] = malloc(sizeof(t_point *) * (len_x));
		x = 0;
		while (x < len_x)
		{
			map[y][x] = malloc(sizeof(t_point));
			map[y][x]->z = ft_atoi(lines->split_line[x]);
			free(lines->split_line[x]);
			ft_calc_points(map[y][x], x, y);
			x++;
		}
		map [y][x] = NULL;
		free(lines->split_line);
		y++;
		tmp = lines;
		lines = lines->next;
		free(tmp);
	}
}

void	ft_parse_input(t_vars vars, int fd)
{
	char		*line;
	t_line_lst	*lines;
	t_line_lst	*lines_head;
	int			i;

	i = 0;
	line = ft_get_next_line(fd);
	lines = malloc(sizeof(t_line_lst));
	lines_head = lines;
	while (line)
	{
		lines->split_line = ft_split(line, ' ');
		lines->next = malloc(sizeof(t_line_lst));
		lines = lines->next;
		i++;
		line = ft_get_next_line(fd);
	}
	ft_fill_map(vars, lines_head, i);
}

int	ft_input_handler(int argc, char **argv, t_vars vars)
{
	int	fd;
	if (argc != 2)
		return (-1);
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			perror("Error: No such file or directory");
			exit(1);
		}
		else
			ft_parse_input(vars, fd);
		close(fd);
		return (0);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;

	vars.img = NULL;

	if (ft_input_handler(argc, argv, vars) == -1)
	{
		ft_printf_err("Error: Invalid number of arguments.\n");
		exit(1);
		return (0);
	}
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1280, 570, "Hello world!");
	img.img = mlx_new_image(vars.mlx, 1280, 570);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	my_mlx_pixel_put(&img, 460, 85, 0x00FF0000);
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	vars.img = &img;
	mlx_hook(vars.win, 2, 1L << 0, mlx_key_handler, &vars);
	mlx_hook(vars.win, 17, 1L << 17, mlx_close, &vars);
	mlx_loop(vars.mlx);

}
