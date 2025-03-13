/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:28:53 by bmoretti          #+#    #+#             */
/*   Updated: 2025/03/13 16:42:32 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

void	import(int argc, char *argv[], t_level *lvl)
{
	t_import_elements	lvl_el;

	lvl_el.fd = basic_validation(argc, argv);
	lvl_el.lvl = lvl;
	lvl_el.line = NULL;
	get_elements(&lvl_el);
	get_map(&lvl_el);
	map_validation(lvl);
	close(lvl_el.fd);
}
