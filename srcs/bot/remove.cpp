/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:51:44 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/04/27 20:42:49 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::remove(Channel &channel)
{
    std::map<std::string, Client>::iterator bot = channel.getUsers().find(channel.getBot()->getNick());
    std::cout << bot->first << std::endl;
    channel.getUsers().erase(bot);
}