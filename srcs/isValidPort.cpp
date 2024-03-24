/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidPort.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 13:42:55 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

bool	isValidPort(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}