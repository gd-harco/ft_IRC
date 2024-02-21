#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstdlib>

#define DELIMITER "\r\n"

int main(int argc, char **argv)
{

    if (argc != 4)
    {
        std::cout << "Usage: ./ircbot <ip> <port> <password>" << std::endl;
        return 1;
    }
    struct addrin
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        return 1;
    }
    std::string password = argv[3];

    sockaddr_in server_addr;
    uint64_t port = std::strtol(argv[1], NULL, 10);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    inet_pton(AF_INET, argv[2], &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        return 1;
    }

    return 0;
    std::string nickname = "MonBot";
    std::string username = "mon_bot";
    std::string realname = "Mon Bot";

    std::string connection_message = "PASS" + password + DELIMITER;
    std::string nick_message = "NICK " + nickname + "\r\n";
    std::string user_message = "USER " + username + " 0 * :" + realname + "\r\n";
    
    send(sockfd, connection_message.c_str(), connection_message.size(), 0);
    send(sockfd, nick_message.c_str(), nick_message.size(), 0);
    send(sockfd, user_message.c_str(), user_message.size(), 0);

    char buffer[1024];
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
        std::cerr << "Erreur lors de la réception des données du serveur." << std::endl;
    else
        std::cout << "Réponse du serveur : " << std::string(buffer, bytes_received) << std::endl;

    close(sockfd);
    return 0;
}