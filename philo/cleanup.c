/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:16:46 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/29 22:17:16 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/**
 * @brief Cleans up resources and destroys mutexes related to the dining party.
 *
 * This function is responsible for cleaning up resources and destroying the 
 * mutexes associated with the forks and meal updates for each philosopher. 
 * It also frees the memory allocated for the philosophers and forks arrays.
 *
 * @param party	A pointer to party struct
 */
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
	free(party->philosophers);
	free(party->forks);
}

/**
 * @brief Joins the threads of each philosopher.
 *
 * This function is responsible for joining the threads of each philosopher. 
 * If any join operation fails, it prints an error message and returns 
 * JOIN_FAIL.
 *
 * @param party	A pointer to party struct
 * @return t_return_value SUCCESS or JOIN_FAIL
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
			return (JOIN_FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Joins the monitoring thread.
 *
 * This function is responsible for joining the monitoring thread, which checks 
 * if any philosopher has starved or if everyone is fed. If the join operation 
 * fails, it prints an error message and returns JOIN_FAIL.
 *
 * @param party	A pointer to party struct
 * @return t_return_value SUCCESS or JOIN_FAIL
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

/**
 * @brief Joins the threads of each philosopher to exit the party.
 *
 * This function joins the threads of each philosopher and the monitoring 
 * thread to ensure all threads have completed their tasks before exiting the 
 * program. If any join operation fails, it prints an error message and returns
 * JOIN_FAIL.
 *
 * @param party	A pointer to party struct
 * @return t_return_value SUCCESS or JOIN_FAIL
 */
t_return_value	join_threads_to_exit_party(t_party *party)
{
	if (join_monitoring_thread(party) == JOIN_FAIL)
	{
		join_philosopher_threads(party);
		return (JOIN_FAIL);
	}
	else if (join_philosopher_threads(party) == JOIN_FAIL)
		return (JOIN_FAIL);
	return (SUCCESS);
}
