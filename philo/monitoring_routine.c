/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:17:17 by lclerc            #+#    #+#             */
/*   Updated: 2023/08/01 13:17:08 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Checks if any philosopher has starved.
 *
 * This function checks if any philosopher in the party has starved by 
 * comparing the current time with the time the philosopher last ate. If the 
 * time since the philosopher's last meal exceeds the time_to_die value for 
 * the party, the philosopher is considered to have starved, and the function 
 * prints a death message. It returns SOMEONE_DIED if a philosopher has 
 * starved,otherwise LIFE_GOES_ON.
 *
 * @param party A pointer to the t_party struct representing the party of 
 * philosophers.
 * @return t_return_value The return value indicating if a philosopher has 
 * starved or not.
 */
static t_return_value	someone_starved(t_party *party)
{
	unsigned int		i;
	unsigned long long	curr_time;
	unsigned long long	time_philo_last_ate;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		curr_time = get_current_time();
		pthread_mutex_lock(&(party->philosophers[i].meal_update));
		time_philo_last_ate = party->philosophers[i].time_last_ate;
		pthread_mutex_unlock(&(party->philosophers[i].meal_update));
		if (curr_time - time_philo_last_ate >= party->time_to_die)
		{
			print_whats_happening(&(party->philosophers[i]), "died");
			return (SOMEONE_DIED);
		}
		i++;
	}
	return (LIFE_GOES_ON);
}

/**
 * @brief Checks if all philosophers have eaten the specified number of 
 * meals.
 *
 * This function checks if all philosophers in the party have eaten the 
 * specified number of meals (party->number_of_meals). If party->
 * number_of_meals is negative, it means the philosophers can eat infinitely, 
 * and the function returns LIFE_GOES_ON. If all philosophers have eaten the 
 * specified number of meals, it returns EVERYONE_IS_FED; otherwise, it 
 * returns LIFE_GOES_ON.
 *
 * @param party A pointer to the t_party struct representing the party of 
 * philosophers.
 * @return t_return_value The return value indicating if all philosophers 
 * have eaten the specified number of meals or not.
 */
static t_return_value	everyone_is_fed(t_party *party)
{
	unsigned int	i;
	int				philo_meal_count;

	if (party->number_of_meals < 0)
		return (LIFE_GOES_ON);
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_lock(&(party->philosophers[i].meal_update));
		philo_meal_count = party->philosophers[i].meal_count;
		pthread_mutex_unlock(&(party->philosophers[i].meal_update));
		if (philo_meal_count < party->number_of_meals)
			return (LIFE_GOES_ON);
		i++;
	}
	return (EVERYONE_IS_FED);
}

/**
 * @brief Monitoring routine for the philosophers.
 *
 * This function serves as a monitoring routine for the philosophers' party. 
 * It continuously checks if any philosopher has starved or if all 
 * philosophers have eaten the specified number of meals. If either of these 
 * conditions is met, it sets party->someone_dead to 1 and exits the loop. 
 * A delay of 500 us is used between each loop iteration.
 * 
 * @param party_data A pointer to the t_party struct representing the party 
 * of philosophers.
 * @return void* This function always returns NULL as it is intended to be 
 * used as a pthread routine.
 */
void	*monitoring_routine(void *party_data)
{
	t_party	*party;

	party = (t_party *)party_data;
	while (1)
	{
		if (someone_starved(party) == SOMEONE_DIED)
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			break ;
		}
		if (everyone_is_fed(party) == EVERYONE_IS_FED)
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			break ;
		}
		usleep(500);
	}
	return (NULL);
}
