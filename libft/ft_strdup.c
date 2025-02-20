/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:24:19 by pviegas-          #+#    #+#             */
/*   Updated: 2025/02/20 13:53:16 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		length;
	int		i;

	i = 0;
	length = ft_strlen(s);
	str = malloc((length + 1) * sizeof(char));
	if (str == (void *)0)
		return ((void *)0);
	if (str != (void *)0)
	{
		while (s[i] != '\0')
		{
			str[i] = s[i];
			i++;
		}
	}
	str[i] = '\0';
	return (str);
}

//if (s == (void *)0)
	//	return ((void *)0);
