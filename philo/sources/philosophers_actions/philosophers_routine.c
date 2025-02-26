/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:51 by scarlos-          #+#    #+#             */
/*   Updated: 2025/02/05 12:05:56 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

int	check_loop(void *arguments)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)arguments;
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->general->philosopher_dead == 0 && \
		philosophers->number_of_meals != 0)
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	return (0);
}

void	*philosophers_routine(void *arguments)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)arguments;
	if (philosophers->id % 2 == 0)
		ft_sleep(2, philosophers);
	while (check_loop(philosophers))
	{
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosopher_takes_forks(philosophers);
		philosophers_is_eating(philosophers);
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosophers_is_sleeping(philosophers);
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosophers_is_thinking(philosophers);
		usleep(philosophers->general->time_to_eat);
	}
	return (NULL);
}

void	join_threads(t_general *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}

void	begin_philosophers_routine(t_general *data)
{
	int	i;

	i = 0;
	data->starting_time = get_time();
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].thread_id,
			NULL, &philosophers_routine, (void *)&data->philosophers[i]);
		i++;
	}
}
