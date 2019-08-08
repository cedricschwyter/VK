/**
    Defines the Queue enumeration

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Queue.cpp
    @brief        Definition of the Queue enumeration
*/
#ifndef QUEUE_CPP
#define QUEUE_CPP

/**
        Defines an enumeration for queues
*/
typedef enum Queue {

    GRAPHICS_QUEUE = 2,
    TRANSFER_QUEUE = 1,
    PRESENT_QUEUE = 0

} Queue;
#endif  // QUEUE_CPP
