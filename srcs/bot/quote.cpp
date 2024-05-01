/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:34:02 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/01 14:32:17 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

#define BOT_NICKNAME this->getNick()

void Bot::quote(Server server, Channel &channel, Client &client)
{
	std::vector<std::string> quotes;
	quotes.push_back("Success is the sum of small efforts repeated day in and day out. - Robert Collier\r\n");
	quotes.push_back("The best way to predict the future is to create it. - Peter Drucker\r\n");
	quotes.push_back("The only place where success comes before work is in the dictionary. - Vidal Sassoon\r\n");
	quotes.push_back("Persistence is the path to success. - Charlie Chaplin\r\n");
	quotes.push_back("Talent wins games, but teamwork and intelligence win championships. - Michael Jordan\r\n");
	quotes.push_back("The only way to do great work is to love what you do. - Steve Jobs\r\n");
	quotes.push_back("No matter how many times you fail, you must be determined to succeed. You must dedicate yourself to your goal. - Vince Lombardi\r\n");
	quotes.push_back("The secret to getting ahead is getting started. - Mark Twain\r\n");
	quotes.push_back("It's not about how much time you have, but what you do with it. - Lou Holtz\r\n");
	quotes.push_back("The greatest pleasure in life is doing what people say you cannot do. - Walter Bagehot\r\n");
	quotes.push_back("Believe in yourself and all that you are. Know that there is something inside you that is greater than any obstacle. - Christian D. Larson\r\n");
	quotes.push_back("It does not matter how slowly you go, as long as you do not stop. - Confucius\r\n");
	quotes.push_back("You are never too old to set another goal or to dream a new dream. - C.S. Lewis\r\n");
	quotes.push_back("The only way to do great work is to love what you do. - Steve Jobs\r\n");
	quotes.push_back("Nothing is impossible, the word itself says 'I'm possible'! - Audrey Hepburn\r\n");
	quotes.push_back("Life is 10% what happens to me and 90% of how I react to it. - Charles Swindoll\r\n");
	quotes.push_back("Success is not the key to happiness. Happiness is the key to success. If you love what you are doing, you will be successful. - Albert Schweitzer\r\n");
	quotes.push_back("The greater the obstacle, the more glory in overcoming it. - Molière\r\n");
	quotes.push_back("Our greatest weakness lies in giving up. The most certain way to succeed is always to try just one more time. - Thomas Edison\r\n");
	quotes.push_back("The only place where success comes before work is in the dictionary. - Donald Kendall\r\n");
	quotes.push_back("Failure is simply the opportunity to begin again, this time more intelligently. - Henry Ford\r\n");
	quotes.push_back("The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt\r\n");
	quotes.push_back("You don't fail until you give up. - Gordon B. Hinckley\r\n");
	quotes.push_back("Success is going from failure to failure without losing your enthusiasm. - Winston Churchill\r\n");
	quotes.push_back("The only limit to our realization of tomorrow will be our doubts of today. - Franklin D. Roosevelt\r\n");
	quotes.push_back("You may never know what results come of your actions, but if you do nothing, there will be no results. - Mahatma Gandhi\r\n");
	quotes.push_back("To succeed, you need to find something to hold on to, something to motivate you, something to inspire you. - Tony Dorsett\r\n");
	quotes.push_back("The real secret of success is enthusiasm. - Walter Chrysler\r\n");
	quotes.push_back("The only place where success comes before work is in the dictionary. - Vidal Sassoon\r\n");
	quotes.push_back("Success is the sum of small efforts repeated day in and day out. - Robert Collier\r\n");


	std::srand(std::time(0));
	int randomIndex = std::rand() % quotes.size();
	std::string randomQuote = quotes[randomIndex];
	SEND(client.getSocket(), ":Marvin!" + client.getUsername() + "@" + server.getHostname() + " PRIVMSG " + channel.getName() + " :" + randomQuote, "Error sending JOIN message to client.");
}
