#include "philosophers.h"

void	clean_up(t_party	*party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_destroy(&(party->forks[i]));
		pthread_mutex_destroy(&(party->philosophers[i].meal_update));
		i++;
	}
	pthread_mutex_destroy(&(party->guard));
	pthread_mutex_destroy(&(party->dying));
	pthread_mutex_destroy(&(party->reporting_enough_meals));
	free(party->philosophers);
	free(party->forks);
}

/**
 * @brief Joins all philosopher threads.
 *
 * This function is responsible for joining all the philosopher threads created
 * by the party. It iterates through the `party->philosophers` array and calls
 * `pthread_join` on each philosopher's thread. If an error occurs during
 * joining, it prints an error message indicating which philosopher thread
 * failed to join.
 *
 * @param party Pointer to the `t_party` struct holding party information
 * @return Returns SUCCESS if all philosopher threads are joined successfully,
	otherwise FAILED.
 */
static t_return_value	join_philosopher_threads(t_party *party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (pthread_join(party->philosophers[i].thread, NULL) != SUCCESS)
		{
			printf("Failed to join philosopher thread %u\n", i);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Joins the monitoring thread.
 *
 * This function is responsible for joining the monitoring thread created by
 * the party. It calls `pthread_join` on the `party->monitoring_thread`. If an
 * error occurs during joining, it prints an error message.
 *
 * @param party Pointer to the `t_party` struct holding party information
 * @return Returns SUCCESS if all philosopher threads are joined successfully,
	otherwise FAILED.
 */
static t_return_value	join_monitoring_thread(t_party *party)
{
	if (pthread_join(party->monitoring_thread, NULL) != SUCCESS)
	{
		printf("Failed to join monitoring thread\n");
		return (JOIN_FAIL);
	}
	return (SUCCESS);
}

t_return_value	join_threads_to_exit_party(t_party *party)
{
	if (join_monitoring_thread(party) == JOIN_FAIL)
	{
		join_philosopher_threads(party);
		return (JOIN_FAIL);
	}
	else if (join_philosopher_threads(party) == JOIN_FAIL)
		return (JOIN_FAIL); // Remember cleaning after join FAIL
	return (SUCCESS);
}