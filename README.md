# NDS Controller

This is a NDS homebrew program which can turn a NDS into a wireless gamepad for Windows, Mac, Linux, etc.

### Requirements

- A Nintendo DS console to be used as a gamepad, which can connect to Wifi
- A flashcart (e.g. R4, DSTwo) which can let you run homebrew program on NDS
- A PC (or any device, if you write your own server program) to be connected by NDS
- A wireless network with Open or WEP encryption, for the NDS (and PC) to connect

### Preparation

1. Set up a wireless network. The security option must be `Open` or `WEP` otherwise it cannot be connected by NDS.

   Common solutions:

   - Use a wireless router
   - Set up a hotspot on your PC or phone

2. Configure the Wifi Settings of your NDS. Make sure your Wifi ssid and password (if any) is correct.

3. Make sure your PC's ip address can be found by your NDS. This is automatically achieved if you use PC's hotspot for NDS to connect. If not, you can connect your PC to the same wireless network with NDS.

### Usage

1. Run `NDS_Controller.nds`. It will automatically turn on Wifi and try connecting.
2. Run server program on your pc.
3. On NDS, input the ip address and server port (defined in server program)
4. If everything is OK, you should be able to use the NDS gamepad to input now.

The key map is also defined in server program.

### For developers

You can write you own server program to connect NDS to any device you want. In short it should be a socket server. Every time a key is pressed/released on NDS, the NDS will send a byte array with length of  `sizeof(int) * 2` via the socket connection. For the detailed meaning of the array you can take a look at the source code.