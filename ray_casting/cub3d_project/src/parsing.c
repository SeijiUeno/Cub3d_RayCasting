#include "../includes/parsing.h"
#include "../includes/file_utils.h"
#include "../includes/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void	parse_cub_file(t_app *app, const char *filename)
{
	char		*file_content;
	char		*p;
	int			configCount = 0;
	int			mapStarted = 0;
	t_mapLine	*head = NULL;
	t_mapLine	*tail = NULL;
	int			mapLineCount = 0;

	file_content = read_file_to_str(filename);
	p = file_content;
	while (*p)
	{
		char *line_start = p;
		while (*p && *p != '\n')
			p++;
		if (*p == '\n')
		{
			*p = '\0';
			p++;
		}
		/* Remove trailing '\r' if present */
		int len = ft_strlen(line_start);
		while (len > 0 && line_start[len - 1] == '\r')
		{
			line_start[len - 1] = '\0';
			len--;
		}
		if (line_start[0] == '\0')
			continue;
		if (!mapStarted)
		{
if (ft_strncmp(line_start, "NO ", 3) == 0)
{
    char *temp = ensure_texture_path(line_start + 3);
    if (!temp)
    {
        perror("Texture path error");
        exit(EXIT_FAILURE);
    }
    app->config.texNorthPath = temp;
    configCount++;
    continue;
}
else if (ft_strncmp(line_start, "SO ", 3) == 0)
{
    char *temp = ensure_texture_path(line_start + 3);
    if (!temp)
    {
        perror("Texture path error");
        exit(EXIT_FAILURE);
    }
    app->config.texSouthPath = temp;
    configCount++;
    continue;
}
else if (ft_strncmp(line_start, "WE ", 3) == 0)
{
    char *temp = ensure_texture_path(line_start + 3);
    if (!temp)
    {
        perror("Texture path error");
        exit(EXIT_FAILURE);
    }
    app->config.texWestPath = temp;
    configCount++;
    continue;
}
else if (ft_strncmp(line_start, "EA ", 3) == 0)
{
    char *temp = ensure_texture_path(line_start + 3);
    if (!temp)
    {
        perror("Texture path error");
        exit(EXIT_FAILURE);
    }
    app->config.texEastPath = temp;
    configCount++;
    continue;
}

			else if (line_start[0] == 'F' && line_start[1] == ' ')
			{
				const char *ptr = line_start + 2;
				int r = ft_atoi(ptr);
				while (*ptr && *ptr != ',')
					ptr++;
				if (*ptr == ',')
					ptr++;
				int g = ft_atoi(ptr);
				while (*ptr && *ptr != ',')
					ptr++;
				if (*ptr == ',')
					ptr++;
				int b = ft_atoi(ptr);
				app->config.floorColor = ft_pixel(r, g, b, 255);
				configCount++;
				continue;
			}
			else if (line_start[0] == 'C' && line_start[1] == ' ')
			{
				const char *ptr = line_start + 2;
				int r = ft_atoi(ptr);
				while (*ptr && *ptr != ',')
					ptr++;
				if (*ptr == ',')
					ptr++;
				int g = ft_atoi(ptr);
				while (*ptr && *ptr != ',')
					ptr++;
				if (*ptr == ',')
					ptr++;
				int b = ft_atoi(ptr);
				app->config.ceilingColor = ft_pixel(r, g, b, 255);
				configCount++;
				continue;
			}
			else
				mapStarted = 1;
		}
		if (mapStarted)
		{
			t_mapLine *node = malloc(sizeof(t_mapLine));
			if (!node)
			{
				perror("malloc error");
				exit(EXIT_FAILURE);
			}
			node->line = ft_strdup(line_start);
			if (!node->line)
			{
				perror("ft_strdup error");
				exit(EXIT_FAILURE);
			}
			node->next = NULL;
			if (!head)
				head = node;
			else
				tail->next = node;
			tail = node;
			mapLineCount++;
		}
	}
	free(file_content);
	if (configCount < 6)
	{
		printf("Incomplete configuration in cub file.\n");
		exit(EXIT_FAILURE);
	}
	if (mapLineCount == 0)
	{
		printf("No map data found in cub file.\n");
		exit(EXIT_FAILURE);
	}
	/* Determine map dimensions */
	app->map.height = mapLineCount;
	app->map.width = 0;
	for (t_mapLine *node = head; node; node = node->next)
	{
		int lineLen = ft_strlen(node->line);
		if (lineLen > app->map.width)
			app->map.width = lineLen;
	}
	/* Allocate app->map.world */
	app->map.world = malloc(sizeof(int *) * app->map.height);
	if (!app->map.world)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < app->map.height; i++)
	{
		app->map.world[i] = malloc(sizeof(int) * app->map.width);
		if (!app->map.world[i])
		{
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < app->map.width; j++)
			app->map.world[i][j] = 0;
	}
	int currentLine = 0;
	int playerFound = 0;
	for (t_mapLine *node = head; node; node = node->next, currentLine++)
	{
		for (int j = 0; j < app->map.width; j++)
		{
			char cell = ' ';
			if (j < (int)ft_strlen(node->line))
				cell = node->line[j];
			if (cell >= '0' && cell <= '9')
				app->map.world[currentLine][j] = cell - '0';
			else if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
			{
				app->player.posX = j + 0.5;
				app->player.posY = currentLine + 0.5;
				if (cell == 'N')
				{
					app->player.dirX = 0;
					app->player.dirY = -1;
					app->player.planeX = 0.66;
					app->player.planeY = 0;
				}
				else if (cell == 'S')
				{
					app->player.dirX = 0;
					app->player.dirY = 1;
					app->player.planeX = -0.66;
					app->player.planeY = 0;
				}
				else if (cell == 'E')
				{
					app->player.dirX = 1;
					app->player.dirY = 0;
					app->player.planeX = 0;
					app->player.planeY = 0.66;
				}
				else if (cell == 'W')
				{
					app->player.dirX = -1;
					app->player.dirY = 0;
					app->player.planeX = 0;
					app->player.planeY = -0.66;
				}
				app->map.world[currentLine][j] = 0;
				playerFound++;
			}
			else if (cell == ' ')
				app->map.world[currentLine][j] = 0;
			else
			{
				printf("Invalid map character '%c' at (%d, %d)\n", cell, currentLine, j);
				exit(EXIT_FAILURE);
			}
		}
		free(node->line);
	}
	while (head)
	{
		t_mapLine *tmp = head;
		head = head->next;
		free(tmp);
	}
	if (playerFound != 1)
	{
		printf("Error: Map must have exactly one spawn; found %d\n", playerFound);
		exit(EXIT_FAILURE);
	}
}
