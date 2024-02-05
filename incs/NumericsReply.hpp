//
// Created by zorkz on 05/02/24.
//

#ifndef NUMERICSREPLY_HPP
#define NUMERICSREPLY_HPP


// NAMES
# define RPL_NAMREPLY(client, channel, list_of_nicks) (":localhost 353 " + client + " " + " #" + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")

// JOIN
# define RPL_JOIN(user, channel) (user + " JOIN :#" +  channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

#endif //NUMERICSREPLY_HPP
