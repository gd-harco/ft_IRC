# ft_IRC

## Introduction
This project involves the development of an IRC (Internet Relay Chat) server in C++98. The server is designed to handle multiple clients simultaneously, and all I/O operations must be non-blocking. The server uses a single poll (or equivalent) to manage read, write, listen, and other operations.

## Usage
Run the IRC server with the following command:
```shell
./ircserv <port> <password>
