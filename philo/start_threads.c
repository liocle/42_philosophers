/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:18:25 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/29 21:51:37 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
