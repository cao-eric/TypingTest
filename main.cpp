#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cstdlib>
#include <Windows.h>
#include <thread>
#include <fstream>
#include <time.h>
#include <conio.h>
#include <vector>

using namespace std;
float seconds = 0;
float accuracy = 0;
int wordCount = 0;
float wpm = 0;
bool complete = true;


void readyItUp()
{
   cout << "Rules:" << endl
   << "1) MUST HIT SPACE AT THE END BECAUSE BAD CODING." << endl
   << "2) Accuracy is based on \"words\" correct, not characters." << endl
   << "3) Hit any key to start" << endl;

   char start;
   start = _getch();
   cout << "Starting in: " << endl;
   for(int x = 4; x > 0; x--)
   {
      Sleep(1000);
      cout << x << endl;
   }
   system("CLS");

}
void selectPassage(string &passage)
{
   ifstream inFS;
   //randomly selects the passage
   srand(time(NULL));
   int num = (rand() % 3) +1;
   if(num == 1)
   {
      inFS.open("passage1.txt");
      getline(inFS, passage);
      inFS.close();
   }
   else if(num == 2)
   {
      inFS.open("passage2.txt");
      getline(inFS, passage);
      inFS.close();
   }
   else if(num == 3)
   {
      inFS.open("passage3.txt");
      getline(inFS, passage);
      inFS.close();
   }

}
int counter(string passage)
{
   int spaces = 1; //adds an extra word for the last one with no space
   for(unsigned long x = 0; x < passage.length(); x++) //counts the number of spaces in the passage
   {
      if(passage[x] == ' ')
         spaces++;
   }
   return spaces;
}
void timer()
{
   while(complete)
   {
      Sleep(1000); //ticks every seconds
      seconds++;
   }
}

void test(string passage)
{
   HANDLE hConsole; //for coloring the chat boxes
      int green = 10;
      int white = 15;
   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


   string answer = "";
   char letter;
   string temp = "";
   vector<string> wordsInOrder; //fills up the vector with each word from the passage
   for(int x = 0; x <= passage.length(); x++)
   {
      if(x == passage.length()) //ends the loop
      {
         wordsInOrder.push_back(temp);
         break;
      }

      letter = passage[x]; //gets the individual word
      if(letter == ' '){
         wordsInOrder.push_back(temp);
         temp = "";
      }
      else
         temp = temp + letter;
   }

   float incorrections = 0; //detects incorrect words

   for(int x = 0; x < wordsInOrder.size(); x++)
   {
      for(int y = 0; y < wordsInOrder.size(); y++) //prints the whole passage
      {
         if(y == x)
         {
            SetConsoleTextAttribute(hConsole, green); //if it is the highlighted word, console green
            cout << wordsInOrder.at(y) << " ";
         }
         else
         {
            SetConsoleTextAttribute(hConsole, white); //else print it white
            cout << wordsInOrder.at(y) << " ";
         }
      }
      cout << endl << endl;
      answer = "";
      char ch = _getch(); //gets character input
      while((int)ch != 32) //if not space keep getting more inputs
      {
         cout << ch; //print the input

         if((int)ch != 127 && (int)ch != 8) { //if its not a backspace, add it to the answer
         answer = answer + ch; }

         ch = _getch(); //gets next character input
         if((int)ch == 127 || (int)ch == 8) //if backspace is detected
         {
            cout << "\b"; //console goes back one and replaces with space
            cout << " ";

            if(answer.length() > 0) //deletes last character from answer if possible
            {
               string copyover = "";
               for(int z = 0; z < answer.length()-1; z++)
               {
                  copyover = copyover + answer[z];
               }
               answer = copyover;
            }
         }
      }
      if(answer != wordsInOrder.at(x)) //counts incorrect words
      {
         incorrections++;
      }
      system("CLS"); //clears the screen after


   }
   complete = false;
   SetConsoleTextAttribute(hConsole, white); //sets text back to white
   //calculates accuracy
   cout << "word count: " << wordCount << endl;
   cout << "incorrects: " << incorrections << endl;
   accuracy = ((wordCount-incorrections)/wordCount)*100;

}

int main()
{
   readyItUp();
   string passage = "The";
   selectPassage(passage); //selects the passage
   wordCount = counter(passage); // and its word count

   thread first (timer); //starts the timer
   thread second (test, passage); //prompts the user for the typing test

   //synchronizes the programs
   first.join();
   second.join();

   //Prints out the WPM and Accuracy
   float minutes = (seconds/60);
   cout << "Words Per Minute: " << wordCount/minutes << endl;
   cout << "Accuracy: " << fixed << setprecision(2) << accuracy << "%" << endl;
   return 0;
}
