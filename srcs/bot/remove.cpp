/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:51:44 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/04/28 18:42:02 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::remove(Channel &channel)
{
    std::map<std::string, Client>::iterator bot;
    
    bot = channel.getUsers().find(channel.getBot()->getNick());
    std::cout << bot->first << std::endl;
    channel.getUsers().erase(bot);
}