#include "cub3d.h"
#include <stdio.h>

/** print err msg & exit(1) !! */
void	err_exit(const char *str, t_game *g, int err)
{
	if(str)
		printf("Error\n%s\n", str);
	if(err & E_INIT)
		free_double_ptr((void **)g->mlx->tmp);
		// free g->mlx->texture
	if(err & E_PARSE)
		free_cub(g->cub);
	// 이후의 에러들과 그에 따른 free 추가
	exit(1);
}

int	close_win(t_mlx *mlx)
{
	mlx_clear_window(mlx->mlx_ptr, mlx->mlx_win);
	mlx_destroy_window(mlx->mlx_ptr, mlx->mlx_win);
	printf("See you later!\n");
	exit(0);
}

int	start_cub3d(t_game *g)
{
	g->mlx->mlx_ptr = mlx_init();
	if (!g->mlx->mlx_ptr)
		return (FAIL);
	g->mlx->mlx_win = mlx_new_window(g->mlx->mlx_ptr, WIN_X, WIN_Y, "cub3D");
	if (!g->mlx->mlx_win)
		return (FAIL);
	init_texture(g);
	mlx_loop_hook(g->mlx->mlx_ptr, &draw_game, g);
	mlx_hook(g->mlx->mlx_win, ON_KEYDOWN, 0, key_press, g);			// 키 조작
	mlx_hook(g->mlx->mlx_win, ON_DESTROY, 0, close_win, g->mlx);	// x 버튼 클릭 시 윈도우 종료
	mlx_loop(g->mlx->mlx_ptr);
	return (SUCCESS);
}

int main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		err_exit("Wrong arguments", 0, 0);
	if (init_struct(&game))
		err_exit("Init failed", &game, E_INIT);
	if (parse(argv[1], game.cub))
		err_exit("Parsing failed", &game, E_INIT | E_PARSE);
	if (start_cub3d(&game))
		err_exit("cub3D failed", &game, E_INIT | E_PARSE );
	// 모든 에러를 비트연산으로 처리
	// 예를 들어, E_INIT | E_PARSE 인 경우 
	// 0000 0011 로 두개의 비트가 모두 켜져 있기 때문에
	// E_INIT, E_PARSE 에 해당하는 메모리를 모두 해제
	return (0);
}
