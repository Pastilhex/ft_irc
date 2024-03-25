/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getAddressInfo.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:48:09 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 19:24:08 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void	ft_getAddressInfo(void)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        cerr << "Erro ao obter o nome do host." << endl;
        return ;
    }

    struct hostent *host_info = gethostbyname(hostname);
    if (host_info == NULL || host_info->h_addr_list[0] == NULL) {
        cerr << "Erro ao obter o endereço IP." << endl;
        return ;
    }

    char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));
    cout << "Endereço IP local do servidor: " << ip_address << endl;
}