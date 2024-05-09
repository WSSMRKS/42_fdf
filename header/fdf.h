/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wssmrks <wssmrks@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:38:00 by maweiss           #+#    #+#             */
/*   Updated: 2024/05/09 01:43:55 by wssmrks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "fdf_definitions.h"
# include "fdf_includes.h"

typedef enum e_bool {
	FALSE,
	TRUE
}				t_bool;

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

typedef struct s_m_dat {
	int		map_height;
	int		map_width;
	int		x_max;
	int		y_max;
	int		z_max;
	int		x_min;
	int		y_min;
	int		z_min;
	int		img_w;
	int		img_h;
	int		screen_height;
	int		screen_width;
	float	raster_x;
	float	raster_y;
	int		valid;
}				t_m_dat;

typedef struct s_vars {
	void		*mlx;
	void		*win;
	t_data		*img;
	t_point		**map;
	t_m_dat		*m_dat;
}				t_vars;

typedef struct s_line_lst {
	char		*line;
	char		**split_line;
	void		*next;
}				t_line_lst;

typedef struct s_bresenham {
	int	dx;
	int	dy;
	int	delta;
	int	x;
	int	y;
	int	yi;
	int	xi;
}				t_bresenham;

// mlx
void	ft_mlx_pixel_put(t_data *data, int x, int y, int color);
int		ft_mlx_key_handler(int keycode, t_vars *vars);
int		ft_mlx_close(t_vars *vars);

// parsing
int		ft_input_handler(int argc, char **argv, t_vars *vars);
void	ft_malloc_lst(void **lst, t_vars *vars, int ymax);
void	ft_parse_input(t_vars *vars, int fd);
void	ft_assign_arg_map(t_vars *vars, t_line_lst *lines, int y, int x);
void	ft_fill_map(t_vars *vars, t_line_lst *lines, int i);

// parsing2
int		ft_validate_args(char *str, int *valid);
void	ft_calc_points(int x, int y, t_vars *vars, int delta_y);
void	ft_size_map(t_vars *vars);
void	ft_recalc(t_vars *vars, int *y, int *x, int *delta_y);
void	ft_recalc_map(t_vars *vars);

// drawing
void	ft_draw_map(t_vars *vars, t_data *img);
void	ft_bresenham(t_vars *vars, t_data *img);
void	ft_case_bres(t_point point1, t_point point2, t_data *img, t_vars *vars);
void	ft_put_bres_90(t_point point1, t_point point2, t_data *img);
void	ft_put_bres_45(t_point point1, t_point point2, t_data *img);

// clean closing
void	ft_lstclear_fdf(t_line_lst **lst, void (*del)(void *));
void	ft_lstdelone_fdf(t_line_lst *lst, void (*del)(void *));
void	ft_exit_err_msg(char *msg, t_vars *vars);
void	ft_abort_err_msg(char *msg, t_vars *vars, int ymax);

// init structs
void	ft_init_t_line_lst(t_line_lst *lines);
void	ft_init_t_m_dat(t_m_dat *m_dat);
void	ft_init_vars(t_vars *vars);
void	ft_init_bres(t_bresenham *bres);
#endif