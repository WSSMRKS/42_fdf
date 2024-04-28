/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/28 15:35:27 by maweiss          ###   ########.fr       */
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

typedef struct s_map_data {
	int		map_height;
	int		map_width;
	int		size_x_max;
	int		size_y_max;
	int		size_x_min;
	int		size_y_min;
	int		img_width;
	int		img_height;
	int		screen_height;
	int		screen_width;
	float	raster_x;
	float	raster_y;
}				t_map_data;

typedef struct s_vars {
	void		*mlx;
	void		*win;
	t_data		*img;
	t_point		**map;
	t_map_data	*map_data;
}				t_vars;

typedef struct s_line_lst {
	char		*line;
	char		**split_line;
	void		*next;
}				t_line_lst;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	ft_draw_map(t_vars *vars, t_data *img);
void	ft_size_map(t_vars *vars);
int		ft_input_handler(int argc, char **argv, t_vars *vars);
void	ft_parse_input(t_vars *vars, int fd);
void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i);
void	ft_calc_points(int x, int y, t_vars *vars);
int		mlx_key_handler(int keycode, t_vars *vars);
int		mlx_close(t_vars *vars);

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

void	ft_calc_points(int x, int y, t_vars *vars)
{
	float	alpha;

	alpha = 30 * (PI / (float)180);
	vars->map[y][x].x = vars->map_data->raster_x * (10 * x) * (float)cos(alpha)
		+ vars->map_data->raster_x * (10 * y) * (float)cos(alpha + 2)
		+ vars->map_data->raster_x * vars->map[y][x].z * cos(alpha - 2);
	vars->map[y][x].y = vars->map_data->raster_y * (10 * x) * (float)sin(alpha)
		+ vars->map_data->raster_y * (10 * y) * (float)sin(alpha + 2)
		+ vars->map_data->raster_y * vars->map[y][x].z * sin(alpha - 2);
	if (vars->map[y][x].x > vars->map_data->size_x_max)
		vars->map_data->size_x_max = vars->map[y][x].x;
	if (vars->map[y][x].x < vars->map_data->size_x_min)
		vars->map_data->size_x_min = vars->map[y][x].x;
	if (vars->map[y][x].y > vars->map_data->size_y_max)
		vars->map_data->size_y_max = vars->map[y][x].y;
	if (vars->map[y][x].y < vars->map_data->size_y_min)
		vars->map_data->size_y_min = vars->map[y][x].y;
}

void ft_fill_map(t_vars *vars, t_line_lst *lines, int i)
{
	int			len_x;
	int			y;
	int			x;
	t_line_lst	*tmp;
	t_point		**map;

	vars->map = malloc(sizeof(t_point *) * (i + 1));
	len_x = 0;
	y = 0;
	map = vars->map;
	while (lines->split_line[len_x])
		len_x++;
	vars->map_data->map_width = len_x;
	while (lines->split_line)
	{
		map[y] = malloc(sizeof(t_point) * (len_x));
		x = 0;
		while (x < len_x)
		{
			map[y][x].z = ft_atoi(lines->split_line[x]);
			free(lines->split_line[x]);
			ft_calc_points(x, y, vars);
			x++;
		}
		free(lines->split_line);
		y++;
		tmp = lines;
		lines = lines->next;
		free(tmp);
	}
	vars->map_data->map_height = y;
}

void	ft_parse_input(t_vars *vars, int fd)
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

int	ft_input_handler(int argc, char **argv, t_vars *vars)
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

void ft_recalc_map(t_vars *vars)
{
	int	x;
	int	y;

	y = 0;
	vars->map_data->size_x_max = 0;
	vars->map_data->size_x_min = 0;
	vars->map_data->size_y_max = 0;
	vars->map_data->size_y_min = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			ft_calc_points(x, y, vars);
			x++;
		}
		y++;
	}
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	vars->map_data->screen_height = vars->map_data->img_height + 40;
	vars->map_data->screen_width = vars->map_data->img_width + 40;
}

void ft_size_map(t_vars *vars)
{
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	if (vars->map_data->img_width > 1240 || vars->map_data->img_height > 680)
	{
		vars->map_data->raster_y = 680 / (float)vars->map_data->img_height;
		vars->map_data->raster_x = 1240 / (float)vars->map_data->img_width;
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	else
	{
		vars->map_data->raster_y = (float)vars->map_data->img_height * (680 / (float)vars->map_data->img_height);
		vars->map_data->raster_x = (float)vars->map_data->img_width *(1240 / (float)vars->map_data->img_width);
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	ft_recalc_map(vars);
}

// in this function run through map and put to screen.

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

	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			x_actual = vars->map[y][x].x + -vars->map_data->size_x_min + 20;
			y_actual = vars->map[y][x].y + 20;
			my_mlx_pixel_put(img, x_actual, y_actual, 0x00FF0000);
			x++;
		}
		y++;
	}
}


int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;

	vars.img = NULL;
	vars.map_data = malloc(sizeof(t_map_data));
	vars.map_data->size_x_max = 0;
	vars.map_data->size_x_min = 0;
	vars.map_data->size_y_max = 0;
	vars.map_data->size_y_min = 0;
	vars.map_data->raster_x = 1;
	vars.map_data->raster_y = 1;
	if (ft_input_handler(argc, argv, &vars) == -1)
	{
		ft_printf_err("Error: Invalid number of arguments.\n");
		exit(1);
		return (0);
	}
	ft_size_map(&vars);
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, vars.map_data->screen_width,
			vars.map_data->screen_height, "Hello world!");
	img.img = mlx_new_image(vars.mlx, vars.map_data->screen_width,
			vars.map_data->screen_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	ft_draw_map(&vars, &img);
	vars.img = &img;
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_hook(vars.win, 2, 1L << 0, mlx_key_handler, &vars);
	mlx_hook(vars.win, 17, 1L << 17, mlx_close, &vars);
	mlx_loop(vars.mlx);

}
/*
Todo:
- add input check;
- add lines;
- maybe zoom if small;
- add Header files and enum;
- valgrind;
- error Handling;
- Norminette;
- Makefile;
- check against subject;
*/
