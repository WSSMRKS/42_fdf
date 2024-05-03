/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/03 14:02:53 by maweiss          ###   ########.fr       */
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
#define EDGE 40
#define WIDTH 1920
#define HEIGHT 900
#define Z_SCALE 1

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
	int		size_z_max;
	int		size_x_min;
	int		size_y_min;
	int		size_z_min;
	int		img_width;
	int		img_height;
	int		screen_height;
	int		screen_width;
	float	raster_x;
	float	raster_y;
	int		valid;
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

void	ft_draw_map(t_vars *vars, t_data *img);
void	ft_bresenham(t_vars *vars, t_data *img);
void	ft_case_bres(t_point point1, t_point point2, t_data *img, t_vars *vars);
void	ft_hv_line(t_point act1, t_point act2, t_data *img);
void	ft_put_bres_90(t_point point1, t_point point2, t_data *img);
void	ft_put_bres_45(t_point point1, t_point point2, t_data *img);
void	ft_size_map(t_vars *vars);
void	ft_recalc_map(t_vars *vars);
int		ft_input_handler(int argc, char **argv, t_vars *vars);
void	ft_parse_input(t_vars *vars, int fd);
void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i);
void	ft_calc_points(int x, int y, t_vars *vars, int delta_y);
int		mlx_key_handler(int keycode, t_vars *vars);
int		ft_validate_args(t_vars *vars, char *str, int *valid);
int		mlx_close(t_vars *vars);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
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
int		ft_validate_args(t_vars *vars, char *str, int *valid)
{
	int		i;
	long	nbr;

	nbr = 0;
	i = 0;
	if (!str || (str[i] == '-' && str[i + 1] == '\0'))
		*valid = 0;
	if (*valid != 0 && str[i] == '-')
		i++;
	while (*valid != 0 && str[i] != '\0')
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '\n')
			i++;
		else
			*valid = 0;
	}
	if (valid != 0)
		nbr = ft_atol(str);
	if (valid != 0 && str[i] == '\0' && i <= 11 && nbr >= -2147483648
		&& nbr <= 2147483647)
	{
		if (nbr < vars->map_data->size_z_min)
			vars->map_data->size_z_min = nbr;
		return ((int) nbr);
	}
	else
		return (*valid = 0);
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

void	ft_calc_points(int x, int y, t_vars *vars, int delta_y)
{
	float	alpha;

	alpha = 30 * (PI / (float)180);
	vars->map[y][x].x = vars->map_data->raster_x * (10 * x) * (float)cos(alpha)
		+ vars->map_data->raster_x * (10 * y) * (float)cos(alpha + 2)
		+ vars->map_data->raster_x * vars->map[y][x].z * cos(alpha - 2);
	vars->map[y][x].y = delta_y + vars->map_data->raster_y * (10 * x)
		* (float)sin(alpha)
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

void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i)
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
	// ft_validate_args(vars, lines->split_line[len_x++], &vars->map_data->valid);
	// [ ] add procedure for invalid map;
	vars->map_data->map_width = len_x;
	while (lines && lines->split_line)
	{
		map[y] = malloc(sizeof(t_point) * (len_x));
		x = 0;
		while (x < len_x)
		{
			map[y][x].z = Z_SCALE * ft_validate_args(vars, lines->split_line[x], &vars->map_data->valid);
			// map[y][x].z = Z_SCALE * ft_atoi(lines->split_line[x]);
			// [ ] maybe change atoi vs validate args
			free(lines->split_line[x]);
			ft_calc_points(x, y, vars, 0);
			x++;
		}
		free(lines->split_line);
		y++;
		tmp = lines;
		lines = lines->next;
		free(tmp);
	}
	free(lines);
	vars->map_data->map_height = y;
}

void ft_init_t_line_lst(t_line_lst *lines)
{
	lines->line = NULL;
	lines->split_line = NULL;
	lines->next = NULL;
}

void	ft_parse_input(t_vars *vars, int fd)
{
	char		*line;
	t_line_lst	*lines;
	t_line_lst	*lines_head;
	int			i;
	char		*free_line;

	i = 0;
	line = ft_get_next_line(fd);
	lines = malloc(sizeof(t_line_lst));
	ft_init_t_line_lst(lines);
	lines_head = lines;
	while (line)
	{
		free_line = line;
		lines->split_line = ft_split(line, ' ');
		free(free_line);
		lines->next = malloc(sizeof(t_line_lst));
		lines = lines->next;
		ft_init_t_line_lst(lines);
		i++;
		line = ft_get_next_line(fd);
	}
	ft_fill_map(vars, lines_head, i);
}

int		ft_input_handler(int argc, char **argv, t_vars *vars)
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

void	ft_recalc_map(t_vars *vars)
{
	int	x;
	int	y;
	int	delta_y;

	y = 0;
	delta_y = 0;
	vars->map_data->size_x_max = 0;
	vars->map_data->size_x_min = 0;
	vars->map_data->size_y_max = 0;
	vars->map_data->size_y_min = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			if (vars->map_data->size_z_min < 0)
				vars->map[y][x].z = vars->map[y][x].z + abs(vars->map_data->size_z_min);
			ft_calc_points(x, y, vars, 0);
			x++;
		}
		y++;
	}
	if (vars->map_data->size_y_min < 0)
		delta_y = -vars->map_data->size_y_min;
	y = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			ft_calc_points(x, y, vars, delta_y);
			x++;
		}
		y++;
	}
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min - delta_y;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	vars->map_data->screen_height = vars->map_data->img_height + EDGE;
	vars->map_data->screen_width = vars->map_data->img_width + EDGE;
}

void	ft_size_map(t_vars *vars)
{
	vars->map_data->img_height
		= vars->map_data->size_y_max - vars->map_data->size_y_min;
	vars->map_data->img_width
		= vars->map_data->size_x_max - vars->map_data->size_x_min;
	if (vars->map_data->img_width > (WIDTH - EDGE) || vars->map_data->img_height > HEIGHT)
	{
		vars->map_data->raster_y = (HEIGHT - EDGE) / (float)vars->map_data->img_height;
		vars->map_data->raster_x = (WIDTH - EDGE) / (float)vars->map_data->img_width;
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	else
	{
		vars->map_data->raster_y = ((HEIGHT - EDGE) / (float)vars->map_data->img_height);
		vars->map_data->raster_x = ((WIDTH - EDGE) / (float)vars->map_data->img_width);
		if (vars->map_data->raster_y > vars->map_data->raster_x)
			vars->map_data->raster_y = vars->map_data->raster_x;
		else
			vars->map_data->raster_x = vars->map_data->raster_y;
	}
	ft_recalc_map(vars);
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
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
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
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
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
			my_mlx_pixel_put(img, x, y++, 0x00FF0000);
	}
	else if (act1.x == act2.x)
	{
		x = act1.x;
		y = act2.y;
		while (y < act1.y)
			my_mlx_pixel_put(img, x, y++, 0x00FF0000);
	}
	else if (act1.y == act2.y && act1.x < act2.x)
	{
		x = act1.x;
		y = act1.y;
		while (x < act2.x)
			my_mlx_pixel_put(img, x++, y, 0x00FF0000);
	}
	else if (act1.y == act2.y)
	{
		x = act2.x;
		y = act1.y;
		while (x < act1.x)
			my_mlx_pixel_put(img, x++, y, 0x00FF0000);
	}
}

void	ft_case_bres(t_point point1, t_point point2, t_data *img, t_vars *vars)
{
	t_point	act1;
	t_point	act2;

	act1.x = point1.x + -vars->map_data->size_x_min + (EDGE / 2);
	act1.y = point1.y + (EDGE / 2);
	act2.x = point2.x + -vars->map_data->size_x_min + (EDGE / 2);
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


void	ft_bresenham(t_vars *vars, t_data *img)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (y < vars->map_data->map_height)
	{
		x = 0;
		while (x < vars->map_data->map_width)
		{
			if (x + 1 < vars->map_data->map_width)
				ft_case_bres(vars->map[y][x], vars->map[y][x + 1], img, vars);
			if (y + 1 < vars->map_data->map_height)
				ft_case_bres(vars->map[y][x], vars->map[y + 1][x], img, vars);
			x++;
		}
		y++;
	}
}


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
			x_actual = vars->map[y][x].x + -vars->map_data->size_x_min + (EDGE / 2);
			y_actual = vars->map[y][x].y + (EDGE / 2);
			my_mlx_pixel_put(img, x_actual, y_actual, 0x00FF0000);
			x++;
		}
		y++;
	}
	ft_bresenham(vars, img);
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
	vars.map_data->size_z_max = 0;
	vars.map_data->size_z_min = 0;
	vars.map_data->valid = 1;
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
- add lines; [x]
- maybe zoom if small; [x]
- add Header files and enum;
- valgrind; [ ]
- error Handling; [ ]
- Norminette; [ ]
- Makefile; [ ]
- check against subject; [ ]
*/
