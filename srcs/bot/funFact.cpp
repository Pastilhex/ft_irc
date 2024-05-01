/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funFact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:27:02 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/05/01 16:39:43 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

#define BOT_NICKNAME this->getNick()

void Bot::funFact(Server server, Channel &channel, Client &client)
{
    std::vector<std::string> funFacts;
    funFacts.push_back("Bananas are berries, but strawberries aren't.");
    funFacts.push_back("Octopuses have three hearts.");
    funFacts.push_back("The shortest war in history was between Britain and Zanzibar on August 27, 1896. Zanzibar surrendered after 38 minutes.");
    funFacts.push_back("A group of flamingos is called a flamboyance.");
    funFacts.push_back("The national animal of Scotland is the unicorn.");
    funFacts.push_back("Honey never spoils. Archaeologists have found pots of honey in ancient Egyptian tombs that are over 3,000 years old and still perfectly edible.");
    funFacts.push_back("Cows have best friends and can become stressed when separated from them.");
    funFacts.push_back("The shortest war in history was between Britain and Zanzibar on August 27, 1896. Zanzibar surrendered after 38 minutes.");
    funFacts.push_back("Penguins can jump up to 6 feet in the air.");
    funFacts.push_back("The first oranges weren't orange.");
    funFacts.push_back("Rats laugh when tickled.");
    funFacts.push_back("There are more possible iterations of a game of chess than there are atoms in the known universe.");
    funFacts.push_back("Vending machines are twice as likely to kill you as a shark is.");
    funFacts.push_back("Coca-Cola would be green if coloring wasn't added to it.");
    funFacts.push_back("An ostrich's eye is bigger than its brain.");
    funFacts.push_back("The Eiffel Tower can be 15 cm taller during the summer, due to thermal expansion of the iron.");
    funFacts.push_back("A cat's brain is 90% similar to a human's.");
    funFacts.push_back("The national animal of Scotland is the unicorn.");
    funFacts.push_back("A 'jiffy' is an actual unit of time: 1/100th of a second.");
    funFacts.push_back("The first alarm clock could only ring at 4 a.m.");
    funFacts.push_back("The total weight of all the ants on Earth is approximately equal to the total weight of all the humans on Earth.");
    funFacts.push_back("Oxford University is older than the Aztec Empire.");
    funFacts.push_back("The lighter was invented before the match.");
    funFacts.push_back("The average person spends 6 months of their lifetime waiting for a red light to turn green.");

    std::srand(std::time(0));
    int randomIndex = std::rand() % funFacts.size();
    std::string randomFact = funFacts[randomIndex];
    SEND(client.getSocket(), ":Marvin!" + client.getUsername() + "@" + server.getHostname() + " PRIVMSG " + channel.getName() + " :" + randomFact + "\r\n", "Error sending JOIN message to client.");
}
