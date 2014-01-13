Cache-simulator
===============
General Idea:
Given a file consiting hexadecimal addresses(64 bit), this program stores each of the addresses in the appropriate cache and calculate hits/ misses.

My Algorithm:
1) Caches were made up as arrarys because that how I visual caches...
    a) Create 3 caches (L1 L2 L3)
2) Given a hex number, I have to see if that address is already in my cache.
    a) Calculate Index offset(EASY)
    b) if cache is Direct, check if the two addesses are the same
    c) if its n-ways or fully associative cache
        i)FIFO
            Idea: FIFO= Queue( therefore I created a linked list )
        ii)LRU
            Idea:  Added freqency to each element of the array..
