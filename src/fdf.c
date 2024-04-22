/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/22 15:02:49 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "../libft.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;
typedef struct s_vars {
	void	*mlx;
	void	*win;
	t_data	*img;
	char	**map;
}				t_vars;

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

int	ft_parse_input(char **map, int fd)
{
	char	*line;

	line = ft_get_next_line(fd);
	while (line)
	{
		ft_split(line, ' ');
		line = ft_get_next_line(fd);
	}
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
			ft_printf("ft_parse_input(%d)\n", fd);
			ft_parse_input(vars.map, fd);
		return (0);
	}
	return (0);
}



int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;

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
