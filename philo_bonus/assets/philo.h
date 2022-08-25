/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:45:52 by euyi              #+#    #+#             */
/*   Updated: 2022/08/08 17:25:26 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_philo
{
	int				philo_count;
	sem_t			*limit;
	pthread_mutex_t	gen_lck;
	pthread_mutex_t	checkpoint;
	int				last_feed[202];
	int				feed_status[202];
	pthread_t		thread_id[202];
	int				lifedur;
	int				feeddur;
	int				sleepdur;
	int				death;
	int				feed_limit;
	int				fl_count;
	int				init_sec;
	int				init_usec;
	int				i;
}					t_philo;

int		cmd_fix(t_philo	*table, char **argv);
int		my_atoi(const char *nptr);
int		rig(int philo_count, int this_philo);
int		sem_switch(t_philo *table, int flag);
int		death_checker(int *last_feed, int philo_total);
int		time_fetcher(t_philo *table);
int		feed_limit_verif(t_philo *table);
void	*philo_deeds(void *vptr);
void	run_threads(t_philo *table);
int		echo_death(t_philo	*table);
void	echo_eat(t_philo *table, int i);
void	echo_sleep(t_philo *table, int i);
void	echo_think(t_philo *table, int i);
void	*death_inspect(void *vptr);
int		expect_death(t_philo *table);

#endif
