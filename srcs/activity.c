#include "../includes/philo.h"

// **************************************************************************
// print_death() function simply checks whether the current elapsed time	* 
// since the very first philo had started eating at the table is greater	*
// than the (time $this_philo last_fed + life_duration CMD arg)				*
// Any of those makes it print the death message for $this_philo, then it 	*
// unlocks the mutexes the dead philo had locked, so that the other philo	*
// beside the dead philo dont wait for his fork forever, then it returns 	*
// true, else it returns false									*			*
// **************************************************************************
int	echo_death(t_philo	*table)
{
	pthread_mutex_lock(&(table->gen_lck));
	if (table->death)
	{
		pthread_mutex_unlock(&(table->gen_lck));
		sem_post(table->limit);
		return (1);
	}	
	pthread_mutex_unlock(&(table->gen_lck));
	return (0);
}

void	echo_eat(t_philo *table, int i)
{
	pthread_mutex_lock(&(table->gen_lck));
	table->last_feed[i] = time_fetcher(table);
	printf("%ims %i has taken a fork\n", table->last_feed[i], i + 1);
	printf("%ims %i has taken a fork\n", table->last_feed[i], i + 1);
	printf("%ims %i is eating\n", table->last_feed[i], i + 1);
	pthread_mutex_unlock(&(table->gen_lck));
	usleep(table->feeddur * 1000);
	sem_post(table->limit);
}

void	echo_sleep(t_philo *table, int i)
{
	pthread_mutex_lock(&(table->gen_lck));
	printf("%ims %i is sleeping\n", time_fetcher(table), i + 1);
	pthread_mutex_unlock(&(table->gen_lck));
	usleep(table->sleepdur * 1000);
}

// **********************************************************
// The usleep(10) here is just to guard against				*
// the few times when an unfortunate odd number 			*
// philosopher gets waiting									*
// to eat and the next batch comes and passes him			*
// by (due to the absence of FIFO in sem and mutex locks)	*
// causing him to diefeeding locks 							*
// **********************************************************
void	echo_think(t_philo *table, int i)
{
	pthread_mutex_lock(&(table->gen_lck));
	printf("%ims %i is thinking\n", time_fetcher(table), i + 1);
	table->fl_count += 1;
	pthread_mutex_unlock(&(table->gen_lck));
	usleep(10);
}
