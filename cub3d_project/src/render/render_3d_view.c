#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* render_3d_view: Renders the 3D scene column by column.
   Parameters: (t_app *app)
*/
void	render_3d_view(t_app *app)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		render_column(app, x);
		x = x + 1;
	}
}
