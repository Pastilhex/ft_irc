#include "../includes/ircserv.hpp"


void Server::MODE(std::string message, Client client) {
	(void)client;
	std::vector<std::string> mode_cmd = Utils::split(message, " \n\t\r");
	std::string channel_name = mode_cmd[1].substr(1);
	std::map<std::string, Channel>::iterator it = getChannels().find(channel_name);
	std::vector<std::string> operators = it->second.getOperators();

	if (mode_cmd.size() < 3) {
		return (Utils::logMessage("Not enough arguments", 1), void());
	}

	if (!Utils::isValidUser(it->second, client.getNick()) && Utils::isOperator(it->second, client.getNick())) {
		return (Utils::logMessage("You are not an operator in this channel", 1), void());
	}

	if (channel_name.empty() || it == getChannels().end()) {
		return (Utils::logMessage("Channel not found", 1), void());
	}

	std::string mode = mode_cmd[2];
	if (mode == "+o") {
		handleOperatorMode(mode_cmd, it);
	} else if (mode == "+i") {
		handlePrivateAccessMode(it);
	} else if (mode == "+t") {
		handleRestrictedTopicMode(it);
	} else if (mode == "+k") {
		handlePasswordMode(mode_cmd, it);
	} else if (mode == "+l") {
		handleUserLimitMode(mode_cmd, it);
	} else {
		Utils::logMessage("Invalid mode flag", 1);
	}
}

void Server::handleOperatorMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it) {
	if (mode_cmd.size() != 4) {
		return (Utils::logMessage("Invalid number of arguments", 1), void());
	} else if (!Utils::isValidUser(it->second, mode_cmd[3])) {
		return (Utils::logMessage("Invalid user", 1), void());
	} else if (mode_cmd[3] == it->second.getName()) {
		return (Utils::logMessage("You can't set yourself as operator", 1), void());
	} else if (Utils::isOperator(it->second, mode_cmd[3])) {
		return (Utils::logMessage("User is already an operator", 1), void());
	} else {
		it->second.AddOperator(it->second.getUsers().find(mode_cmd[3])->first);
		return (Utils::logMessage("User is now an operator", 0), void());
	}
}

void Server::handlePrivateAccessMode(std::map<std::string, Channel>::iterator it) {
	if (it->second.getModePrivateAccess()) {
		return (Utils::logMessage("Channel is already private", 0), void());
	}
	it->second.setModePrivateAccess(true);
	return (Utils::logMessage("Channel is now private", 0), void());
}

void Server::handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it) {
	if (it->second.getRestrictedTopic()) {
		return (Utils::logMessage("Channel topic is already restricted", 0), void());
	}
	it->second.setRestrictedTopic(true);
	return (Utils::logMessage("Channel topic is now restricted", 0), void());
}

void Server::handlePasswordMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it) {
	if (mode_cmd.size() >= 4) {
		std::string password = mode_cmd[3];
		it->second.setPassword(password);
		return (Utils::logMessage("Channel key is now set", 0), void());
	}
}

void Server::handleUserLimitMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it) {
	if (mode_cmd.size() >= 4) {
		try {
			double userLimit = atof(mode_cmd[3].c_str());
			int roundedLimit = static_cast<int>(userLimit);
			it->second.setUserLimit(roundedLimit);
			return (Utils::logMessage("User limit is now set", 0), void());
		} catch (const std::exception& e) {
			return (Utils::logMessage("Removing user limit", 1), void());
			it->second.setUserLimit(UNLIMITED_USERS);
		}
	}
}

