/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:32:33 by bszilas           #+#    #+#             */
/*   Updated: 2024/05/22 18:34:43 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	char			*ret;
	unsigned char	ch;

	ch = c;
	i = 0;
	if (!s)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		if (s[i] == ch)
		{
			ret = (char *)&s[i];
			return (ret);
		}
		i++;
	}
	return (NULL);
}
