/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:18:20 by lclerc            #+#    #+#             */
/*   Updated: 2023/08/02 18:25:01 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* ************************************************************************** */
/*   Standard Library Headers 												  */
/* ************************************************************************** */
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

/* ************************************************************************** */
/*   Define Constants														  */
/* ************************************************************************** */
# define EXPECT_ARG_COUNT 5
# define EXPECT_ARG_COUNT_WITH_MEALS 6
# define MAX_AMOUNT_PHILO_ALLOWED 300
# define MAX_TIME_VALUE 9223372036854775

/* ************************************************************************** */
/*   Forward Declarations													  */
/* ************************************************************************** */
typedef struct s_party			t_party;
typedef struct s_philosopher	t_philosopher;
typedef enum e_return_value		t_return_value;

/* ************************************************************************** */
/*   Struct Definitions														  */
/* ************************************************************************** */
typedef struct s_philosopher
{
	pthread_t			thread;
	pthread_mutex_t		*fork_own;
	pthread_mutex_t		*fork_borrowed;
	int					meal_count;
	unsigned int		index;
	unsigned long long	time_last_ate;
	pthread_mutex_t		meal_update;
	t_party				*party;
}						t_philosopher;

typedef struct s_party
{
	unsigned int		number_of_philosophers;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned long long	party_start_time;
	int					number_of_meals;
	t_philosopher		*philosophers;
	pthread_t			monitoring_thread;
	pthread_mutex_t		*forks;
	pthread_mutex_t		guard;
	pthread_mutex_t		dying;
	unsigned int		someone_dead;
}						t_party;

/* ************************************************************************** */
/*   Enum Definitions														  */
/* ************************************************************************** */
typedef enum e_return_value
{
	SUCCESS,
	ERROR,
	ARG_COUNT_ERROR,
	ARG_NOT_NUMERIC,
	TOO_MANY_PHILOS,
	MALLOC_FAIL,
	MUTEX_FAIL,
	THREAD_FAIL,
	JOIN_FAIL,
	SINGLE_PHILO_CASE,
	LIFE_GOES_ON,
	SOMEONE_DIED,
	EVERYONE_IS_FED,
}						t_return_value;
/* ************************************************************************** */
/*   Function Prototypes													  */
/* ************************************************************************** */
t_return_value			parse_args(t_party *party, int argc, char **argv);
unsigned long long		get_current_time(void);
void					custom_usleep(unsigned long long duration,
							t_party *party);
void					print_whats_happening(t_philosopher *philosopher,
							char *event);
t_return_value			prepare_party(t_party *party);
t_return_value			start_philosopher(t_party *party, unsigned int i);
t_return_value			start_monitoring(t_party *party);
void					*philosopher_routine(void *philosopher_data);
void					*monitoring_routine(void *party_data);
void					clean_up(t_party *party);
t_return_value			join_threads_to_exit_party(t_party *party);

#endif
