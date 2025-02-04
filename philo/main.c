/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:06:51 by ezeppa            #+#    #+#             */
/*   Updated: 2025/02/04 11:49:26 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_philosopher *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->stop_simulation)
	{
		ft_printf("%ld %d %s\n",
			get_timestamp() - philo->data->start_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!philo->data->stop_simulation)
	{
		if (philo->id % 2 == 0)
			usleep(1000);
		print_status(philo, "is thinking");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		philo->last_meal_time = get_timestamp();
		usleep(philo->data->time_to_eat * 1000);
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
	}
	return (NULL);
}

void	cleanup(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->philosophers)
		free(data->philosophers);
	pthread_mutex_destroy(&data->print_lock);
	free(data);
	data = NULL;
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	if (argc < 5 || argc > 6)
	{
		ft_printf("Usage: %s"
			" number_of_philosophers time_to_die time_to_eat time_to_sleep"
			" [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	data = initialize_data(argc, argv);
	if (!data)
		return (ft_printf("Error initializing data\n"), 1);
	i = -1;
	while (++i < data->num_philosophers)
		pthread_create(&data->philosophers[i].thread,
			NULL, philosopher_routine, &data->philosophers[i]);
	monitor_philosophers(data);
	i = -1;
	while (++i < data->num_philosophers)
		pthread_join(data->philosophers[i].thread, NULL);
	cleanup(data);
	return (0);
}
