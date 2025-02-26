/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_are_characters_check.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:19 by scarlos-          #+#    #+#             */
/*   Updated: 2024/12/09 11:21:21 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

int	arguments_outside_limits(char *str)
{
	if (ft_atoi_long(str) > INT_MAX || ft_atoi_long(str) < INT_MIN)
		return (TRUE);
	return (FALSE);
}

int	no_arguments_outside_limits(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (arguments_outside_limits(argv[i]))
			return (FALSE);
	}
	return (TRUE);
}

int	arguments_are_correct(int argc, char **argv)
{
	if (right_amount_of_arguments(argc) && all_arguments_are_numbers(argv))
	{
		if (no_arguments_outside_limits(argv))
			return (TRUE);
		return (print_limits_error(), FALSE);
	}
	return (print_right_syntax(), FALSE);
}

int	right_amount_of_arguments(int argc)
{
	if (argc < 5 || argc > 6)
		return (FALSE);
	else
		return (TRUE);
}
