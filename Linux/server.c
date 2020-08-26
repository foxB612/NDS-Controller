// Server side C/C++ program to demonstrate Socket programming
#include <xdo.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define BIT(n) (1 << (n))

char * KEY[14] = {
    "x",        // KEY_A
    "z",        // KEY_B
    "v",        // KEY_SELECT
    "space",    // KEY_START
    "Right",    // KEY_RIGHT
    "Left",     // KEY_LEFT
    "Up",       // KEY_UP
    "Down",     // KEY_DOWN
    "w",        // KEY_R
    "q",        // KEY_L
    "s",        // KEY_X
    "a",        // KEY_Y
    "",         // KEY_TOUCH
    ""          // KEY_LID
};

int main(int argc, char const *argv[]) 
{ 
    int PORT = 8080;

    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                    (socklen_t*)&addrlen))<0) 
    {
        perror("accept"); 
        exit(EXIT_FAILURE);
    } 

    xdo_t * x = xdo_new(NULL);
    int buffer[2] = {0};
    while (1)
    {
        valread = read(new_socket , buffer, 2 * sizeof(int));
        if (valread <= 0)
            break;
        if (buffer[0] || buffer[1])
            printf("\033[2K");
            printf("%d %d\n", buffer[0], buffer[1]);
            printf("\033[A");
        if (buffer[0] != 0)
        {
            // keyDown
            for (int i = 0; i < 14; i++)
            {
                if (buffer[0] & BIT(i))
                    xdo_send_keysequence_window_down(x, CURRENTWINDOW, KEY[i], 0);
            }
        }
        if (buffer[1] != 0)
        {
            // keyUp
            for (int i = 0; i < 14; i++)
            {
                if (buffer[1] & BIT(i))
                    xdo_send_keysequence_window_up(x, CURRENTWINDOW, KEY[i], 0);
            }
        }
    }
    // keyUp
    for (int i = 0; i < 14; i++)
    {
        xdo_send_keysequence_window_up(x, CURRENTWINDOW, KEY[i], 0);
    }
    printf("End\n");
    shutdown(server_fd, 0);
    
    return 0; 
} 
