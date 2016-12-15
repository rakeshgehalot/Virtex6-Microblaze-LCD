/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * 
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */


#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "gpio_header.h"
#include "uartlite_header.h"
#include "xsysace.h"
#include "sysace_header.h"
#include "xtmrctr.h"
#include "tmrctr_header.h"
#include "xstatus.h"



#define LCD_DB4    0x01
#define LCD_DB5    0x02
#define LCD_DB6    0x04
#define LCD_DB7    0x08
#define LCD_RW     0x10
#define LCD_RS     0x20
#define LCD_E      0x40
#define LCD_TEST   0x80



XGpio GpioOutput;
XTmrCtr DelayTimer;



void delay_us(Xuint32 time);
void delay_ms(Xuint32 time);
void gpio_write(Xuint32 c);

Xuint32 gpio_read(void);

void lcd_clk(void);
void lcd_set_test(void);
void lcd_reset_test(void);
void lcd_set_rs(void);
void lcd_reset_rs(void);
void lcd_set_rw(void);
void lcd_reset_rw(void);
void lcd_write(Xuint32 c);
void lcd_clear(void);
void lcd_puts(const char * s);
void lcd_putch(Xuint32 c);
void lcd_goto(Xuint32 line,Xuint32 pos);
void lcd_init(void);







int main() 
{


   Xil_ICacheEnable();
   Xil_DCacheEnable();

   print("---Entering main---\n\r");

   Xuint32 status;


    xil_printf("%c[2J",27);
    xil_printf("https://github.com/rakeshgehalot\r\n");
    xil_printf("simpletest");


    status = XTmrCtr_Initialize(&DelayTimer,
                                  XPAR_XPS_TIMER_0_DEVICE_ID);
    if (status != XST_SUCCESS){
      xil_printf("Timer failed to initialize\r\n");
      return XST_FAILURE;
    }
    XTmrCtr_SetOptions(&DelayTimer, 1, XTC_DOWN_COUNT_OPTION);


    status = XGpio_Initialize(&GpioOutput,
                                   XPAR_XPS_GPIO_0_DEVICE_ID);
    if (status != XST_SUCCESS){
      xil_printf("GPIO failed to initialize\r\n");
      return XST_FAILURE;
    }

    XGpio_SetDataDirection(&GpioOutput, 1, 0x00);


    lcd_init();


    lcd_puts("github.com/rakeshgehalot");
    lcd_goto(1,2);
    lcd_puts("Line @212");



   Xil_DCacheDisable();
   Xil_ICacheDisable();

   return 0;
}






void delay_us(Xuint32 time)
{
  XTmrCtr_SetResetValue(&DelayTimer, 1, time * 125);
  XTmrCtr_Start(&DelayTimer, 1);
  while(!(XTmrCtr_IsExpired(&DelayTimer, 1))){}
  XTmrCtr_Stop(&DelayTimer, 1);
}


void delay_ms(Xuint32 time)
{
  XTmrCtr_SetResetValue(&DelayTimer, 1, time * 125000);
  XTmrCtr_Start(&DelayTimer, 1);
  while(!(XTmrCtr_IsExpired(&DelayTimer, 1))){}
  XTmrCtr_Stop(&DelayTimer, 1);
}


void gpio_write(Xuint32 c)
{

  XGpio_DiscreteWrite(&GpioOutput, 1, c & 0x0FF);
}


Xuint32 gpio_read()
{

  return(XGpio_DiscreteRead(&GpioOutput, 1));
}


void lcd_clk()
{
  Xuint32 c;

  c = gpio_read();
  delay_us(1);

  gpio_write(c | LCD_E);
  delay_us(1);

  gpio_write(c & (~LCD_E));
  delay_us(1);
}


void lcd_set_rs()
{
  Xuint32 c;

  c = gpio_read();

  gpio_write(c | LCD_RS);
  delay_us(1);
}


void lcd_reset_rs()
{
  Xuint32 c;

  c = gpio_read();

  gpio_write(c & (~LCD_RS));
  delay_us(1);
}


void lcd_set_rw()
{
  Xuint32 c;

  c = gpio_read();

  gpio_write(c | LCD_RW);
  delay_us(1);
}


void lcd_reset_rw()
{
  Xuint32 c;

  c = gpio_read();

  gpio_write(c & (~LCD_RW));
  delay_us(1);
}


void lcd_write(Xuint32 c)
{
  Xuint32 temp;

  temp = gpio_read();
  temp = temp & 0xF0;

  temp = temp | ((c >> 4) & 0x0F);
  gpio_write(temp);

  lcd_clk();

  delay_us(100);

  temp = temp & 0xF0;
  temp = temp | (c & 0x0F);
  gpio_write(temp);

  lcd_clk();

  delay_us(100);
}

void lcd_clear(void)
{
  lcd_reset_rs();

  lcd_write(0x01);

  delay_ms(2);
}

void lcd_puts(const char * s)
{
  lcd_set_rs();
  while(*s)
    lcd_write(*s++);
}

void lcd_putch(Xuint32 c)
{
  lcd_set_rs();
  lcd_write(c);
}

void lcd_goto(Xuint32 line, Xuint32 pos)
{
  lcd_reset_rs();
  pos = pos & 0x3F;
  if(line == 0)
    lcd_write(0x80 | pos);
  else
    lcd_write(0xC0 | pos);
}

void lcd_init(void)
{
  Xuint32 temp;

  lcd_reset_rw();
  lcd_reset_rs();

  delay_ms(15);
  temp = gpio_read();
  temp = temp | LCD_DB5;
  gpio_write(temp);
  lcd_clk();
  lcd_clk();
  lcd_clk();
  delay_ms(15);
  lcd_write(0x28);
  lcd_write(0x0C);
  lcd_write(0x06);

  lcd_clear();
}

