/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:02:41 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/17 16:44:04 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

# define TRUE 1

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				stop_simulation;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	t_philosopher	*philosophers;
}	t_data;

long	get_timestamp(void);
void	print_status(t_philosopher *philo, const char *status);
void	*philosopher_routine(void *arg);
void	cleanup(t_data *data);

void	initialize_forks(t_data *data);
void	initialize_philosophers(t_data *data);
t_data	*initialize_data(int argc, char **argv);

void	monitor_check_die(t_data *data, t_philosopher *philo);
void	monitor_philosophers(t_data *data);

int		ft_atoi(const char *nptr);

#endif