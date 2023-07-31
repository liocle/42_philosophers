/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:18:06 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/31 17:28:13 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
/**
 * @brief Performs the eat-sleep-think routine for a philosopher.
 *
 * This function represents the routine of a philosopher's life where they go 
 * through the process of eating, sleeping, and thinking. If there is only 
 * one philosopher in the party, it returns SINGLE_PHILO_CASE immediately. 
 * Otherwise, the philosopher attempts to pick up their own fork and the 
 * borrowed fork, then proceeds to eat for a specified time. After eating, 
 * the philosopher releases both forks, updates the meal count, and goes to 
 * sleep for a specified time. Finally, the philosopher prints that they are 
 * thinking and returns SUCCESS.
 *
 * @param philosopher A pointer to the t_philosopher struct representing the 
 * philosopher.
 * @return t_return_value The return value indicating the result of the 
 * routine.
 */
static t_return_value	eat_sleep_think(t_philosopher *philosopher)
{
	if (philosopher->party->number_of_philosophers == 1)
		return (SINGLE_PHILO_CASE);
	pthread_mutex_lock(philosopher->fork_own);
	print_whats_happening(philosopher, "has taken a fork");
	pthread_mutex_lock(philosopher->fork_borrowed);
	print_whats_happening(philosopher, "has taken a fork");
	pthread_mutex_lock(&philosopher->meal_update);
	philosopher->time_last_ate = get_current_time();
	pthread_mutex_unlock(&philosopher->meal_update);
	print_whats_happening(philosopher, "is eating");
	custom_usleep(philosopher->party->time_to_eat, philosopher->party);
	pthread_mutex_unlock(philosopher->fork_own);
	pthread_mutex_unlock(philosopher->fork_borrowed);
	pthread_mutex_lock(&philosopher->meal_update);
	philosopher->meal_count++;
	pthread_mutex_unlock(&philosopher->meal_update);
	print_whats_happening(philosopher, "is sleeping");
	custom_usleep(philosopher->party->time_to_sleep, philosopher->party);
	print_whats_happening(philosopher, "is thinking");
	return (SUCCESS);
}
/**
 * @brief Handles the philosopher's life routine.
 *
 * This function is the main routine for a philosopher's life. It starts by 
 * unlocking the party's guard mutex to allow all philosophers to start their 
 * routines concurrently. The philosopher then prints that they are thinking 
 * and waits for a short time (if the philosopher's index is even) to allow 
 * others to start eating first and avoid potential deadlocks. The 
 * philosopher then enters an infinite loop where they keep performing the 
 * eat-sleep-think routine until a philosopher dies. If there is only one 
 * philosopher in the party, the philosopher picks up their fork and breaks 
 * out of the loop. After each iteration, the function checks if someone has 
 * died to break the loop. Once the loop ends, the function returns NULL.
 *
 * @param philosopher_data A pointer to the t_philosopher struct representing 
 * the philosopher data.
 */
void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	int				someone_dead;

	someone_dead = 0;
	philosopher = (t_philosopher *)philosopher_data;
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	print_whats_happening(philosopher, "is thinking");
	if (philosopher->index % 2 == 0)
		custom_usleep(philosopher->party->time_to_eat / 10, philosopher->party);
	while (1)
	{
		if (eat_sleep_think(philosopher) == SINGLE_PHILO_CASE)
		{
			print_whats_happening(philosopher, "has taken a fork");
			break ;
		}
		pthread_mutex_lock(&(philosopher->party->dying));
		someone_dead = philosopher->party->someone_dead;
		pthread_mutex_unlock(&(philosopher->party->dying));
		if (someone_dead != 0)
			break ;
	}
	return (NULL);
}
