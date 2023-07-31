/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   party_preparations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:17:55 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/31 16:58:35 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/**
 * @brief Prepares a philosopher for the party.
 *
 * This function prepares an individual philosopher for the party. It sets up 
 * the philosopher's own fork and the borrowed fork, initializes the meal 
 * count to 0, and sets the party pointer for the philosopher. It also 
 * initializes the meal_update mutex for the philosopher. If the mutex 
 * initialization fails, the function returns MUTEX_FAIL; otherwise, it 
 * returns SUCCESS.
 * 
 * @param party A pointer to the t_party struct representing the party of 
 * philosophers.
 * @param i The index of the philosopher to prepare in the philosophers 
 * array.
 * @return t_return_value The return value indicating the success or failure 
 * of the preparation.
 */
static t_return_value	prepare_philosopher(t_party *party, unsigned int i)
{
	party->philosophers[i].fork_own = &party->forks[i];
	party->philosophers[i].fork_borrowed = &party->forks[(i + 1)
		% party->number_of_philosophers];
	party->philosophers[i].meal_count = 0;
	party->philosophers[i].party = party;
	if (pthread_mutex_init(&(party->philosophers[i].meal_update),
			NULL) != SUCCESS)
		return (MUTEX_FAIL);
	return (SUCCESS);
}
/**
 * @brief Initializes the mutexes for the party.
 *
 * This function initializes all the necessary mutexes for the party, 
 * including the mutexes for individual philosophers' meal_update and forks, 
 * as well as the guard and dying mutexes for synchronization purposes. If 
 * any mutex initialization fails, the function returns MUTEX_FAIL; 
 * otherwise, it returns SUCCESS.
 *
 * @param party A pointer to the t_party struct representing the party of 
 * philosophers.
 * @return t_return_value The return value indicating the success or failure 
 * of mutex initialization.
 */
static t_return_value	initialize_mutexes(t_party *party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (prepare_philosopher(party, i) != SUCCESS)
			return (MUTEX_FAIL);
		i++;
	}
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (pthread_mutex_init(&(party->forks[i]), NULL) != SUCCESS)
			return (MUTEX_FAIL);
		i++;
	}
	if (pthread_mutex_init(&(party->guard), NULL) != SUCCESS)
		return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->dying), NULL) != SUCCESS)
		return (MUTEX_FAIL);
	return (SUCCESS);
}
/**
 * @brief Prepares the party of philosophers.
 *
 * This function prepares the whole party of philosophers. It initializes the 
 * party's someone_dead flag to 0 and allocates memory for the philosophers 
 * and forks arrays. It then calls initialize_mutexes to set up all the 
 * necessary mutexes. If any memory allocation or mutex initialization fails, 
 * the function returns MALLOC_FAIL or MUTEX_FAIL respectively; otherwise, it 
 * returns SUCCESS.
 *
 * @param party A pointer to the t_party struct representing the party of 
 * philosophers.
 * @return t_return_value The return value indicating the success or failure 
 * of party preparation.
 */
t_return_value	prepare_party(t_party *party)
{
	party->someone_dead = 0;
	party->philosophers = malloc(sizeof(t_philosopher) * \
			party->number_of_philosophers);
	if (party->philosophers == NULL)
		return (MALLOC_FAIL);
	party->forks = malloc(sizeof(pthread_mutex_t) * \
			party->number_of_philosophers);
	if (party->forks == NULL)
		return (MALLOC_FAIL);
	if (initialize_mutexes(party) == MUTEX_FAIL)
		return (MUTEX_FAIL);
	return (SUCCESS);
}
