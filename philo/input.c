/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:17:00 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/31 15:32:38 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Converts an ASCII string to an unsigned long long integer.
 *
 * This function takes an ASCII string as input and converts it to an unsigned 
 * long long integer. It reads characters from the string until it finds a 
 * non-digit character or reaches the end of the string. If the conversion is 
 * successful and the resulting value is within the allowed range (less than 
 * or equal to MAX_TIME_VALUE), it returns the converted value. If the string 
 * contains non-digit characters or the converted value exceeds MAX_TIME_VALUE,
 * it returns 0 to indicate an error.
 *
 * @param str The input ASCII string to be converted.
 * @return unsigned long long The converted value if successful, otherwise 0.
 */
static unsigned long long	ascii_to_positive_int(const char *str)
{
	unsigned long long	result;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		if (result > MAX_TIME_VALUE)
			return (0);
		str++;
	}
	if (*str != '\0')
		return (0);
	return ((unsigned long long)result);
}
/**
 * @brief Validates and stores the arguments provided in the command-line.
 *
 * This function is responsible for validating the command-line arguments and 
 * storing them in the `t_party` struct. It first converts the input argument 
 * string to an unsigned long long integer using `ascii_to_positive_int` 
 * function. If the conversion is successful and the value is within the 
 * allowed range, it stores the value in the appropriate field of the `t_party
 * ` struct. If the conversion fails (e.g., the argument contains non-digit 
 * characters) or the value exceeds the allowed range, it returns an 
 * appropriate error code.
 *
 * @param party A pointer to the `t_party` struct where the validated 
 * arguments will be stored.
 * @param string The command-line argument string to be validated and 
 * converted.
 * @param argument The position of the argument in the commakd-line (1 to 
 * 5).
 * @return t_return_value SUCCESS or FAILURE 
 */
static t_return_value	store_arg_if_validated(t_party *party, char *string,
		int argument)
{
	unsigned long long	validated_value;

	validated_value = ascii_to_positive_int(string);
	if (validated_value == 0)
		return (ARG_NOT_NUMERIC);
	else if (argument == 1)
	{
		if (validated_value > MAX_AMOUNT_PHILO_ALLOWED)
			return (TOO_MANY_PHILOS);
		party->number_of_philosophers = validated_value;
	}
	else if (argument == 2)
		party->time_to_die = validated_value * 1000;
	else if (argument == 3)
		party->time_to_eat = validated_value * 1000;
	else if (argument == 4)
		party->time_to_sleep = validated_value * 1000;
	else if (argument == 5)
		party->number_of_meals = (int)validated_value;
	return (SUCCESS);
}

/**
 * @brief Prints the usage information for running the philosopher simulation.
 *
 * This function prints the usage information for running the philosopher 
 * simulation to the standard output. It provides a brief description of the 
 * expected command-line arguments and their format.
 */
static void	print_philo_usage(void)
{
	printf("Usage: ./philo\tnumber_of_philosophers\t(max 200 philos)\n\
		time_to_die\t(millisecond, ms)\n\t\ttime_to_eat\t(ms)\n\
		time_to_sleep\t(ms)\n\
		[number_of_times_each_philosopher_must_eat]\n");
}

/**
 * @brief Checks the number of command-line arguments and validates their count.
 *
 * This function checks the number of command-line arguments provided by the 
 * user and validates their count. It expects either `EXPECT_ARG_COUNT` or 
 * `EXPECT_ARG_COUNT_WITH_MEALS` arguments.
 * If the provided argument count does not match the expected count, it 
 * prints an error message along with the usage information and returns an 
 * error code. Otherwise, it returns SUCCESS to indicate a valid number of 
 * arguments.
 *
 * @param argc The number of command-line arguments.
 * @return t_return_value The return value indicating the success or failure 
 * of the argument count check.
 */
static t_return_value	argument_number_check(int argc)
{
	if (argc == EXPECT_ARG_COUNT || argc == EXPECT_ARG_COUNT_WITH_MEALS)
		return (SUCCESS);
	printf("Improper amount of arguments. Expected %d or %d arguments\n\n",
		EXPECT_ARG_COUNT - 1,
		EXPECT_ARG_COUNT_WITH_MEALS - 1);
	print_philo_usage();
	return (ARG_COUNT_ERROR);
}

/**
 * @brief Parses and validates the command-line arguments for the philosopher 
 * simulation.
 *
 * This function parses and validates the command-line arguments provided by 
 * the user for the philosopher simulation. It first checks the number of 
 * arguments using `argument_number_check` and then proceeds to validate each 
 * argument using `store_arg_if_validated`. If any argument fails validation, 
 * it prints an appropriate error message along with the usage information 
 * and returns the corresponding error code. If all arguments are valid, it 
 * stores them in the `t_party` struct and returns SUCCESS.
 *
 * @param party A pointer to the `t_party` struct where the validated 
 * arguments will be stored.
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return t_return_value Indicates the success or failure of the argument 
 * parsing and validation process.
 */
t_return_value	parse_args(t_party *party, int argc, char **argv)
{
	t_return_value	ret_val;
	int				index;

	index = 1;
	ret_val = argument_number_check(argc);
	if (ret_val == ARG_COUNT_ERROR)
		return (ARG_COUNT_ERROR);
	if (argc == 5)
		party->number_of_meals = -1;
	while (index < argc)
	{
		ret_val = store_arg_if_validated(party, argv[index], index);
		if (ret_val != SUCCESS)
		{
			if (ret_val == TOO_MANY_PHILOS)
				printf("Recommended philosopher amount should be less or equal to \
200\n\n");
			else if (ret_val == ARG_NOT_NUMERIC)
				printf("Arguments must be only non zero positive numbers\n\n");
			print_philo_usage();
			return (ret_val);
		}
		index++;
	}
	return (SUCCESS);
}
