// Stack implementation in C
// stole this code from https://www.programiz.com/dsa/stack
// i cannot be asked to make a stack and waste my time
// 20 mins later: this stack implementation is trash, I have to change stuff
// it was so trash that I gave up and just asked gpt to make it for me
// better from gpt but still had to edit :(
#include "../headers/stack.h"

void stack_init(Stack *stack) {
    stack->top = -1;
}

int stack_push(Stack *stack, int value) {
    if (stack->top + 1 >= STACK_CAPACITY)
        return 0; // overflow
    stack->data[++stack->top] = value;
    return 1;
}

int stack_pop(Stack *stack, int *value) {
    if (stack->top < 0)
        return 0; // underflow
    if (value)
        *value = stack->data[stack->top];
    stack->top--;
    return 1;
}

int stack_peek(Stack *stack, int *value) {
    if (stack->top < 0)
        return 0;
    if (value)
        *value = stack->data[stack->top];
    return 1;
}

int stack_is_empty(const Stack *stack) {
    return stack->top < 0;
}

int stack_size(const Stack *stack) {
    return stack->top + 1;
}
