/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_old.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:52:45 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/21 15:35:06 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "../libft.h"

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
	void	*img;
}				t_vars;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	key_action(int keycode, t_vars vars)
{
	static int	old_key;

	ft_printf("%d\n", keycode);
	if (keycode == 65307 || (old_key == 65507 && keycode == 99))
	{
		ft_printf("running\n");
		if (vars.img)
			mlx_destroy_image(vars.mlx, &vars.img);
		if (vars.win)
			mlx_destroy_window(vars.mlx, &vars.win);
		if (vars.mlx)
			mlx_destroy_display(vars.mlx);
		ft_printf("Program terminated Gracefully ❤️");
		free(vars.mlx);
	}
	old_key = keycode;
	return (0);
}

int	close_mlx(int keycode, t_vars vars)
{
	// mlx_destroy_image(vars.mlx, &vars.img);
	mlx_destroy_window(vars.mlx, &vars.win);
	// mlx_destroy_display(vars.mlx);
	free(vars.mlx);
	exit(1);
	return (0);
}


int	main(void)
{
	t_vars	vars;
	t_data	img;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 960, 930, "Test1!");
	img.img = mlx_new_image(vars.mlx, 960, 930);
	img.addr = mlx_get_data_addr(&img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	my_mlx_pixel_put(vars.img, 460, 85, 0x00FF0000);
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	vars.img = &img;
	mlx_hook(vars.win, 02, 1L << 0, close_mlx, &vars);
	// mlx_hook(vars.win, 02, (1L << 0), key_action, (void *)&vars);
	mlx_loop(vars.mlx);
}

// int	main(void)
// {
// 	void	*mlx;
// 	t_data	img;

// 	mlx = mlx_init();
// 	img.img = mlx_new_image(mlx, 1920, 1080);

// 	/*
// 	** After creating an image, we can call `mlx_get_data_addr`, we pass
// 	** `bits_per_pixel`, `line_length`, and `endian` by reference. These will
// 	** then be set accordingly for the *current* data address.
// 	*/
// 	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
// 			&img.line_length, &img.endian);
// }