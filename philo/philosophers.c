/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:18:12 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/31 17:35:41 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/**
 * @brief Initializes the party start time and sets the time_last_ate for all 
 * philosophers.
 *
 * This function initializes the party's start time to the current time and 
 * sets the time_last_ate for each philosopher to the party's start time. It 
 * updates the time_last_ate of each philosopher to the party's start time, 
 * indicating that they have not eaten since the party's inception.
 *
 * @param party A pointer to the t_party struct representing the party data.
 */
static void	initialize_party_start_time(t_party *party)
{
	unsigned int	i;

	i = 0;
	party->party_start_time = get_current_time();
	while (i < party->number_of_philosophers)
	{
		party->philosophers[i].time_last_ate = party->party_start_time;
		i++;
	}
}

/**
 * @brief Initializes the dining party.
 *
 * This function starts the dining party by creating threads for each 
 * philosopher using the `start_philosopher` function. If creating a thread 
 * for any philosopher fails, the function unlocks the party's guard mutex 
 * and returns THREAD_FAIL. If all threads are successfully created, the 
 * function initializes the party start time using 
 * `initialize_party_start_time` and starts the monitoring thread using 
 * `start_monitoring`. If starting the monitoring thread fails, the function 
 * unlocks the party's guard mutex and returns THREAD_FAIL. Otherwise, it 
 * returns SUCCESS.
 *
 * @param party A pointer to the t_party struct representing the party data.
 * @return t_return_value The return value indicating the result of 
 * initializing the party.
 */
static t_return_value	initialize_party(t_party *party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (start_philosopher(party, i) == THREAD_FAIL)
		{
			pthread_mutex_unlock(&(party->guard));
			return (THREAD_FAIL);
		}
		i++;
	}
	initialize_party_start_time(party);
	if (start_monitoring(party) == THREAD_FAIL)
	{
		pthread_mutex_unlock(&(party->guard));
		return (THREAD_FAIL);
	}
	return (SUCCESS);
}
/**
 * @brief Runs the dining party.
 *
 * This function is the core of the dining party process. It locks the 
 * party's guard mutex, then initializes and starts the party using the 
 * `initialize_party` function. After initializing and starting the party, it 
 * unlocks the party's guard mutex to allow all philosophers to start their 
 * routines concurrently. The function then waits for all philosopher threads 
 * to finish using the `join_threads_to_exit_party` function. If joining any 
 * philosopher thread fails, the function returns JOIN_FAIL. Otherwise, it 
 * returns SUCCESS.
 *
 * @param party A pointer to the t_party struct representing the party data.
 * @return t_return_value SUCCESS, THREAD_FAIL or JOIN_FAIL
 */
static t_return_value	run_dining_party(t_party *party)
{
	pthread_mutex_lock(&(party->guard));
	if (initialize_party(party) == THREAD_FAIL)
		return (THREAD_FAIL);
	pthread_mutex_unlock(&(party->guard));
	if (join_threads_to_exit_party(party) == JOIN_FAIL)
		return (JOIN_FAIL);
	return (SUCCESS);
}

/**
 * @brief Entry point of the program.
 *
 * This is the main function of the program. It initializes the `t_party`
 * struct, parses the command-line arguments using `parse_args`, prepares the
 * party using `prepare_party`, runs the party using `run_party`, and finally
 * cleans up the resources using `clean_up`.
 *
 * @param ac Number of command-line arguments
 * @param av Array of command-line argument strings
 * @return int SUCCESS or the various error messages carried by ret_val.
 */
int	main(int ac, char **av)
{
	t_party			party;
	t_return_value	ret_val;

	ret_val = parse_args(&party, ac, av);
	if (ret_val != SUCCESS)
		return (ret_val);
	ret_val = prepare_party(&party);
	if (ret_val != SUCCESS)
		return (ret_val);
	ret_val = run_dining_party(&party);
	if (ret_val != SUCCESS)
	{
		clean_up(&party);
		return (ret_val);
	}
	clean_up(&party);
	return (SUCCESS);
}
