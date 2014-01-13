/*
 * File:   cache-sim.c
 * Author: Vaibhav
 *
 * Created on December 13, 2013, 3:56 PM
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "cache-sim.h"
 
 
/*
 *
 */

/*Global Varibales that I will be using throughtout my code*/
int L1Size=0;int L1assoc=0;int numberofsets1=0; int L1_Coldmisses=0; int L1_misses=0; int L1_hits=0;
int L2Size=0;int L2assoc=0;int numberofsets2=0;int L2_Coldmisses=0; int L2_misses=0; int L2_hits=0;
int L3Size=0;int L3assoc=0;int numberofsets3=0;int L3_Coldmisses=0; int L3_misses=0; int L3_hits=0;
int memoryaccesses=0;
Cache ** L1;
Cache ** L2;
Cache ** L3;
int blocksize;
int replacealg;

 /*Update my cache so I can do my LRU calculation.... it increments the frequency counter to 1 and if there is a hit or cold miss it incremements everything in my array and makes the initial item's frequency 1*/
void updatecache(Cache **cache, int setindex, int associativity, int ix){
    int i;
    for(i=0; i<associativity ;i++){
        if(cache[setindex][i].freq!=0){
             cache[setindex][i].freq++;
        }
 
    }
    cache[setindex][ix].freq=1;
   
}
 /*Since I have my frequncies down... All i have to do is check for the MAXIMUM frequency number and remove that because that was the least recently used*/
void lru(Cache **cache, int setindex ,int associativity, size_t tag){
    int maximum = cache[setindex][0].freq;
    int i, j=0;
    for(  i =0;i<associativity;i++){
        if(maximum < cache[setindex][i].freq){
            maximum= cache[setindex][i].freq;
            j=i;
        }
    }
    cache[setindex][j].tag=tag;
    updatecache(cache, setindex,associativity,j );
   
}
 
 
 /*Creates a linked list for me to First in first out*/
Node* createlinkedlist(int associativity){
    Node *head;
    Node *list;
    int i;
    list=NULL;
 
    for(i=0;i<associativity;i++){
        head=(Node *)malloc(sizeof(Node));
        head->next=list;
        list=head;
    }
    head=list;
    return head;
}
 
 /*My fifo function which acutally gets rid of the first element in the array and put the new one in the back
  * 1)Copy the the elements from the array to linkedlist to help me delete the node
  * 2)After deleteing front add the linked list back into the array*/
void fifo(int setindex, int associativity, size_t tag, int chc){
    int i;
    Node* ll= createlinkedlist(associativity);
    Node* temp = ll;
 if(chc==1){
      for(i = 0; i <associativity; i++){
          temp->tag= L1[setindex][i].tag;
          if(temp->next==NULL){
              Node *newelement =(Node*)malloc(sizeof(Node));
              newelement->tag= tag;
              newelement->next= NULL;
              temp->next=newelement;
          }else
              temp=temp->next;
      }ll= ll->next;
      for( i = 0; i < associativity; i++){
          L1[setindex][i].tag= ll->tag;
          ll=ll->next;
      }
 
 }if(chc==2){
        for(i = 0; i <associativity; i++){
          temp->tag= L2[setindex][i].tag;
          if(temp->next==NULL){
              Node *newelement =(Node*)malloc(sizeof(Node));
              newelement->tag= tag;
              newelement->next= NULL;
              temp->next=newelement;
          }else
              temp=temp->next;
      }ll= ll->next;
      for( i = 0; i < associativity; i++){
          L2[setindex][i].tag= ll->tag;
          ll=ll->next;
      }
 
 }if(chc==3){
        for(i = 0; i <associativity; i++){
          temp->tag= L3[setindex][i].tag;
          if(temp->next==NULL){
              Node *newelement =(Node*)malloc(sizeof(Node));
              newelement->tag= tag;
              newelement->next= NULL;
              temp->next=newelement;
          }else
              temp=temp->next;
      }ll= ll->next;
      for( i = 0; i < associativity; i++){
          L3[setindex][i].tag= ll->tag;
          ll=ll->next;
      }
 
 }  
   
}
 /*Search method is THE HEART of the program. IT searches for the given tag and if its not there it adds to the caches*/
int Search(Cache** cache, int inoffset, size_t tag, int associativity ,int chc){
  int i;
 
  for(i=0; i< associativity ;i++){
    if(cache[inoffset][i].valid==0){
        cache[inoffset][i].tag=tag;
        cache[inoffset][i].valid=1;
        if(chc==1){
            L1_Coldmisses++;
            L1_misses++;
            updatecache(cache, inoffset, associativity,i);
                 
         
        }else if(chc==2){
            L2_Coldmisses++;
            L2_misses++;
            updatecache(cache, inoffset, associativity,i);
 
        }else{
            L3_Coldmisses++;
            L3_misses++;
            updatecache(cache, inoffset, associativity,i);
			return 1;
        }
        return 0;
    }else if(cache[inoffset][i].valid==1){
        if(cache[inoffset][i].tag==tag){
            if(chc==1){
                L1_hits++;
                    updatecache(cache, inoffset, associativity,i);
                    return 1;
                 
            }else if(chc==2){
                L2_hits++;
                    updatecache(cache, inoffset, associativity,i);
                    return 1;
                 
            }else if(chc==3){
                L3_hits++;
                    updatecache(cache, inoffset, associativity,i);
                    return 1;

            }
            
        }else if(associativity==1){
          cache[inoffset][i].tag= tag;
            if(chc==1){
                 L1_misses++;
            }else if(chc==2){
                 L2_misses++;
            }else if(chc==3){
                 L3_misses++;
            }
                 return 0;
        }else{
            continue;
        }
    }
  }
  if(chc==1){
     L1_misses++;
        if(replacealg==1){
                fifo(inoffset,L1assoc,tag,chc);
        }else{
              lru(cache,inoffset,L1assoc,tag);
        }
     }else if(chc==2){
         
        L2_misses++;
        if(replacealg==1){
                fifo(inoffset,L2assoc,tag,chc);
        }else{
              lru(cache,inoffset,L2assoc,tag);
        }
     }else if(chc==3){
        L3_misses++;
        if(replacealg==1){
               fifo(inoffset,L3assoc,tag,chc);
        }else{
              lru(cache,inoffset,L3assoc,tag);
        }
     }
    return 0;
   }
 /*Calculates my indexoffsets*/
int indexoffset(int inoffset, size_t hex, int blockoffset){
 
    size_t large= pow(2,inoffset)-1;
    int index_offset = (hex >> blockoffset)&large;
    return index_offset;
}
 /*Allocate some space in the memory for the caches*/
Cache** createCache(int numberofsets, int associativity){
    Cache** array;
    array = malloc(numberofsets * sizeof(Cache *));
    int i,n;
    for(i=0; i< numberofsets; i++){
        array[i] = malloc((associativity) * sizeof(Cache));
    }
   
    for(i=0; i< numberofsets; i++){
        for(n=0; n<associativity;n++){
            array[i][n].valid=0;
            array[i][n].freq=0;
        }
    }
   
    return array;
}

/*Checks if the size is power of 2*/
int power(int x){
	return x && (!(x&(x-1)));
}
 
int main(int argc, char** argv) {
    if (argc==2){
 
      if(strcmp(argv[1],"-h")==0){
        fprintf(stderr,"Help Menu:\n");
        fprintf(stderr,"cache-sim [-h] -l1size <L1 size> -l1assoc <L1 assoc> -l2size <L2 size> -l2assoc <L2 assoc> -l3size <L3 size> -l3assoc <L3 assoc> <block size> <replace alg> <trace file>\n");
         return 0;
      }else{
        fprintf(stderr,"Not a valid input!");
        return 1;
      }
  }
  if(argc!=16){
    
    fprintf(stderr,"Not a valid number of arguments. Please type in -h for a help menu.");
    return 1;
  }
  blocksize= atoi(argv[13]);
  if(power(blocksize)!=1){
	  return 1;
  }
 
  L1Size= atoi(argv[2]);
  L2Size= atoi(argv[6]);
  L3Size= atoi(argv[10]);
  if (strcmp(argv[4],"direct")==0)
  {
    L1assoc=1;
    numberofsets1=L1Size/(blocksize*L1assoc);
  }else if(strcmp(argv[4],"assoc")==0){
    numberofsets1=1;;
    L1assoc= L1Size/blocksize;
  }else{
        char* assc= argv[4];
        L1assoc=assc[6]-'0';
        numberofsets1=L1Size/(blocksize*L1assoc);
        }
 if (strcmp(argv[8],"direct")==0)
  {
    L2assoc=1;
    numberofsets2=L2Size/(blocksize*L2assoc);
  }else if(strcmp(argv[8],"assoc")==0){
 
    numberofsets2=1;
    L2assoc= L2Size/blocksize;
  }else{
        char* assc= argv[8];
        L2assoc=assc[6]-'0';
        numberofsets2=L2Size/(blocksize*L2assoc);
        
        }
 
  if (strcmp(argv[12],"direct")==0)
  {
    L1assoc=1;
    numberofsets3=L3Size/(blocksize*L3assoc);
  }else if(strcmp(argv[12],"assoc")==0){
 
    numberofsets3=1;
    L3assoc= L3Size/blocksize;
  }else{
        char* assc= argv[12];
        L3assoc=assc[6]-'0';
        numberofsets3=L3Size/(blocksize*L3assoc);
        }
   
  if((strcmp(argv[14],"fifo")==0)||(strcmp(argv[14],"FIFO")==0)){
      replacealg=1;
  }
  if((strcmp(argv[15],"lru")==0)||(strcmp(argv[15],"LRU")==0)){
      replacealg=2;
  }
   //test files
   FILE *file = fopen(argv[15],"r");
   L1= createCache(numberofsets1,L1assoc);
   L2= createCache(numberofsets2,L2assoc);
   L3= createCache(numberofsets3,L3assoc);
 
   size_t hexnum;
   
    while(fscanf(file,"%lx",&hexnum)!= EOF)
    {
         // Calculating the index offset
        int blockoffset=log2(blocksize);
        size_t high= pow(2,blockoffset)-1;
        size_t boffset= hexnum & high;
          // printf("Block Offset: %d\n", blockoffset);
          // printf("Block Address: %lx\n", boffset);
        int inoffsetl1=0;
        int indexoffsetl1;
        inoffsetl1=log2(numberofsets1);
        indexoffsetl1= indexoffset(inoffsetl1,hexnum,blockoffset);
                //     printf("%d\n", indexoffsetl1);
               // printf("%d\n", indexoffsetl1 );
        int inoffsetl2=0;
        int indexoffsetl2;
        inoffsetl2=log2(numberofsets2);
        indexoffsetl2= indexoffset(inoffsetl2,hexnum,blockoffset);
                    // printf("%d\n", indexoffsetl2);
       
        int inoffsetl3=0;
        int indexoffsetl3;
        inoffsetl3=log2(numberofsets3);
        indexoffsetl3= indexoffset(inoffsetl3,hexnum,blockoffset);
                 //printf("%d\n", indexoffsetl3);
       
 
           
           // Calculating the tag
                size_t tagl1 = hexnum >> (blockoffset+inoffsetl1);
                size_t tagl2 = hexnum >> (blockoffset+inoffsetl2);
                size_t tagl3 = hexnum >> (blockoffset+inoffsetl3);
                     //printf("%lx\n", tagl1);
                     //Search/add in L1
               int returnval =Search(L1,indexoffsetl1,tagl1,L1assoc,1);
               //if l1 miss go to l2
               if(returnval==0){
                   //search in l2cache
                   int returnval2=Search(L2,indexoffsetl2,tagl2,L2assoc,2);
                   //l2 miss then go to l3
                   if(returnval2==0){
                       int returnval3= Search(L3,indexoffsetl3,tagl3,L3assoc,3);
     
                   }
               }
                memoryaccesses++;
               
 
                 
    }
            printf("Memory accesses: %d \n", memoryaccesses);
            printf("L1 Cache hits: %d \n",L1_hits);
            printf("L1 Cache miss: %d \n",L1_misses);
            printf("L2 Cache hits: %d \n",L2_hits);
            printf("L2 Cache miss: %d \n",L2_misses);
            printf("L3 Cache hits: %d \n",L3_hits);
            printf("L3 Cache miss: %d \n",L3_misses);
            printf("L1 Cold misses: %d \n",L1_Coldmisses);
            printf("L2 Cold misses: %d \n",L2_Coldmisses);
            printf("L3 Cold misses: %d \n",L3_Coldmisses);
	int i;
	for( i=0; i<numberofsets1; i++){	
		free(L1[i]);
	}
	for( i=0; i<numberofsets2; i++){

		free(L2[i]);
	}
	for( i=0; i<numberofsets3; i++){

		free(L3[i]);
	}
     free(L1);
     free(L2);
     free(L3);
 
    return (EXIT_SUCCESS);
}
