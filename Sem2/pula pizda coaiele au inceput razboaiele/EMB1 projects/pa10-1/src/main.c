
// task 1, EMB1 assignment 10

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"

typedef struct { // define structure
  char name[10];
  unsigned char age;
  unsigned char score;
} student_t; 

int main(void) {  

  unsigned char i; // initiate variables

  unsigned int avg_age=0,avg_score=0; // init variables for averages, as int because of size
  srand(0); //init seed for RNG

  uart_init(); // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  student_t student_array[7]; // init array of students
  
  while(1) // start program loop
  {
    for(i=0;i<7;i++) //loop for reading student info
    {
      printf("Student %hhd name:", i+1); //message for user
      scanf("%s",student_array[i].name); //read string
      printf("Student %hhd age:", i+1); 
      scanf("%hhd",&student_array[i].age);
      student_array[i].score=rand()%100; //get random number for score
      avg_age+=student_array[i].age; //add up average age, to calculate later
      avg_score+=student_array[i].score;
    }
    avg_age=avg_age/7; //calculate average age by dividing total by 7
    avg_score=avg_score/7;
    printf("Average age: %u\n",avg_age); // show average age
    printf("Average score: %u\n",avg_score); // show average score
  }
  return 0; //end program
}
