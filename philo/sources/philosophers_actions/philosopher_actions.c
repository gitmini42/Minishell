/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:41 by scarlos-          #+#    #+#             */
/*   Updated: 2025/02/05 11:57:47 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

int	philosopher_takes_forks(t_philo *philosophers)
{
	if (philosophers->id % 2 == 0)
		pthread_mutex_lock(philosophers->right_fork);
	else
		pthread_mutex_lock(philosophers->left_fork);
	if (!philosopher_is_dead(philosophers))
		print_message("has taken a fork", philosophers);
	if (philosophers->id % 2 == 0)
	{
		if (pthread_mutex_lock(philosophers->left_fork) != 0)
			return (pthread_mutex_unlock(philosophers->right_fork), 1);
		if (!philosopher_is_dead(philosophers))
			print_message("has taken a fork", philosophers);
	}
	else
	{
		if (pthread_mutex_lock(philosophers->right_fork) != 0)
			return (pthread_mutex_unlock(philosophers->left_fork), 1);
		if (!philosopher_is_dead(philosophers))
			print_message("has taken a fork", philosophers);
	}
	return (0);
}

void	philosophers_is_eating(t_philo *philosophers)
{
	print_message("is eating", philosophers);
	pthread_mutex_lock(&philosophers->general->mutex);
	philosophers->last_meal = get_time();
	philosophers->time_to_die = (get_time() \
		- philosophers->general->starting_time) \
			+ philosophers->general->time_to_die;
	if (philosophers->number_of_meals != -1)
		philosophers->number_of_meals--;
	pthread_mutex_unlock(&(philosophers)->general->mutex);
	ft_sleep(philosophers->general->time_to_eat, philosophers);
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}

void	philosophers_is_sleeping(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->general->mutex);
	if (check_meals_completed(philosophers->general))
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return ;
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	print_message("is sleeping", philosophers);
	ft_sleep(philosophers->general->time_to_sleep, philosophers);
}

void	philosophers_is_thinking(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->general->mutex);
	if (check_meals_completed(philosophers->general))
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return ;
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	print_message("is thinking", philosophers);
}

int	philosopher_is_dead(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->general->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	return (0);
}
