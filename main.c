#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Stack {
  char* data;
  char* top;
} Stack;

void stack_push(Stack* stack, char data) {
  *stack->top = data;
  stack->top += 1;
}

bool stack_empty(Stack* stack) {
  return stack->top == stack->data;
}

char stack_peek(Stack* stack) {
  return *(stack->top - 1);
}

char stack_pop(Stack* stack) {
  char ans = stack_peek(stack);
  stack->top -= 1;
  return ans;
}

int precedence_of(char c) {
  switch (c) {
    case '+':
    case '-': {
      return 1;
    }
    case '*':
    case '/': {
      return 2;
    }
    case '^': {
      return 3;
    }
    default: {
      return -1;
    }
  }
}

bool is_left_associative(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

int main() {
  char _o[100];
  Stack _op = {_o, _o};
  Stack* s = &_op;

  char input[100];
  puts("enter expression e.g. 5+a/(3-c)^5^d");
  puts("there must not be blank spaces between tokens");
  puts("integers must be one of 0,1,2,3,4,5,6,7,8,9");
  gets_s(input, 100);
  printf("input : %s\n", input);

  char outputBuffer[100];
  char* output = outputBuffer;

  for (int i = 0; i < strlen(input); ++i) {
    char c = input[i];
    if (isalnum(c)) {
      output += sprintf_s(output, 3, "%c ", c);
    } else if (c == '(') {
      stack_push(s, c);
    } else if (c == ')') {
      while (!stack_empty(s) && stack_peek(s) != '(') {
        output += sprintf_s(output, 3, "%c ", stack_pop(s));
      }
      stack_pop(s);
    } else {
      while (
          !stack_empty(s)
              && precedence_of(c) <= precedence_of(stack_peek(s))
              && is_left_associative(c)
          ) {
        output += sprintf_s(output, 3, "%c ", stack_pop(s));
      }
      stack_push(s, c);
    }
  }

  while (!stack_empty(s)) {
    output += sprintf_s(output, 3, "%c ", stack_pop(s));
  }

  printf("output: %s\n", outputBuffer);

  return 0;
}
