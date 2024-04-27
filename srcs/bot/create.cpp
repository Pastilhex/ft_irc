/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:50:15 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/04/27 20:38:38 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

				
#include "../../includes/ircserv.hpp"

void Bot::create(Channel &channel, std::string bot_nick)
{
	Client *bot = new Client();
	if(bot_nick.empty())
	{
		bot->setNick("Bot");
		bot->setUsername("Bot");
	}
	else
	{
		bot->setNick(bot_nick);
		bot->setUsername(bot_nick);
	}
	bot->setRealName("Bot");
	channel.setNewUser(*bot);
	channel.setBot(bot);
}