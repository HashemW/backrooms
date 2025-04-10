#ifndef STACK_H
#define STACK_H

#define STACK_CAPACITY 128

typedef struct {
    int data[STACK_CAPACITY];
    int top;
} Stack;

// Initialize the stack
void stack_init(Stack *stack);

// Push an item
int stack_push(Stack *stack, int value);

// Pop an item
int stack_pop(Stack *stack, int *value);

// Peek at the top item
int stack_peek(Stack *stack, int *value);

// Check if stack is empty
int stack_is_empty(const Stack *stack);

// Get current size
int stack_size(const Stack *stack);

#endif

