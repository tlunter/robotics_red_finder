#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int open_uart(void)
{
    //-------------------------
    //----- SETUP USART 0 -----
    //-------------------------
    //At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
    int uart0_filestream = -1;
    
    //OPEN THE UART
    //The flags (defined in fcntl.h):
    //  Access modes (use 1 of these):
    //      O_RDONLY - Open for reading only.
    //      O_RDWR - Open for reading and writing.
    //      O_WRONLY - Open for writing only.
    //
    //  O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
    //                                          if there is no input immediately available (instead of blocking). Likewise, write requests can also return
    //                                          immediately with a failure status if the output can't be written immediately.
    //
    //  O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
    uart0_filestream = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        //ERROR - CAN'T OPEN SERIAL PORT
        std::cout << "Error - Unable to open UART.  Ensure it is not in use by another application" << std::endl;
    }
    
    //CONFIGURE THE UART
    //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
    //  Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //  CSIZE:- CS5, CS6, CS7, CS8
    //  CLOCAL - Ignore modem status lines
    //  CREAD - Enable receiver
    //  IGNPAR = Ignore characters with parity errors
    //  ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //  PARENB - Parity enable
    //  PARODD - Odd parity (else even)
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // 8N1
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // no flow control
    options.c_cflag &= ~CRTSCTS;

    options.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    options.c_oflag &= ~OPOST; // make raw

    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 0;

    //options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    //options.c_iflag = IGNPAR;
    //options.c_oflag = 0;
    //options.c_lflag = 0;
    tcsetattr(uart0_filestream, TCSANOW, &options);
    if(tcsetattr(uart0_filestream, TCSAFLUSH, &options) < 0) {
        std::cout << "Couldn't set UART attributes" << std::endl;
        return -1;
    }


    return uart0_filestream;
}

bool uart_write(int fd, char value)
{
    if (fd != -1)
    {
        int count = write(fd, &value, 1);
        if (count < 0)
        {
            return false;
        }
    }
    return true;
}

bool uart_read(int fd)
{
    unsigned char rx_buffer[256];
    int rx_length = read(fd, (void*)rx_buffer, 255);
    if (rx_length < 0)
    {
        return false;
    }
    else if (rx_length > 0)
    {
        //rx_buffer[rx_length] = '\0';
        //std::cout << rx_buffer << std::endl;
    }
    return true;
}
