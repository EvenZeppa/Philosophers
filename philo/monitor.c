/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:48:26 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/17 16:43:55 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor_check_die(t_data *data, t_philosopher *philo)
{
	if (get_timestamp() - philo->last_meal_time > data->time_to_die)
	{
		pthread_mutex_lock(&data->print_lock);
		if (!data->stop_simulation)
		{
			printf("%ld %d died\n",
				get_timestamp() - data->start_time, philo->id);
			data->stop_simulation = 1;
		}
		pthread_mutex_unlock(&data->print_lock);
	}
}

void	monitor_philosophers(t_data *data)
{
	t_philosopher	*philo;
	int				finished_philosophers;
	int				i;

	while (!data->stop_simulation)
	{
		finished_philosophers = 0;
		i = 0;
		while (i < data->num_philosophers)
		{
			philo = &data->philosophers[i];
			monitor_check_die(data, philo);
			if (data->meals_required > 0
				&& philo->meals_eaten >= data->meals_required)
				finished_philosophers++;
			i++;
		}
		if (data->meals_required > 0
			&& finished_philosophers == data->num_philosophers)
			data->stop_simulation = 1;
		usleep(500);
	}
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

int	ft_atoi(const char *nptr)
{
	int	nb;
	int	sign;

	nb = 0;
	sign = 1;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		nb = nb * 10 + (*nptr - '0');
		nptr++;
	}
	return (sign * nb);
}
