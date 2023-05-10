#include "../includes/philo.h"
#include <stdio.h>

int	main(int argc, char *argv[])
{
	t_philo	table;

	if ( (argc < 5 && write(2, "CMD Error!\n", 11))
		|| (cli_cmd_parser(&table, argv) && write(2, "0ms 1 died\n", 11)) )
		return (1);

	run_threads(&table);

	return (0);
}
