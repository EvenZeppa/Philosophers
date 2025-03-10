/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-27 17:10:14 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-27 17:10:14 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnchr(const char *s, int c, size_t n)
{
	char	*ptr;

	ptr = (char *)s;
	while (n-- && *ptr && *ptr != (unsigned char)c)
		ptr++;
	if (*ptr != (unsigned char)c)
		return (NULL);
	return (ptr);
}
