/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_are_numbers_check.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:24:12 by scarlos-          #+#    #+#             */
/*   Updated: 2024/12/09 11:22:27 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

int	is_numeric(int count)
{
	if (count >= '0' && count <= '9')
		return (TRUE);
	else
		return (FALSE);
}

int	string_is_numeric(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (is_numeric(str[i]) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

int	all_arguments_are_numbers(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (string_is_numeric(argv[i]))
			;
		else
			return (FALSE);
	}
	return (TRUE);
}
