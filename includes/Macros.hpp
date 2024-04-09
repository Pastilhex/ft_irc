/**
 * @file Macros.hpp
 * @brief This file contains the macro definitions used in the project.
 */

#ifndef MACROS_HPP
#define MACROS_HPP

#define UNLIMITED_USERS -1

#define MODE_CHANNELMSG(channel, mode) (":localhost MODE #" + channel + " " + mode + "\r\n")
#define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

#endif