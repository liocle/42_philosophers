#include "philosophers.h"

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

static t_return_value	start_party(t_party *party)
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


static t_return_value	run_dining_party(t_party *party)
{
	pthread_mutex_lock(&(party->guard));
	start_party(party);
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
 * @return int `SUCCESS` if the program runs successfully, otherwise `ERROR`
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
