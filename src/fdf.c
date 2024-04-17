/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:52:45 by maweiss           #+#    #+#             */
/*   Updated: 2024/04/17 16:57:16 by maweiss          ###   ########.fr       */
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
	img.img = mlx_new_image(mlx, 960, 1080);
	ft_printf("%d\n", img.img);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	ft_printf("%c\n", img.addr);
	my_mlx_pixel_put(&img, 460, 1, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 2, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 3, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 4, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 5, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 6, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 7, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 8, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 9, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 10, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 11, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 12, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 13, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 14, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 15, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 16, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 17, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 18, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 19, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 20, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 21, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 22, 0x00FF0000);
	my_mlx_pixel_put(&img, 230, 23, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 24, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 25, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 26, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 27, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 28, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 29, 0x00FF0000);
	my_mlx_pixel_put(&img, 690, 30, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 31, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 32, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 33, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 34, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 35, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 36, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 37, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 38, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 39, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 40, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 41, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 42, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 43, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 44, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 45, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 46, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 47, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 48, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 49, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 50, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 51, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 52, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 53, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 54, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 55, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 56, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 57, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 58, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 59, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 60, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 61, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 62, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 63, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 64, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 65, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 66, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 67, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 68, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 69, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 70, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 71, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 72, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 73, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 74, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 75, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 76, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 77, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 78, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 79, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 80, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 81, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 82, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 83, 0x00FF0000);
	my_mlx_pixel_put(&img, 460, 84, 0x00FF0000);
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
