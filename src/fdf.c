/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:22:19 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/04 22:05:28 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/fdf.h"

int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;

	ft_init_vars(&vars);
	if (ft_input_handler(argc, argv, &vars) == -1)
	{
		ft_printf_err("Error: Invalid number of arguments.\n");
		exit(1);
		return (0);
	}
	ft_size_map(&vars);
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, vars.map_data->screen_width,
			vars.map_data->screen_height, "Fil de Fer");
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
