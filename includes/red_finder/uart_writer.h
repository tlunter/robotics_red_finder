#ifndef __UART_WRITER
#define __UART_WRITER

int open_uart(void);
bool uart_write(int, char);
bool uart_read(int);

#endif
