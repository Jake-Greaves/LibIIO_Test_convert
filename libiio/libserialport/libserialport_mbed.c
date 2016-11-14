/*
 * Libserialport-fake
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include <drivers/uart/adi_uart.h>

#include "libserialport.h"

uint8_t                 UartDeviceMem[UART_MEMORY_SIZE];//UART memory size

struct sp_port {
    ADI_UART_HANDLE uart;
    ADI_UART_WORDLEN data_bits;
    ADI_UART_STOPBITS stop_bits;
    ADI_UART_PARITY parity;
    char *name, *description;
};

static struct sp_port sp_ports[5];

static void init_port(struct sp_port *port,
        char *name, char *description)
{
    port->data_bits = ADI_UART_WORDLEN_8BITS;
    port->stop_bits = ADI_UART_ONE_STOPBIT;
    port->parity = ADI_UART_NO_PARITY;
    port->name = name;
    port->description = description;
    
  //open Uart and initialize handle
  adi_uart_Open(UART_DEVICE_NUM, ADI_UART_DIR_BIDIRECTION,
                UartDeviceMem,
                UART_MEMORY_SIZE,
                &port->uart);
  
  //configure UART device with NO-PARITY, ONE STOP BIT and 8bit word length. 
  adi_uart_SetConfiguration(&port->uart,
                            port->parity,
                            port->stop_bits,
                            port->data_bits);
}

enum sp_return sp_get_port_by_name(const char *portname,
        struct sp_port **port_ptr)
{
  //if initializing UART pins connected to Arduino interface
    if (!strcmp(portname, "UART0")) {
        //init UART struct
        init_port(&sp_ports[0], "UART0", "Arduino UART");
        *port_ptr = &sp_ports[0];
        return SP_OK;
    }
    
    //if not UART0
    return SP_ERR_ARG;
}

void sp_free_port(struct sp_port *port)
{
  //assumed to mean close device?
    adi_uart_Close(&port->uart);
}

enum sp_return sp_open(struct sp_port *port, enum sp_mode flags)
{
  //check if UART if transmitting
    if (flags != SP_MODE_READ_WRITE)
        return SP_ERR_SUPP;
    else
        return SP_OK;
}

enum sp_return sp_close(struct sp_port *port)
{
  //nothing?
    return SP_OK;
}

enum sp_return sp_set_baudrate(struct sp_port *port, int baudrate)
{
  if (baudrate == 115200) 
  {    
    //set glue baudrate. Only 115200 required for now
    adi_uart_ConfigBaudRate(&port->uart,
                          UART_DIV_C_115200,
                          UART_DIV_M_115200,
                          UART_DIV_N_115200,
                          UART_OSR_115200);
    return SP_OK;
    }
  
    //if not matched case
    return SP_ERR_ARG;
}

enum sp_return sp_set_bits(struct sp_port *port, int bits)
{
    port->data_bits = (ADI_UART_WORDLEN)bits;
    
    adi_uart_SetConfiguration(&port->uart,
                            port->parity,
                            port->stop_bits,
                            port->data_bits);
    
    return SP_OK;
}

enum sp_return sp_set_stopbits(struct sp_port *port, int stopbits)
{
    port->stop_bits = (ADI_UART_STOPBITS)stopbits;
    adi_uart_SetConfiguration(&port->uart,
                            port->parity,
                            port->stop_bits,
                            port->data_bits);
    return SP_OK;
}

enum sp_return sp_set_parity(struct sp_port *port, enum sp_parity parity)
{
  //parity switch
    switch (parity) {
    case SP_PARITY_NONE:
        port->parity = ADI_UART_NO_PARITY;
        break;
    case SP_PARITY_ODD:
        port->parity = ADI_UART_ODD_PARITY;
        break;
    case SP_PARITY_EVEN:
        port->parity = ADI_UART_EVEN_PARITY;
        break;
    default:
        return SP_ERR_SUPP;
    }

    //update config
    adi_uart_SetConfiguration(&port->uart,
                            port->parity,
                            port->stop_bits,
                            port->data_bits);
    
    return SP_OK;
}

enum sp_return sp_set_flowcontrol(
        struct sp_port *port, enum sp_flowcontrol flow)
{
  //no flow control implemented
    if (flow != SP_FLOWCONTROL_NONE)
        return SP_ERR_SUPP;

    return SP_OK;
}

enum sp_return sp_flush(struct sp_port *port, enum sp_buffer buffers)
{
    if (buffers == SP_BUF_BOTH)
        serial_clear(&port->uart);
    return SP_OK;
}

char *sp_get_port_name(const struct sp_port *port)
{
  //return name
    return port->name;
}

char *sp_get_port_description(const struct sp_port *port)
{
  //return description
    return port->description;
}

enum sp_return sp_blocking_write(struct sp_port *port, const void *buf,
        size_t count, unsigned int timeout_ms)
{
    adi_uart_Write(&port->uart, (void*)buf, count);

    return count;
}

enum sp_return sp_blocking_read_next(struct sp_port *port, void *buf,
        size_t count, unsigned int timeout_ms)
{
    if (count == 0)
        return SP_ERR_ARG;

    adi_uart_Read(&port->uart, buf, count);
    
    return 1;
}

int sp_last_error_code(void)
{
    return 42;
}