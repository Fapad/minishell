/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szilas <szilas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 17:47:41 by bszilas           #+#    #+#             */
/*   Updated: 2024/06/20 12:50:12 by szilas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	fill_temp_arr(char *s, long long n)
{
	int	i;
	int	negative;

	negative = n >> ((sizeof (long long) * BYTE) - 1) & 1;
	if (negative)
		n *= -1;
	i = 11;
	s[i] = 0;
	if (n == 0)
		s[--i] = '0';
	while (n > 0)
	{
		s[--i] = n % 10 + '0';
		n /= 10;
	}
	if (negative)
		s[--i] = '-';
	return (i);
}

char	*ft_itoa(long long n)
{
	char	*ret;
	int		i;
	int		max_siz;
	char	temp_arr[12];

	max_siz = 12;
	i = fill_temp_arr(temp_arr, n);
	ret = malloc((max_siz - i) * sizeof (char));
	if (!ret)
		return (NULL);
	ft_memcpy((void *)ret, (const void *)temp_arr + i, max_siz - i);
	return (ret);
}
/*
int main(int argc, char const *argv[])
{
	char	*n = ft_itoa(atoi(argv[1]));
	for (size_t i = 0; i <= strlen(n); i++)
		printf("[%d] = %c\n", i, n[i]);	
	free(n);
	return 0;
}
*/