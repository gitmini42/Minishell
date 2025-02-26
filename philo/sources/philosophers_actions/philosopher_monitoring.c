/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_monitoring.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:46 by scarlos-          #+#    #+#             */
/*   Updated: 2025/02/05 12:03:10 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

int	check_last_meal(t_philo *philosophers)
{
	if (get_time() - philosophers->general->starting_time
		> philosophers->time_to_die)
	{
		philosophers->general->philosopher_dead = 1;
		printf("%d %d \033[31mdied\n\033[0m", get_time()
			- philosophers->general->starting_time, philosophers->id);
		return (0);
	}
	else
		return (1);
}

int	check_if_one_is_dead(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->number_of_philosophers)
	{
		if (check_last_meal(&general->philosophers[i]) == ERROR)
			return (ERROR);
		i++;
	}
	return (1);
}

void	free_philosophers(t_general *philosophers)
{
	free(philosophers->philosophers);
	free(philosophers->fork_mutex);
}

int	check_meals_completed(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->number_of_philosophers)
	{
		if (general->philosophers[i].number_of_meals != 0)
			return (ERROR);
		i++;
	}
	general->philosopher_dead = 1;
	return (SUCCESS);
}

void	*begin_monitoring(void *arg)
{
	t_general	*data;

	data = (t_general *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (check_if_one_is_dead(data) == ERROR \
			|| check_meals_completed(data) == SUCCESS)
		{
			return (pthread_mutex_unlock(&data->mutex), NULL);
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(10);
	}
}
