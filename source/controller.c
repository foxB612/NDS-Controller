// #define ARM9

#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>

typedef struct
{
    char host[64];
    unsigned short port;
} Address;

Address getAddress();
int connectWifi();
int connectSocket(Address address);
void controller();

void OnKeyPressed(int key)
{
   if(key > 0)
      iprintf("%c", key);
}

int main(void)
{
	consoleDemoInit();  //setup the sub screen for printing
    videoSetMode(MODE_0_2D);
    iprintf("NDS Controller\n");
    iprintf("by foxb612\n\n");

    int retry = 0;
    while (1)
    {
        if (!connectWifi())
        {
            iprintf("Press start to retry.\n");
            while (1)
            {
                swiWaitForVBlank();
                scanKeys();
                if (keysDown() & KEY_START) break;
            }
            iprintf("\nRetry[%d]\n", ++retry);
            sleep(1);
        }
        else break;
    }

    int socket;
    while (1)
    {
        Address address = getAddress();
        iprintf("The address is: %s:%d\n", address.host, address.port);
        socket = connectSocket(address);
        if (socket != -1) break;
        consoleClear();
        sleep(1);
    }
    
    controller(socket);

    sleep(1);
    Wifi_DisconnectAP();
    sleep(1);
    Wifi_DisableWifi();
    iprintf("You can shut down your nds now.");
    while (1) swiWaitForVBlank();
    return 0;
}

Address getAddress()
{
    Keyboard *kbd = keyboardDemoInit();
    kbd->OnKeyPressed = OnKeyPressed;

    iprintf("\nPlease input the host address.(Length: 0~64)\n");
    iprintf("Example: 192.168.0.1\n");
    Address address;
    keyboardShow();
    scanf("%s", address.host);

    iprintf("Please input the port.\n");
    iprintf("Example: 8080\n");
    scanf("%hd", &address.port);
    keyboardHide();

    return address;
}

int connectWifi()
{
    iprintf("\nConnecting via WFC data ...\n");

	if (!Wifi_InitDefault(WFC_CONNECT))
    {
		iprintf("Failed to connect!\n");
        return 0;
	}
    else
    {
        iprintf("Connected!\n");
        return 1;
    }
}

int connectSocket(Address address)
{
    char * url = address.host;
    unsigned short port = address.port;
    int retry = 0;
    while (1)
    {
        // Find the IP address of the server, with gethostbyname
        struct hostent * myhost = gethostbyname( url );
        iprintf("\nFound IP Address!\n");
    
        // Create a TCP socket
        int my_socket;
        my_socket = socket( AF_INET, SOCK_STREAM, 0 );
        iprintf("Created Socket!\n");

        // Tell the socket to connect to the IP address we found
        struct sockaddr_in sain;
        sain.sin_family = AF_INET;
        sain.sin_port = htons(port);
        sain.sin_addr.s_addr= *( (unsigned long *)(myhost->h_addr_list[0]) );
        int c = connect( my_socket,(struct sockaddr *)&sain, sizeof(sain) );
        if (c == 0)
        {
            iprintf("Connected to server!\n");
            return my_socket;
        }
        else
        {
            shutdown(my_socket,0);
            closesocket(my_socket);
            iprintf("Failed to connect to the server.\n");
            iprintf("Press A to retry, or press Start to reinput the address.\n");
            while (1)
            {
                swiWaitForVBlank();
                scanKeys();
                if (keysDown() & KEY_START)
                    return -1;
                else if (keysDown() & KEY_A) break;
            }
            iprintf("\nRetry[%d]\n", ++retry);
            sleep(1);
        }
    }
}

void controller(int socket)
{
    iprintf("\nYou can use this NDS as a controller now.\n");
    iprintf("Touch the screen to pause or continue.\n");
    iprintf("To stop, touch the screen while holding Start button.\n");

    int keys[2] = {0, 0};
    int size = 2 * sizeof(int);
    char flag = 1;
    while (1)
    {
        swiWaitForVBlank();
        scanKeys();
        keys[0] = keysDown();
        keys[1] = keysUp();
        if (keys[1] & KEY_TOUCH)
        {
            if (keysHeld() & KEY_START) break;
            flag = !flag;
            if (flag) iprintf("Continued.\n");
            else iprintf("Paused.\n");
            continue;
        }
        if (!flag || !(keys[0] || keys[1])) continue;
        // iprintf("%d %d\n", keys[0], keys[1]);
        send(socket, keys, size, 0);
    }

    iprintf("Connection closed!\n");
    shutdown(socket,0); // good practice to shutdown the socket.
    closesocket(socket); // remove the socket.
}
