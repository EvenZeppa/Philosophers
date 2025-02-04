/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:48:26 by ezeppa            #+#    #+#             */
/*   Updated: 2025/02/04 11:49:29 by ezeppa           ###   ########.fr       */
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
			ft_printf("%d %d died\n",
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
