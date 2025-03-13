/* src/frame_update.c */
#include "../../includes/render.h"
#include "../../includes/input_handler.h"

void	update_frame(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	process_input(app);
	clear_image(app->gfx.img, CLEAR_COLOR);
	render_3d_view(app);
	render_minimap(app);
}
