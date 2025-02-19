#ifndef RAYCASTING_UTILS_H
# define RAYCASTING_UTILS_H

# include "../../../includes/structures.h"

/* Initializes ray data for a given column */
void init_ray_data(t_app *app, int x, t_ray_data *rd);

/* Runs the DDA loop until a wall is hit and returns the perpendicular wall distance */
double dda(t_app *app, t_ray_data *rd);

#endif
