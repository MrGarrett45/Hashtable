#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int theTableSize = 0;
 
struct hash *table = NULL;
 
struct node 
{
    char string[1000];
    int docID;
    int wordCount;
    int howManyDocs;
    struct node *next;
};
 
struct hash 
{
    struct node *head;
    int counter;
};

 
struct node * makeNode(char *aString, int docID) 
{
    struct node *aNode;
    aNode = (struct node *) malloc(sizeof(struct node));
    //aNode->string = (char *)malloc((strlen(aString) +1) *tableSizeof(char));
    strcpy(aNode->string, aString);
    aNode->docID = docID;
    aNode->next = NULL;
    aNode->wordCount = 1;
    aNode->howManyDocs = 1;
    return aNode;
}

int hash(char* aString)
{
  int total = 0;
   int i;
   for(i = 0; aString[i] != '\0';i++)
   {
     total += aString[i];
   }
  
   
   return total % theTableSize;
}
 
void hashInsert(char* aString, int docID) 
{
    int hashIndex = hash(aString);
    struct node *tempNode = table[hashIndex].head;
    while (tempNode != NULL) 
    {
        if(strcmp(tempNode->string,aString) == 0 && tempNode->docID != docID)
        {
          tempNode->howManyDocs++;
        }
        if(strcmp(tempNode->string,aString) == 0 && tempNode->docID == docID) 
        {
            printf("Already in table: %s\n", tempNode->string);
            tempNode->wordCount++;
            printf("word count: %d\n", tempNode->wordCount);
            return;
           // break;
        }
        tempNode = tempNode->next;
    }
    
    //printf("%s\n", aString);
    //printf("%d\n",hashIndex);
    struct node *aNode = makeNode(aString, docID);
  
    if (!table[hashIndex].head) 
    {
        table[hashIndex].head = aNode;
        table[hashIndex].counter = 1;
        return;
    }
    aNode->next = (table[hashIndex].head);
    table[hashIndex].head = aNode;
    table[hashIndex].counter++;
    return;
}
 

void removeStop(char* aString)        //essentially just a remove node method repurposed to delete stop words
{
    int hashIndex = hash(aString), tester = 0;
    struct node *temp, *aNode;
    int q;
    aNode = table[hashIndex].head;
    if (!aNode) 
    {
        printf("Not in table\n");
        return;
    }
    temp = aNode;
    while (aNode != NULL) {
        if (strcmp(aNode->string, aString) == 0) 
        {
            //printf("Deleted %s from table as a stop word(appears in all documents)\n", aNode->string);
            tester = 1;
            if (aNode == table[hashIndex].head)
            {
              table[hashIndex].head = aNode->next;
            }
            else
            {
              temp->next = aNode->next;
            }
            table[hashIndex].counter--;
            free(aNode);
            break;
        }
        temp = aNode;
        aNode = aNode->next;
    }
    if(tester)
        q = 0;
        //printf("Deleted from table\n");
    else
        printf("Not in table\n");
    return;
}
 
int checkFrequency(char* aString, int docID)  //finds word frequency
{
  struct node *aNode;
  int i;
  int flag = 0;
  int totalFrequency = 0;
  for (i = 0; i < theTableSize; i++) {
      if (table[i].counter == 0)
          continue;
      aNode = table[i].head;
      if (!aNode)
          continue;
      while (aNode != NULL) 
      {
          if(strcmp(aNode->string, aString) == 0 && aNode->docID == docID)
          {
            //printf("The frequency of %s in document %d is %d\n", aString, aNode->docID, aNode->wordCount);
            totalFrequency += aNode->wordCount;
          }
        
          /*
          else if(flag == 1)
          {
            printf("The frequency of %s in document %d is %d\n", aString, aNode->docID, 0);
            flag = 0;
          }
          */
          aNode = aNode->next;
      }
     //printf("The frequency of %s in document %d is %d\n", aString, aNode->docID , totalFrequency);
  }
  printf("The frequency of %s in document %d is %d\n", aString, docID , totalFrequency);
  return totalFrequency;
}

int getHowMany(char* aString) //keeps track of how many documents each word appears in
{
  struct node *aNode;
  int i;
  int flag = 0;
  int howMany = 0;
  for (i = 0; i < theTableSize; i++) {
      if (table[i].counter == 0)
          continue;
      aNode = table[i].head;
      if (!aNode)
          continue;
      while (aNode != NULL) 
      {
          if(strcmp(aNode->string, aString) == 0)
          {
            howMany = aNode->howManyDocs;
          }  
          aNode = aNode->next;
      }
     //printf("The frequency of %s in document %d is %d\n", aString, aNode->docID , totalFrequency);
  }
  //printf("The frequency of %s in document %d is %d\n", aString, docID , totalFrequency);
  return howMany;
}

void printTable() //displays the hash table
{
    //printf("Test");
    struct node *aNode;
    int i;
    for (i = 0; i < theTableSize; i++) {
        if (table[i].counter == 0)
            continue;
        aNode = table[i].head;
        if (!aNode)
            continue;
        printf("\nIndex %d:\n", i);
        while (aNode != NULL) 
        {
            printf("%s\n", aNode->string);
            aNode = aNode->next;
        }
    }
    return;
}

void readIn(char* aString, int docID)   //reads in the documents
{
  FILE *file_ptr = (FILE *)malloc(sizeof(FILE));
    file_ptr = fopen(aString, "r");
    char *x = (char *)malloc(1024*sizeof(char));
    int i = 0;
    char *stringTemp;
    while(fscanf(file_ptr, "%s", x) == 1) 
    {

        hashInsert(x, docID);
        
    }
    fclose(file_ptr);
}

int qfunc (const void * a, const void * b)
{
   return ( *(double*)b - *(double*)a);
}
 
int main() {
    
    char buf[100];
    printf ("Enter table size: ");
  
    if (fgets(buf, sizeof(buf), stdin) != NULL)
    {
      theTableSize = atoi(buf);
      printf ("You entered %d\n", theTableSize);
    } 
  
    table = (struct hash *) malloc(sizeof(struct hash));
    table->head = (struct node *)malloc(sizeof(struct node) * theTableSize);
    
    readIn("D1.txt", 1);
    readIn("D2.txt", 2);
    readIn("D3.txt", 3);
    printTable();
  
    //searchHash("more");
    char input[256];
    printf("Enter your search query: ");
    fgets(input, 256, stdin);
 
    int freq1[3] = {0, 0, 0};
    int freq2[3] = {0, 0, 0};
    int freq3[3] = {0, 0, 0};
    int howManyArray[3] = {0, 0, 0};
    char * split;
    char * last;
    split = strtok (input," ,.-");
    int looper = 0;
    while (split != NULL)
    {
      if(split[strlen(split) - 1] == '\n')
      {
        split[strlen(split) - 1] = '\0';
      }
      
      howManyArray[looper] = getHowMany(split); //sees how many documents the word appears in
      if(howManyArray[looper] == 3)
      {
        //printf("--------------------------------\n");
        printf("Deleted %s from table as a stop word(appears in all documents)\n",split);
        printf("--------------------------------\n");
        removeStop(split);            //REMOVING STOP WORDS, checks if IDF = 3, removes node if it does
        removeStop(split);
        removeStop(split);
      }
      
      freq1[looper] = checkFrequency(split, 1);
      freq2[looper] = checkFrequency(split, 2);
      freq3[looper] = checkFrequency(split, 3);
      
      printf("--------------------------------\n");
      printf("%s appears in %d documents\n",split, howManyArray[looper]);
      printf("--------------------------------\n");
      looper++;
      split = strtok (NULL, " ,.-");
    }
    
    
    int looper2;
    double final[3] = {0, 0, 0};
    //printf("looper %d\n", looper);
    for(looper2 = 0; looper2 < looper; looper2++)
    {
      if(howManyArray[looper2] != 0)
      {
        final[0] += (double)freq1[looper2] * (double)((double)3/(double)howManyArray[looper2]);
        final[1] += (double)freq2[looper2] * (double)((double)3/(double)howManyArray[looper2]); ////tfidf = tf*idf
        final[2] += (double)freq3[looper2] * (double)((double)3/(double)howManyArray[looper2]);
      }
    }
    
    //printf("--------------------------------\n");
    printf("TOTAL RANK doc 1: %f\n", final[0]);
    printf("TOTAL RANK doc 2: %f\n", final[1]); 
    printf("TOTAL RANK doc 3: %f\n", final[2]);
    
    int doc1Val = final[0];
    int doc2Val = final[1];
    int doc3Val = final[2];
  
    qsort(final, 3, sizeof(double), qfunc);
  
    int q;
    int flag = 0;
    /*
    printf("RECOMMANDED RANKING: \n");
    for(q = 0; q < 3; q++)
    {
      flag = 0;
      if(doc2Val == final[q] && flag == 0)
      {
        printf("DOC 2: %f\n", final[q]);
        flag = 1;
      }
      
      if(doc3Val == final[q] && flag == 0)
      {
        printf("DOC 3: %f\n", final[q]);
        flag = 1;
      }
      
      if(doc3Val != final[q] && doc2Val != final[q])
      {
        printf("DOC 1: %f\n", final[q]);
        flag = 1;
      }
    }
    */
  
    //numDocuments/number of documents word appears in
    return 0;
}