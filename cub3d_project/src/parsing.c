#include "../includes/parsing.h"
#include "../includes/file_utils.h"
#include "../includes/utils.h"
#include <stdio.h>
#include <stdlib.h>

/* process_texture:
   Processes a texture configuration line and stores the texture path.
   (app is unused here.)
*/
static void process_texture(t_app *app, const char *line, const char *id,
	char **dest, int *configCount)
{
	(void)app;
	if (ft_strncmp(line, id, 3) == 0)
	{
		*dest = ensure_texture_path(line + 3);
		if (!*dest)
		{
			perror("Texture path error");
			exit(EXIT_FAILURE);
		}
		(*configCount)++;
	}
}

/* process_color:
   Processes a color configuration line and sets the appropriate color.
*/
static void process_color(t_app *app, const char *line, char type, int *configCount)
{
	const char *ptr = line + 2;
	int r = ft_atoi(ptr);
	while (*ptr && *ptr != ',') ptr++;
	if (*ptr == ',') ptr++;
	int g = ft_atoi(ptr);
	while (*ptr && *ptr != ',') ptr++;
	if (*ptr == ',') ptr++;
	int b = ft_atoi(ptr);
	if (type == 'F')
		app->config.floorColor = ft_pixel(r, g, b, 255);
	else if (type == 'C')
		app->config.ceilingColor = ft_pixel(r, g, b, 255);
	(*configCount)++;
}

/* process_config_line:
   Determines if a line is a configuration line; if not, flags mapStarted.
*/
static void process_config_line(t_app *app, char *line,
	int *configCount, int *mapStarted)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		process_texture(app, line, "NO ", &app->config.texNorthPath, configCount);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		process_texture(app, line, "SO ", &app->config.texSouthPath, configCount);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		process_texture(app, line, "WE ", &app->config.texWestPath, configCount);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		process_texture(app, line, "EA ", &app->config.texEastPath, configCount);
	else if (line[0] == 'F' && line[1] == ' ')
		process_color(app, line, 'F', configCount);
	else if (line[0] == 'C' && line[1] == ' ')
		process_color(app, line, 'C', configCount);
	else
		*mapStarted = 1;
}

/* add_map_line:
   Creates a new map-line node and appends it to the list.
*/
static void add_map_line(t_mapLine **head, t_mapLine **tail,
	const char *line, int *count)
{
	t_mapLine *node = malloc(sizeof(t_mapLine));
	if (!node) { perror("malloc error"); exit(EXIT_FAILURE); }
	node->line = ft_strdup(line);
	if (!node->line) { perror("ft_strdup error"); exit(EXIT_FAILURE); }
	node->next = NULL;
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
	(*count)++;
}

/* allocate_map:
   Computes maximum width and allocates a 2D map array.
*/
static void allocate_map(t_app *app, t_mapLine *head, int mapLineCount)
{
	int width = 0;
	for (t_mapLine *node = head; node; node = node->next)
	{
		int len = ft_strlen(node->line);
		if (len > width)
			width = len;
	}
	app->map.height = mapLineCount;
	app->map.width = width;
	app->map.world = malloc(sizeof(int *) * app->map.height);
	if (!app->map.world) { perror("malloc error"); exit(EXIT_FAILURE); }
	for (int i = 0; i < app->map.height; i++)
	{
		app->map.world[i] = malloc(sizeof(int) * app->map.width);
		if (!app->map.world[i]) { perror("malloc error"); exit(EXIT_FAILURE); }
		for (int j = 0; j < app->map.width; j++)
			app->map.world[i][j] = 0;
	}
}

/* fill_map:
   Fills the 2D map array from the linked list and sets player data.
*/
static void fill_map(t_app *app, t_mapLine *head)
{
	int row = 0;
	for (t_mapLine *node = head; node; node = node->next, row++)
	{
		for (int j = 0; j < app->map.width; j++)
		{
			char c = ' ';
			if (j < (int)ft_strlen(node->line))
				c = node->line[j];
			if (c >= '0' && c <= '9')
				app->map.world[row][j] = c - '0';
			else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				app->player.posX = j + 0.5;
				app->player.posY = row + 0.5;
				if (c == 'N') { app->player.dirX = 0; app->player.dirY = -1;
					app->player.planeX = 0.66; app->player.planeY = 0; }
				else if (c == 'S') { app->player.dirX = 0; app->player.dirY = 1;
					app->player.planeX = -0.66; app->player.planeY = 0; }
				else if (c == 'E') { app->player.dirX = 1; app->player.dirY = 0;
					app->player.planeX = 0; app->player.planeY = 0.66; }
				else if (c == 'W') { app->player.dirX = -1; app->player.dirY = 0;
					app->player.planeX = 0; app->player.planeY = -0.66; }
				app->map.world[row][j] = 0;
			}
			else if (c == ' ')
				app->map.world[row][j] = 0;
			else { printf("Invalid map character '%c' at (%d, %d)\n", c, row, j);
				exit(EXIT_FAILURE); }
		}
	}
}

/* build_map_list:
   Scans file_content manually to build a linked list of map lines.
*/
static t_mapLine *build_map_list(t_app *app, const char *file_content,
	int *configCount, int *mapStarted, int *mapLineCount)
{
	char *p = (char *)file_content;
	t_mapLine *head = NULL, *tail = NULL;
	while (*p)
	{
		int len = 0;
		while (p[len] && p[len] != '\n')
			len++;
		char *line = ft_strndup(p, len);
		if (!line) { perror("ft_strndup error"); exit(EXIT_FAILURE); }
		p += len; if (*p == '\n') p++;
		while (ft_strlen(line) > 0 &&
			line[ft_strlen(line) - 1] == '\r')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] != '\0')
		{
			if (!*mapStarted)
				process_config_line(app, line, configCount, mapStarted);
			if (*mapStarted)
				add_map_line(&head, &tail, line, mapLineCount);
		}
		free(line);
	}
	return head;
}

/* parse_cub_file:
   Main function: reads file content, builds the map list, allocates and fills
   the map array, then cleans up.
*/
void parse_cub_file(t_app *app, const char *filename)
{
	char *file_content = read_file_to_str(filename);
	int configCount = 0;
	int mapStarted = 0;
	int mapLineCount = 0;
	t_mapLine *head = build_map_list(app, file_content,
		&configCount, &mapStarted, &mapLineCount);
	free(file_content);

	printf("%i \n, %i \n", configCount, mapLineCount);
	if (configCount < 6 || mapLineCount == 0)
	{
		printf("Incomplete configuration or no map data.\n");
		exit(EXIT_FAILURE);
	}
	allocate_map(app, head, mapLineCount);
	fill_map(app, head);
	while (head)
	{
		t_mapLine *tmp = head;
		head = head->next;
		free(tmp->line);
		free(tmp);
	}
}
