/*******************************************************
Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <delay.h>
#include "LCD.h"
#include <string.h>
#include <stdbool.h>
#include "LCD2.h"

unsigned char out;
unsigned char SelectedCategory;
int i;
char* h;

int WordIndex = 0;
int WordLen;
char tmp[50];
char Scoretmp[50];
#define  CATEGORY_LENGTH    4
#define MAX_HEALTH 3
#define MAX_WORD_NUM 20
char* Category[CATEGORY_LENGTH] = {"Sport","Movie","Countries","CS"};
char* Sport[20] = {"football","basketball","volleyball","karate","kungfu","boxing","tennis","running","wrestling","hockey",
                    "judo","golf","baseball","billiard","curling","waterpolo","taekwando","fencing","swimming","jujitsu"};
char * Movie[20] ={"joker","batman","godfather","lizard","soul","incredibles","memento","arrival","buried","cube","bladerunner","alien",
               "shining","pyscho","titanic","vertigo","exam","nocturnalanimals","swissarmyman","seperation"
               };

char * Countries[20]={"iran","germany","brazil","england","argentina","sweden","samoa","china","uae","uzbakistan"
                        ,"denmark","ghana","bangladesh","venezuela","zimbawe","nigeria","luxembourg","greece","france","cyprus"}    ;
                                                                                                                                    
char * ComputerScience[20]={"code","debug","compiler","java","boolean","operatingsystem","github","function","syntax" ,"cipher",
                            "argument","python","machinelearning","interpreter","google","query","algorithm","kernel","processor","concurrency"

};                                                                                                        
char* AWords[6] = { "a","b","c","d","e","f" };
char* BWords[6] = { "g","h","i","j","k","l" }; 
char* CWords[7] = { "m","n","o","p","q","r","s" };
char* FWords[7] = { "t","u","v","w","x","y","z" };
char* DWords[7] = { "t","u","v","w","x","y","z" };

char* Words[26] = { "a","b","c","d","e","f", "g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" }; 

int alphabet[26]; 

int point;

int WordSetState = 0;
                  
int CharIndex; // index of words set
 char* ChoosenChar;
char* CurrentWord;

bool bCanSelectCategory;
bool bCanGuess;
bool bIsSelectedNumber;
bool bIsFirstTime;
bool bWrongGuess;
bool bFoundCorrectGuess;
bool bWon;

int health = MAX_HEALTH;

void CheckGuess( char* in,char* CurrWord);

void ShowCategory();


void SelectAndShowWord(unsigned char in);
 
void ShowRandomWord(char* InputStr);

void Print6( char* in[6]);

void Print7(char*  in[7]);
void InitAlphabet();   
void InitHealth();
void EndGame();
void CalculatePoint();

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0xFF;
TCNT1L=0x0F;
// Place your code here

lcd_gotoxy2(11,4);
for ( i = 0 ; i < 3; i++){
     
    lcd_print2(" "); 
}
lcd_gotoxy2(11,4);

for ( i = 0 ; i < health; i++){
     
    lcd_print2("*"); 
}

   
     sprintf(Scoretmp,"Score: %d",point);
     
      lcd_gotoxy2(10,3);
      lcd_print2(Scoretmp);
      lcd_print2(" ");
      lcd_print2(" ");
      lcd_print2(" ");
}

void main(void)
{

TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0xFF;
TCNT1L=0x0F;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

    bIsFirstTime = true;
     MCUCR=0x02;
     GICR=(1<<INT0); 
       

    KEY_DDR = 0xF0;
    KEY_PRT = 0x0F;
    i=0;
  
// Global enable interrupts


          

    ShowCategory();
    
    lcd_init2();    
    lcd_gotoxy2(1,1);
    lcd_print2("Second Lcd");
#asm("sei") 

while (1)
      {
      // Place your code here

      }
}




interrupt [EXT_INT0] void ext_int0_isr(void)
{
   
  DDRA=0xFF;
  PORTA=0x00;

 lcd_gotoxy(1,1);
 
 out=get_key(); 
 
 

 if(bCanSelectCategory == true)
 {  
    
    if(atoi(&out) >= 1 && atoi(&out) <= CATEGORY_LENGTH)  //Check If input is valid
    {     
       bCanSelectCategory = false;   
       SelectedCategory = out; 
       SelectAndShowWord(out); 

    }
 }
 else if ( bCanGuess == true )
 {   
 

        
    if(out == 'A')
       {           
            WordSetState = 0;
            Print6( AWords);
             
            bIsFirstTime = false;
       }
       else if(out == 'B')
       {                  
            WordSetState = 1;
            Print6( BWords);
            bIsFirstTime = false;
       }  
       else if(out == 'C')
       {                     
            WordSetState = 2;
            Print7( CWords);
            bIsFirstTime = false; 
       }
       else if(out == 'D')
       {                    
            WordSetState = 3;
            Print7( FWords); 
            bIsFirstTime = false;
       }else
       {
           if(!bIsFirstTime)
           {  
                if(out == '#')
                {
                    //todo UpperCase
                }  
                else
                {
                     bIsSelectedNumber = true;
                }
              
           }
       }   
       
 
    if(bIsSelectedNumber)
    {         
        bIsSelectedNumber = false;
         
        h = &out;   
        sscanf(&out,"%d",&CharIndex); 
        


        if(WordSetState == 0)
        {    
                ChoosenChar = AWords[CharIndex - 1]; 

        }
        else if ( WordSetState == 1)
        {
               ChoosenChar = BWords[CharIndex - 1]; 
            
        }  
        else if ( WordSetState == 2)
        {
               ChoosenChar = CWords[CharIndex - 1]; 
        }
        else if ( WordSetState == 3)
        {
              ChoosenChar = DWords[CharIndex - 1]; 
        }
        
        //lcd_print2(&ChoosenChar);  
        CheckGuess(ChoosenChar, CurrentWord);
        
    }

    
 }

      
  KEY_DDR = 0xF0;
  KEY_PRT = 0x0F;

}


  void ShowCategory()
  {     
  
  
        
        InitAlphabet();  
        InitHealth();
        lcd_init();
        WordIndex = 0;  
        point = 0;
        WordIndex = 0;
       lcd_gotoxy(1,1);
      
       for(i = 0 ; i < CATEGORY_LENGTH ; i++)
       {
            if(i != 0) 
                sprintf(tmp," %d.",i+1);
            else
                sprintf(tmp,"%d.",i+1);
            
            lcd_print(tmp);
            lcd_print(Category[i]);
       }
       
       bCanSelectCategory = true;
  }
  
  void SelectAndShowWord(unsigned char in)
  {
      if(WordIndex > MAX_WORD_NUM - 1)
      {
      
           bWon = true;
           EndGame();
      }
      else
      {
    
          //Init and seed
          switch(in)
          {
            case '1': 
                    CurrentWord = Sport[WordIndex];
                    ShowRandomWord(CurrentWord);
                    break;
            case '2':
                    CurrentWord = Movie[WordIndex];
                    ShowRandomWord(CurrentWord);
                    break;
            case '3':
                    CurrentWord = Countries[WordIndex];
                    ShowRandomWord(CurrentWord);
                    break;
            case '4':
                
                CurrentWord = ComputerScience[WordIndex];
                ShowRandomWord(CurrentWord);
                break;
          }     
          
                WordIndex = (WordIndex+ 1) ;  
      }

  } 
  
  void ShowRandomWord(char* InputStr)
  {                   

     lcd_init(); 
     lcdCommand(0x0c);
     WordLen = strlen(InputStr);    
     InitAlphabet();  
     
     
     
     for(  i = 0 ; i < WordLen ; i++)
     {
       alphabet[InputStr[i] - 'a'] = 1;
            
              
         lcd_print("_ ");
         
     }
      bCanGuess = true;
  }
  
  
  void CheckGuess( char* in,char* CurrWord)
  {  
    

     
        
      lcd_init(); 
          
     lcdCommand(0x0c);
     bWrongGuess = true;

     for(  i = 0 ; i < WordLen ; i++)
     {           
          
         if( in[0] == CurrWord[i] )
         {
            alphabet[in[0] - 'a'] = 2;
            bWrongGuess = false;
         }
           
     }  
     

        
     bFoundCorrectGuess = true;
     for(  i = 0 ; i < WordLen ; i++)
     {           
          
         if( alphabet[CurrWord[i] - 'a'] == 2 )
         {

            lcd_print(Words[CurrWord[i] - 'a'] );
            
            lcd_print(" ");
         }
         else if( alphabet[CurrWord[i] - 'a'] == 1 )
         {
              bFoundCorrectGuess = false;
              lcd_print("_ ");
         }
       
         
     }
     
          if(bWrongGuess)
     {
         health -= 1;
            if( health <= 0)
            {
                bWon = false;
                EndGame();
            }
     } 
     
     if(bFoundCorrectGuess)
     {
        
         CalculatePoint();
         delay_ms(100);
         lcd_init();
         lcd_print("Correct!");
         delay_ms(100);
         InitHealth();
         SelectAndShowWord(SelectedCategory);
     }        
     
     
     
                       
  
      //bIsFirstTime = true;
  } 
   
  
  void InitAlphabet()
  {
    for( i = 0 ; i < 25 ; i++)
        alphabet[i] = 0;
  
  }
  
  void Print6( char* in[6])
  {  
  
    
    lcd_init2(); 
    lcdCommand2(0x0c); 
    
    for(i = 0 ; i < 6 ; i++)
    {   
         if(i != 0) 
                sprintf(tmp," %d.",i+1);
            else
                sprintf(tmp,"%d.",i+1);
            
            
            lcd_print2(tmp);  
            lcd_print2(in[i]);
    }
  }
  
    void Print7(char*  in[7])
  {   
  
  
  
    lcd_init2(); 
    lcdCommand2(0x0c); 
    
    for(i = 0 ; i < 7 ; i++)
    {   
         if(i != 0) 
                sprintf(tmp," %d.",i+1);
            else
                sprintf(tmp,"%d.",i+1);
            
            
            lcd_print2(tmp);  
            lcd_print2(in[i]);
    }
  }   
  
  void InitHealth()
  {
    health = MAX_HEALTH;
  
  }
  
    
  
  void EndGame()
  {
    
      bCanGuess = false;
      //todo lcd print 
      lcd_init();
      if(bWon)
      {
           lcd_print("You Won, hooray :) ");
      }
      else
      {
           lcd_print("GAME OVER!!                   try again");
      }
      
      delay_ms(200);
      InitHealth();
      ShowCategory(); 
  
  }
  
  void CalculatePoint()
  {
  
     point += ( WordLen * 10 + WordIndex);

  }