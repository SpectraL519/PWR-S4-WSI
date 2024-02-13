#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "game.h"
#include "minmax.h"



int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP the same as server-side
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Send connection request to server
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    char server_message[16], client_message[16];
    // Receive the server message
    memset(server_message, '\0', sizeof(server_message));
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's message\n");
        return -1;
    }
    printf("Server message: %s\n",server_message);

    // Send the message to server
    memset(client_message, '\0', sizeof(client_message));
    strcpy(client_message, argv[3]);
    if (send(socket_desc, client_message, strlen(client_message), 0) < 0) {
        printf("Unable to send message\n");
        return -1;
    }

    init_board(); 
    bool game_finished = false;
    int player = atoi(argv[3]);
    int depth = atoi(argv[4]);
    if (!set_game_params(player, depth)) 
        return -1;

    while (!game_finished) {
        memset(server_message, '\0', sizeof(server_message));
        if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
            printf("Error while receiving server's message\n");
            return -1;
        }
        printf("Server message: %s\n", server_message);

        int msg = atoi(server_message);
        int move = msg % 100;
        msg /= 100;

        if (move != 0) {
            make_move(move, 3 - player);
            show_board();
        }

        if ((msg == 0) || (msg == 6)) {
            move = bot_movement();
            show_board();

            memset(client_message, '\0', sizeof(client_message));
            sprintf(client_message, "%d", move);
            if (send(socket_desc, client_message, strlen(client_message), 0) < 0) {
                printf("Unable to send message\n");
                return -1;
            }
            printf("Client message: %s\n", client_message);
        }
        else {
            game_finished = true;
            switch (msg) {
                case 1 : { printf("You won.\n"); break; }
                case 2 : { printf("You lost.\n"); break; }
                case 3 : { printf("Draw.\n"); break; }
                case 4 : { printf("You won. Opponent error.\n"); break; }
                case 5 : { printf("You lost. Your error.\n"); break; }
            }
        }
    }

    // Close socket
    close(socket_desc);

    return 0;
}
