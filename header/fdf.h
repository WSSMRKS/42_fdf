/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:38:00 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/07 21:40:44 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "../libft/libft.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#define PI 3.14159265
#define EDGE 40
#define WIDTH 1920
#define HEIGHT 780
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

// mlx
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
int		mlx_key_handler(int keycode, t_vars *vars);

// parsing
int		ft_input_handler(int argc, char **argv, t_vars *vars);
void	ft_parse_input(t_vars *vars, int fd);
void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i);
int		ft_validate_args(t_vars *vars, char *str, int *valid);
void	ft_calc_points(int x, int y, t_vars *vars, int delta_y);
void	ft_size_map(t_vars *vars);
void	ft_recalc_map(t_vars *vars);

// drawing
void	ft_draw_map(t_vars *vars, t_data *img);
void	ft_bresenham(t_vars *vars, t_data *img);
void	ft_case_bres(t_point point1, t_point point2, t_data *img, t_vars *vars);
void	ft_put_bres_90(t_point point1, t_point point2, t_data *img);
void	ft_put_bres_45(t_point point1, t_point point2, t_data *img);
void	ft_hv_line(t_point act1, t_point act2, t_data *img);

// clean closing
int		mlx_close(t_vars *vars);
void	ft_lstdelone_fdf(t_line_lst *lst, void (*del)(void *));
void	ft_lstclear_fdf(t_line_lst **lst, void (*del)(void *));
int		ft_abort(t_vars *vars, int ymax);
void	ft_abort_err_msg(char *msg, t_vars *vars, int ymax);
void	ft_exit_err_msg(char *msg, t_vars *vars);

// init structs
void	ft_init_t_line_lst(t_line_lst *lines);
void	ft_init_t_map_data(t_map_data *map_data);
void	ft_init_vars(t_vars *vars);

