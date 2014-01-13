

#ifndef CACHE-SIM_H
#define	CACHE-SIM_H

typedef struct Cache{
   size_t tag;                
   int valid;
   int freq;
}Cache;
 
typedef struct Node
{
    size_t tag;                 // the data of the Node. In this case its an address
    struct Node *next;              //pointer to the next node
    struct Node *prev;
 
}Node;

#endif	/* CACHE-SIM_H */

