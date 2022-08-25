/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:03:29 by euyi              #+#    #+#             */
/*   Updated: 2022/08/08 16:57:04 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../assets/philo.h"

// ************************************************************************
// This function converts the four/five philo CMD args to int.			 *
// It initializes table->init_sec to 0 to prevent undefined behaviour	 *
// in the if condition set in time_fetcher() function.					 *
// it initializes both table->death, table->fl_count and table->i to 0	 *
// so as to prevent undefined behaviour in the philo_deeds() func.		*
// It also initializes table->feed_limit, table->last_feed and			*
// table->feed_status dynamically depending on the values of the args	*
// received from command line.											 *
// It returns true if it receives only one philosopher as arg,			 *
// else it returns false in every other situation						 *
// ************************************************************************
int	cmd_fix(t_philo	*table, char **argv)
{
	int	i;

	table->philo_count = my_atoi(argv[1]);
	if (table->philo_count == 1)
		return (1);
	table->lifedur = my_atoi(argv[2]);
	table->feeddur = my_atoi(argv[3]);
	table->sleepdur = my_atoi(argv[4]);
	table->init_sec = 0;
	table->death = 0;
	table->fl_count = 0;
	table->i = 0;
	if (argv[5])
		table->feed_limit = my_atoi(argv[5]);
	else
		table->feed_limit = 0;
	i = 0;
	while (i < table->philo_count)
	{
		table->last_feed[i] = 0;
		table->feed_status[i] = 0;
		i++;
	}
	return (0);
}

// **************************************************************************
// This mutex_switch() function simply uses the flag (param 2) to 			*
// either initialize the pthread_mutex_t array in our t_philo				*
// structure (if flag true) based to the tune of how many philosophers 		*
// we received as CMD arg, or destroy the used pthread_mutex_t array		*
// so as to release their seized memory (if flag false)						*
// **************************************************************************
void	mutex_switch(t_philo *table, int flag)
{
	int				i;
	pthread_mutex_t	*philo_mutex;

	i = table->philo_count;
	philo_mutex = table->philo_mutex;
	if (flag)
	{
		while (i > 0)
		{
			pthread_mutex_init(&philo_mutex[i - 1], NULL);
			i--;
		}
		pthread_mutex_init(&table->gen_lck, NULL);
		pthread_mutex_init(&table->checkpoint, NULL);
	}
	if (!flag)
	{
		while (i > 0)
		{
			pthread_mutex_destroy(&philo_mutex[i - 1]);
			i--;
		}
		pthread_mutex_destroy(&table->gen_lck);
		pthread_mutex_destroy(&table->checkpoint);
	}
}

// ***********************************************************
// death_inspect() is invoked by a parallel empty			 *
// thread running simultaneously alongside the philo		 *
// threads. It simply runs a realtime loop that checks the	 *
// table->last_feed[x] last cached feed time of each philo	 *
// to determine if they have exceeded their life duration.	 *
// If yes, it prints the dead philosophers ID and returns.	 *
// Else, it checks if the feed limit has been reached.		 *
// If yes, it breaks the loop and returns.					 *
// ***********************************************************
void	*death_inspect(void *vptr)
{
	int		i;
	int		j;
	t_philo	*table;

	table = (t_philo *) vptr;
	if (!expect_death(table))
		return (NULL);
	j = table->philo_count - 1;
	i = j;
	while (!feed_limit_verif(table) && i >= 0)
	{
		pthread_mutex_lock(&(table->gen_lck));
		if (time_fetcher(table) - table->last_feed[i] > table->lifedur)
		{
			printf("%ims %i died\n", time_fetcher(table), i + 1);
			table->death = 1;
			pthread_mutex_unlock(&(table->gen_lck));
			return (NULL);
		}
		pthread_mutex_unlock(&(table->gen_lck));
		i--;
		if (i < 0)
			i = j;
	}
	return (NULL);
}

// Just a series of permutations to determine if the
// philosophers should be expecting death at the feeding
// table.
// 4 410 200 200
int	expect_death(t_philo *table)
{
	int	expected;
	int	odd_philo;
	int	even_philo;

	expected = 0;
	odd_philo = 0;
	even_philo = 0;
	if ((table->feeddur >= table->sleepdur)
		|| (table->feeddur + table->sleepdur < table->lifedur))
		expected = 1;
	if (table->philo_count % 2)
		odd_philo = 1;
	else
		even_philo = 1;
	if ((expected && even_philo && table->lifedur >= (table->feeddur * 2))
		|| (expected && odd_philo && table->lifedur >= table->feeddur * 3))
		return (0);
	return (1);
}
