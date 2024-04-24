/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/24 18:29:03 by maweiss          ###   ########.fr       */
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
	int	size_x_max;
	int	size_y_max;
	int	size_x_min;
	int	size_y_min;
}				t_map_data;

typedef struct s_vars {
	void		*mlx;
	void		*win;
	t_data		*img;
	t_point		**map;
	t_map_data	map_data;
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

void	ft_calc_points(t_point point, int x, int y)
{
	float	alpha;

	alpha = 30 * (PI / (float)180);
	point.x = 10 * x * (float)cos(alpha) + 10 * y * (float)cos(alpha + 2)
		+ (float)point.z * cos(alpha - 2);
	point.y = 10 * x * sin(alpha) + 10 * y * sin(alpha + 2)
		+ point.z * sin(alpha - 2);
	// if (point.x > vars.map_data.size_x_max)
	// 	vars.map_data.size_x_max = point.x;
	// if (point.x < vars.map_data.size_x_min)
	// 	vars.map_data.size_x_min = point.x;
	// if (point.y > vars.map_data.size_y_max)
	// 	vars.map_data.size_y_max = point.y;
	// if (point.y < vars.map_data.size_y_min)
	// 	vars.map_data.size_y_min = point.y;
}

void ft_fill_map(t_vars vars, t_line_lst *lines, int i)
{
	int			len_x;
	int			y;
	int			x;
	t_line_lst	*tmp;
	t_point		**map;

	map = vars.map;
	len_x = 0;
	y = 0;
	map = malloc(sizeof(t_point *) * (i + 1));
	while (lines->split_line[len_x])
		len_x++;
	while (lines->split_line)
	{
		map[y] = malloc(sizeof(t_point ) * (len_x));
		x = 0;
		while (x < len_x)
		{
			map[y][x].z = ft_atoi(lines->split_line[x]);
			free(lines->split_line[x]);
			ft_calc_points(map[y][x], x, y);
			x++;
		}
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

	void ft_draw_map(t_vars vars)
	{
		printf("size_x_max: %d\n",vars.map_data.size_x_max);
		printf("size_x_min: %d\n",vars.map_data.size_x_min);
		printf("size_y_max: %d\n",vars.map_data.size_y_max);
		printf("size_y_min: %d\n",vars.map_data.size_y_min);
	}

int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;

	vars.img = NULL;
	vars.map_data.size_x_max = 0;
	vars.map_data.size_x_min = 0;
	vars.map_data.size_y_max = 0;
	vars.map_data.size_y_min = 0;
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
	vars.img = &img;
	ft_draw_map(vars);
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_hook(vars.win, 2, 1L << 0, mlx_key_handler, &vars);
	mlx_hook(vars.win, 17, 1L << 17, mlx_close, &vars);
	mlx_loop(vars.mlx);

}
