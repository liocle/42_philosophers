/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:18:25 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/31 18:07:10 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/**
 * @brief Create and start a philosopher thread.
 *
 * This function creates a new thread for a philosopher in the given party.
 * The philosopher's index is set, and the `philosopher_routine` function is
 * assigned as the entry point for the thread. The philosopher's data is 
 * passed as an argument to the thread.
 *
 * @param party The party struct containing information about the philosophers.
 * @param i The index of the philosopher to create and start the thread for.
 * @return t_return_value The result of the operation - SUCCESS if the thread
 * was created successfully, THREAD_FAIL otherwise.
 */
t_return_value	start_philosopher(t_party	*party, unsigned int i)
{
	party->philosophers[i].index = i;
	if (pthread_create(&(party->philosophers[i].thread), NULL, \
		philosopher_routine, (void *)&(party->philosophers[i])) != 0)
	{
		printf("Failed to create a philo thread\n");
		return (THREAD_FAIL);
	}
	return (SUCCESS);
}

/**
 * @brief Create and start the monitoring thread.
 *
 * This function creates a new thread for the monitoring routine in the given 
 * party. The `monitoring_routine` function is assigned as the entry point 
 * for the thread. The party data is passed as an argument to the thread.
 *
 * @param party The party struct containing information about the philosophers.
 * @return t_return_value The result of the operation - SUCCESS if the thread
 * was created successfully, THREAD_FAIL otherwise.
 */
t_return_value	start_monitoring(t_party	*party)
{
	if (pthread_create(&(party->monitoring_thread), NULL, \
		monitoring_routine, (void *)party) != 0)
	{
		printf("Failed to create a monitoring thread\n");
		return (THREAD_FAIL);
	}
	return (SUCCESS);
}
