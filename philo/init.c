/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:48:22 by ezeppa            #+#    #+#             */
/*   Updated: 2025/02/04 11:49:14 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	initialize_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		if (i % 2 == 0)
		{
			data->philosophers[i].left_fork = &data->forks[i];
			data->philosophers[i].right_fork
				= &data->forks[(i + 1) % data->num_philosophers];
		}
		else
		{
			data->philosophers[i].right_fork = &data->forks[i];
			data->philosophers[i].left_fork
				= &data->forks[(i + 1) % data->num_philosophers];
		}
		data->philosophers[i].data = data;
		i++;
	}
}

t_data	*initialize_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	data->stop_simulation = 0;
	data->start_time = get_timestamp();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
	if (!data->forks || !data->philosophers)
		return (cleanup(data), NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	initialize_forks(data);
	initialize_philosophers(data);
	return (data);
}
