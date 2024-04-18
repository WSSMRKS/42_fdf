/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:52:45 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/18 13:33:17 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "../libft.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_data	img;

	mlx = mlx_init();
	ft_printf("%d\n", mlx);
	mlx_win = mlx_new_window(mlx, 960, 930, "Test1!");
	ft_printf("%d\n", mlx_win);
	img.img = mlx_new_image(mlx, 960, 930);
	ft_printf("%d\n", img.img);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	ft_printf("%c\n", img.addr);
	my_mlx_pixel_put(&img, 460, 85, 0x00FF0000);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
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
