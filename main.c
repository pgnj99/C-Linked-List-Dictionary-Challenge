// Created by Peter Giordano
// July 8, 2023

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Maximum number of words that can be stored
#define MAX 100

// word structure acts as nodes for linked list
struct word { 
  int length;         // Length of word
  char letters[5];    // First four letters of word
  struct word *next;  // Next entry in linked list
};

// Pre-allocated pool of word nodes with head index
// This will act in place of dynamic memory
struct word wordPool[MAX];
int head = 0;

// Number of words in linked list
int wordCount = 0;

// "Helper" functions

// Used by bubbleSort function to retrieve previous linked list entry from one being switched
// Needed to reassign next pointer
struct word* getPreviousWord(struct word *ptr) {
  // Create pointer to iterate through list starting from head
  struct word *test = &wordPool[head];
  while (test->next != NULL) {
    if (test->next == ptr) {
      // Return previous entry
      return test;
    }
    test = test->next;
  }
}

// Used by bubbleSort function to change head index when a new entry becomes head
void changeHead(struct word *ptr) {
  // Iterate through pool until new head is found
  for (int i = 0; i < wordCount; i++) {
    if (ptr == &wordPool[i]) {
      head = i;
      return;
    }
  }
}

// Used by addWord to alphabetically sort linked list entries
// wordPool array remains unchanged
void bubbleSort() {
  // If there are fewer than two words then nothing needs to be sorted
  if (wordCount < 2) {
    return;
  }

  // Set up pointers for head, current node to switch, and previous node
  struct word *headptr = &wordPool[head];
  struct word *current;
  struct word *prev;

  // Sorting will iterate one time for each item in linked list
  for (int i = 0; i < wordCount - 1; i++) {
    current = headptr;  // Re-initialize pointer

    // With each iteration, one less item needs to be read
    for (int j = 0; j < wordCount - i - 1; j++) {
      // Check whether words are out of order alphabetically
      // If letters are the same, compare length
      int diff = strcasecmp(current->letters, current->next->letters);
      // If out of order...
      if (diff > 0 || (diff == 0 && current->length > current->next->length)) {
        // Temporary pointer to switch with current
        struct word *temp = current->next;

        // If head is being switched, a new head index must be assigned
        if (current == headptr) {
          changeHead(current->next);
          headptr = current->next;
        }
        // Otherwise, the previous node needs to update its next pointer
        else {
          prev = getPreviousWord(current);
          prev->next = current->next;
        }

        // Switch nodes with each other using next pointers
        // If at end of list, use null pointer
        if (current->next->next != NULL) {
          current->next = current->next->next;
        }
        else {
          current->next = NULL;
        }
        temp->next = current;
      }
      else {
        current = current->next;
      }
    }
  }
}

// For adding word to linked list, as required by parse function
void addWord(int length, char letters[5]) {
  // Check if linked list has reached its full capacity
  if (wordCount >= MAX) {
    printf("Maximum word count reached. Unable to add more words.\n");
    return;
  }
  
  // Check if word has already been added to linked list
  for (int i = 0; i < wordCount; i++) {
    if (strcmp(letters, wordPool[i].letters) == 0 && length == wordPool[i].length) {
      return;
    }
  }

  // Add length, letters, and next to new node
  wordPool[wordCount].length = length;
  wordPool[wordCount].next = NULL;

  for (int i = 0; i < strlen(letters); i++) {
    wordPool[wordCount].letters[i] = letters[i];
  }

  // Set node at end of list with next pointer
  if (wordCount != 0) {
    struct word *point = &wordPool[head];
    while (point->next != NULL) {
      point = point->next;
    }
    point->next = &wordPool[wordCount];
  }

  // Increase wordCount to indicate number of entries
  wordCount++;

  // Sort list alphabetically
  bubbleSort();
}

// a) Enter new sentence
// Starts by parsing words out of sentences
void parse(char sentence[100]) {
  bool check = false;  // True if currently parsing a word
  int start;           // First letter index
  int end;             // Last letter index
  int length;          // Length of word
  int length4;         // Length of shortened word

  // Iterate through each letter of sentence
  for (int i = 0; sentence[i] != '\0'; i++) {
    // Start a new word at start of sentence and after non-alphanumeric characters
    if (check == false && isalnum(sentence[i]) > 0) {
      check = true;
      start = i;
    }

    // End the sentence when the end of the sentence or a non-alphanumeric character is reached
    if (check == true && (isalnum(sentence[i]) == 0 || sentence[i+1] == '\0')) {
      check = false;
      // End index depends on whether end of sentence is reached
      if (sentence[i+1] == '\0' && isalnum(sentence[i]) != 0) {
        end = i;
      }
      else {
        end = i - 1;
      }

      // Word length will be difference between starting and ending points
      length = (end - start) + 1;
      char letters[5] = "";
      
      // Account for whether word is less than four letters long
      if (length >= 4) {
        length4 = 4;
      }
      else {
        length4 = length;
      }

      // Save first four letters in new string
      for (int j = 0; j < length4; j++) {
        letters[j] = sentence[start+j];
      }

      // Add word to linked list
      addWord(length, letters);
    }
  }
}

// b) Search for word
// Much is copied from parsing function
void search(char sentence[100], bool sensitive) {
  int end;             // Last letter index
  int length;          // Length of word
  int length4;         // Length of shortened word
  
  for (int i = 0; sentence[i] != '\0'; i++) {

    if (sentence[i+1] == '\0') {
      length = i + 1;
    }
    else {
      length = i;
    }
  }
  
  char letters[5] = "";
      
  // Account for whether word is less than four letters long
  if (length >= 4) {
    length4 = 4;
  }
  else {
    length4 = length;
  }

  // Save first four letters in new string
  for (int j = 0; j < length4; j++) {
    letters[j] = sentence[j];
  }

  // Check for matching word in linked list
  // This was initially going to be a separate function but ran into too many segmentation errors
  struct word *point = &wordPool[head];
  int i = 0;
  
  while (true) {
    // Match is found if letters and length are equal
    // Account for case sensitivity setting
    if ((strcasecmp(letters, point->letters) == 0 && length == point->length && !sensitive) || (strcmp(letters, point->letters) == 0 && length == point->length)) {
      printf("Match found: %s, length %d\n", point->letters, point->length);
      break;
    }

    // This while loop setup was used due to out-of-bounds errors
    i++;
    if (i == wordCount - 1) {
      printf("No match found.\n");
      break;
    }
    else {
      point = point->next;
    }
  }
}

// c) Print list
void printWordList() {
  // Check if linked list is empty
  if (wordCount == 0) {
    printf("The Word List is empty.\n");
    return;
  }

  // Go through list starting at head
  struct word *point = &wordPool[head];
  printf("Word List:\n");
  while (point != NULL) {
    printf("%s, length %d\n", point->letters, point->length);
    point = point->next;
  }
}

// d) Delete list
void deleteWordList() {
  // Check if linked list is empty
  if (wordCount == 0) {
    printf("The Word List is already empty.\n");
    return;
  }
  
  printf("Deleting...\n");
  
  // Remove data from each used-up entry in pool
  for (int i = 0; i < wordCount; i++) {
    wordPool[i].length = -1;
    wordPool[i].next = NULL;
    strcpy(wordPool[i].letters, "\0");
  }

  // Reset word count and head index
  wordCount = 0;
  head = 0;
  printf("The list has been deleted.\n");
}

int main(void) {
  char sentence[100];
  bool sensitive;
  char response;

  // Receive sentence from user
  printf("Enter your sentence here:\n");
  scanf("%[^\n]", sentence);
  
  // Consume newline character from previous input
  // This prevents endless loop caused by scanf
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  // Parse sentence without using strtok
  parse(sentence);

  // With dictionary initialized, the user can be prompted
  while (true) {
    printf("What would you like to do now?\na) Enter new sentence\nb) Search for word\nc) Print list\nd) Delete list\ne) Exit\n");
    scanf(" %c", &response);
    while ((c = getchar()) != '\n' && c != EOF);

    // a) Enter new sentence
    if (tolower(response) == 'a') {
      printf("Enter your sentence here:\n");
      scanf("%[^\n]", sentence);
      while ((c = getchar()) != '\n' && c != EOF);
      parse(sentence);
    }
      
    // b) Search for word
    else if (tolower(response) == 'b') {
      printf("Enter your word:\n");
      scanf("%s", sentence);
      while ((c = getchar()) != '\n' && c != EOF);

      printf("Would you like to make this case-sensitive? Enter y for yes, or n for no:\n");
      while (true) {
        scanf(" %c", &response);
        while ((c = getchar()) != '\n' && c != EOF);
    
        if (tolower(response) == 'y') {
          sensitive = true;
          break;
        }
        else if (tolower(response) == 'n') {
          sensitive = false;
          break;
        }
        else {
          printf("Invalid response. Enter y for yes, n for no:\n");
        }
      }
      search(sentence, sensitive);
    }

    // c) Print list
    else if (tolower(response) == 'c') {
      printWordList();
    }

    // d) Delete list
    else if (tolower(response) == 'd') {
      deleteWordList();
    }

    // e) Exit program
    else if (tolower(response) == 'e') {
      printf("Goodbye.\n");
      break;
    }

    // For catching invalid responses
    else {
      printf("Invalid response.\n");
    }
  }
  
}