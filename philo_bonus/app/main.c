/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 16:34:34 by euyi              #+#    #+#             */
/*   Updated: 2022/08/03 17:59:45 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../assets/philo.h"
#include <stdio.h>

int	main(int argc, char *argv[])
{
	t_philo	table;

	if ((argc < 5 && write(2, "CMD Error!\n", 11))
		|| (cmd_fix(&table, argv) && write(2, "0ms 1 died\n", 11)))
		return (1);
	run_threads(&table);
	return (0);
}
