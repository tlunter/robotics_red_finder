#ifndef __UART_WRITER
#define __UART_WRITER

int open_uart(void);
void uart_write(int, char);
void uart_read(int);

#endif
