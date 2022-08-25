/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   feeding_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 17:44:02 by euyi              #+#    #+#             */
/*   Updated: 2022/08/08 17:28:42 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../assets/philo.h"

// ***************************************************************
// run_threads() function calls mutex_switch() to initialize	 *
// the required mutexes for our feeding simulation.				 *
// Then duplicates the philo_deeds() into threads. Then it waits *
// for each thread to close, and then it calls mutex_switch()	 *
// again to destroy the memory used by initialized mutexes		 *
// ***************************************************************
void	run_threads(t_philo *table)
{
	pthread_t	*id;
	int			i;

	if (sem_switch(table, 1))
		exit (write(2, "Failed to open semaphore for forks_sem/n", 39));
	id = table->thread_id;
	i = table->philo_count;
	while (i > 0)
	{
		pthread_create(&(id[i - 1]), NULL, philo_deeds, table);
		i--;
		usleep(1);
	}
	i = table->philo_count;
	pthread_create(&(id[i]), NULL, death_inspect, table);
	while (i >= 0)
	{
		pthread_join(id[i], NULL);
		i--;
	}
	sem_switch(table, 0);
}

// *****************************************************************************
// philo_deeds() is simply the action of each philosopher occupying			   *
// their individual thread. It had to be typedefed to "void *" because		   *
// it needs to be fed as arg to pthread_create()							   *
// ((table->i)++; //so that the next thread takes 1 number higher than $this)  *
// (usleep(10); //so that the other feeding philo should start sleeping before *
// this philo starts eating)												   *
// *****************************************************************************
void	*philo_deeds(void *vptr)
{
	int	i;

	pthread_mutex_lock(&(((t_philo *)vptr)->gen_lck));
	i = ((t_philo *)vptr)->i;
	(((t_philo *)vptr)->i)++;
	pthread_mutex_unlock(&(((t_philo *)vptr)->gen_lck));
	while (!feed_limit_verif((t_philo *)vptr))
	{
		sem_wait(((t_philo *)vptr)->limit);
		if (echo_death((t_philo *)vptr))
			break ;
		echo_eat((t_philo *)vptr, i);
		echo_sleep((t_philo *)vptr, i);
		echo_think((t_philo *)vptr, i);
	}
	return (NULL);
}

// *****************************************************************************
// If z.B, 3 philos have to feed 4 times/cycles. During their third feed cycle,*
// the loop in philo_deeds() would have incremented table->fl_count to 9	   *
// meaning (3 * (4 - 1)) or philo_count * (feed_limit - 1)					   *
// It is after table->fl_count becomes 9 that they will all rush back into the *
// loop again but waiting on the mutex to unlock.							   *
// therefore, anything less than or equal to 9, or if feed_limit is false,	   *
// this feed_limit_verif() will return false to enable the loop continue	   *
// *****************************************************************************
int	feed_limit_verif(t_philo *table)
{
	int	loop_cycle;

	pthread_mutex_lock(&(table->gen_lck));
	loop_cycle = table->philo_count * (table->feed_limit - 1);
	if (!table->feed_limit
		|| (table->feed_limit && table->fl_count <= loop_cycle))
	{
		pthread_mutex_unlock(&(table->gen_lck));
		return (0);
	}
	pthread_mutex_unlock(&(table->gen_lck));
	return (1);
}

// **********************************************************************
// rig(int philo_total, int this_philo) uses the total number			*
// of philosophers (param 1) sitting around the table to determine the	*
// number tag of the philosopher who is sitting to the right of			*
// this philosopher (param 2) . 										*
// For instance, if there are 3 philosophers around the table,			*
// Philosopher 0 will have philo 1 sitting to their right hand, 		*
// philo 1 will have philo 2, philo 2 has philo 0.						*
// Return value is the number tag of the philosopher who is sitting to	*
// the right of this_philosopher										*
// **********************************************************************
int	rig(int philo_count, int this_philo)
{
	if (this_philo < philo_count - 1)
		return (this_philo + 1);
	return (0);
}

// *******************************************************************
// time_fetcher() uses gettimeofday() funct to calculate and store	 *
// to table->init_sec && table->init_usec, the time when the first	 *
// philosopher started eating at the table.							 *
// Subsequent calls to this function thereafter will return the time *
// difference between when that first philosopher started to eat	 *
// and the current time in milliseconds								 *
// *******************************************************************
int	time_fetcher(t_philo *table)
{
	struct timeval	gmt;
	int				diff;
	int				init_time;
	int				curr_time;

	pthread_mutex_lock(&(table->checkpoint));
	gettimeofday(&gmt, NULL);
	if (!table->init_sec)
	{
		table->init_sec = (int)gmt.tv_sec;
		table->init_usec = (int)gmt.tv_usec;
		pthread_mutex_unlock(&(table->checkpoint));
		return (0);
	}
	init_time = table->init_sec * 1000 + (table->init_usec / 1000);
	curr_time = (int)gmt.tv_sec * 1000 + (gmt.tv_usec / 1000);
	diff = curr_time - init_time;
	pthread_mutex_unlock(&(table->checkpoint));
	return (diff);
}
