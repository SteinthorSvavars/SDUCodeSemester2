
// task 2, EMB1 assignment 10

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"

typedef struct { // define structure
  int Re;
  int Im;
} complex_t; 

void read_complex(complex_t *com_number)
{
  printf("Type in real part of number:");
  scanf("%d",&(com_number->Re));
  printf("Type in imaginary part of number:");
  scanf("%d",&(com_number->Im));
}

complex_t add_complex(complex_t n1, complex_t n2)
{
  complex_t result;
  result.Re=n1.Re+n2.Re;
  result.Im=n1.Im+n2.Im;
  return result;
}

complex_t mul_complex(complex_t n1, complex_t n2)
{
  complex_t result;
  result.Re=n1.Re*n2.Re-n1.Im*n2.Im;
  result.Im=n1.Re*n2.Im+n2.Re*n1.Im;
  return result;
}


int main(void) {  

  unsigned char i; // initiate variables

  uart_init();
  io_redirect();

  complex_t ar[2],result;

  while(1) // start program loop
  {
    for(i=0;i<2;i++) //loop for reading complex number, first real, then imaginary
    {
      printf("Number %hhu\n", i+1);
      read_complex(&ar[i]);
    }
    for(i=0;i<2;i++) //loop for printing complex number
    {
      printf("Real part of number %d: %d\n", i+1,ar[i].Re);
      printf("Imaginary part of number %d: %d\n", i+1,ar[i].Im);
    }
    result=add_complex(ar[0],ar[1]);
    printf("Real part of number addition: %d\n", result.Re);
    printf("Imaginary part of number adition: %d\n", result.Im);
    result=mul_complex(ar[0],ar[1]);
    printf("Real part of number multiplication: %d\n", result.Re);
    printf("Imaginary part of number multiplication: %d\n", result.Im);
  }
  return 0; //end program
}
