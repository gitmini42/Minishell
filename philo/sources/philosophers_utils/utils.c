/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:27 by scarlos-          #+#    #+#             */
/*   Updated: 2024/12/09 11:28:08 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

void	print_limits_error(void)
{
	printf("Wrong arguments:\n\
Should be above the int limits!\n");
}

void	print_right_syntax(void)
{
	printf("Wrong syntax or negative numbers. Try:\
\n./philo [philos] [death_time] [eating_time] [sleeping_time]\n");
}

int	ft_atoi(const char *str)
{
	int	is_atoi;
	int	is_negative;
	int	i;

	is_atoi = 0;
	is_negative = 1;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == '+' || str[i] == '-')
		{
			if (str[i] == '-')
				is_negative = -1;
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			is_atoi = (is_atoi * 10) + str[i] - 48;
			i++;
		}
		return (is_atoi * is_negative);
	}
	return (0);
}

long	ft_atoi_long(const char *str)
{
	long	long_atoi;
	long	is_negative;
	int		i;

	long_atoi = 0;
	is_negative = 1;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == '+' || str[i] == '-')
		{
			if (str[i] == '-')
				is_negative = -1;
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			long_atoi = (long_atoi * 10) + str[i] - 48;
			i++;
		}
		return (long_atoi * is_negative);
	}
	return (0);
}
