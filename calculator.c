#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define emptyChar ' '
#define emptyDouble -1

typedef struct Node *list;
typedef struct Node
{
    double value;
    char Character;
    bool isChar;
    list next;
} Node;

bool isChar(list node)
{
    return node->isChar;
}
bool CharisOperator(char Character)
{
    return Character == '^' ||Character == '+' || Character == '-' || Character == '*' || Character == '/';
}

bool charIsParenthisis(char Char){
    return Char=='(' || Char==')';
}

bool CharDetector(char Char){
    return CharisOperator(Char) || charIsParenthisis(Char);
}


bool isOperator(list node)
{
    return isChar(node) && CharisOperator(node->Character);
}

bool isParenthesis(list node)
{
    return charIsParenthisis(node->Character);
}

list CreateCharNode(char Character)
{
    list new_node = malloc(sizeof(Node));
    new_node->isChar = 1;
    new_node->Character = Character;
    return new_node;
}

list CreateDoubleNode(double value)
{
    list new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->isChar = 0;

    return new_node;
}

list CreateNode(bool isChar, double value, char Character)
{
    if (isChar)
    {
        return CreateCharNode(Character);
    }
    return CreateDoubleNode(value);
}

void printNode(list Node)
{
    if (Node->isChar)
    {
        printf("%c", Node->Character);
        return;
    }
    printf("%.1lf ", Node->value);
}

void push(list *head, list Node)
{
    Node->next = *head;
    *head = Node;
}

void print(list head)
{
    list temp = head;
    while (temp)
    {
        printNode(temp);
        temp = temp->next;
    }
}

double DoublePop(list *head)
{
    list temp = *head;
    *head = (*head)->next;
    double X = temp->value;
    free(temp);
    return X;
}

char CharPop(list *head)
{
    list temp = *head;
    *head = (*head)->next;
    char X = temp->Character;
    free(temp);
    return X;
}

list pop(list *head)
{
    list temp = *head;
    *head = (*head)->next;
    free(temp);
}

list ListPop(list *head)
{
    list temp = *head;
    *head = (*head)->next;
    return temp;
}


int prio(char X)
{
    return X == '^' ? 3 : X == '*' || X == '/' ? 2
                      : X == '+' || X == '-'   ? 1
                                               : -1;
}


list Top(list head)
{
    return head;
}

char getChar(list head)
{
    return head->Character;
}

double getDouble(list head)
{
    return head->value;
}

bool isEmpty(list head)
{
    return head == NULL;
}

list infixToPosfix(char *expression)
{
    list OperatorStack = NULL;
    list ExpressionStack = NULL;
    int index = 0;
    while (expression[index] != '\0')
    {
        if (CharisOperator(expression[index]))
        {
            while (!isEmpty(OperatorStack) &&
                   prio(getChar(Top(OperatorStack))) >= prio(expression[index]))
            {
                push(&ExpressionStack, CreateNode(1, emptyDouble, CharPop(&OperatorStack)));
            }
            push(&OperatorStack, CreateNode(1, emptyDouble, expression[index]));
        }
        else if (expression[index] == '(')
        {
            push(&OperatorStack, CreateNode(1, emptyDouble, expression[index]));
        }
        else if (expression[index] == ')')
        {
            while (
                   getChar(Top(OperatorStack)) != '(')
            {
                push(&ExpressionStack, CreateNode(1, emptyDouble, CharPop(&OperatorStack)));
            }
            pop(&OperatorStack);
            
        }
        else
        {
            double float_value = 0.0;
            double power = 10;
            while (expression[index] != '\0' && !CharDetector(expression[index]) && expression[index] != '.')
            {
                float_value = float_value * power + (expression[index] - '0');
                index++;
            }
            while (expression[index] != '\0' && !CharDetector(expression[index]) && expression[index] == '.')
            {
                index++;
            }
            while (expression[index] != '\0' && !CharDetector(expression[index]))
            {
                float_value = float_value + (expression[index] - '0') / power;
                power *= 10;
                index++;
            }
            index--;
            push(&ExpressionStack, CreateNode(0, float_value, emptyChar));
        }
        index++;
    }

    while (!isEmpty(OperatorStack))
    {
        push(&ExpressionStack, ListPop(&OperatorStack));
    }

    while (!isEmpty(ExpressionStack))
    {
        push(&OperatorStack, ListPop(&ExpressionStack));
    }

    return OperatorStack;
}

double add(double x, double y)
{
    return x + y;
}

double min(double x, double y)
{
    return x - y;
}

double mul(double x, double y)
{
    return x * y;
}

double division(double x, double y)
{
    return x / y;
}

double power(double x , double y){
    return pow(x , y);
}

double getResult(double op1, double op2, char Operation)
{
    switch (Operation)
    {
    case '+':
        return add(op2, op1);
        break;

    case '-':
        return min(op2, op1);
        break;

    case '*':
        return mul(op2, op1);

    case '/':
        return division(op2, op1);
        break;
    case '^' : 
        return power(op2 , op1);
        break;
    }
        
}

bool isInt(double x){
    bool test ;
    int* temp = malloc(sizeof(int));
    *temp = (int) x;
    test = *temp == x ? true : false;
    free(temp);
    return test;
}

void printResult(double x){
    if(isInt(x)){
        printf("%d\n" , (int)x);
        return ;
    }
    printf("%lf\n" , x);
}

double solve(list head)
{
    list result = NULL;
    while (!isEmpty(head))
    {
        if (!isOperator(head))
        {
            push(&result, ListPop(&head));
        }
        else
        {
            double op1 = DoublePop(&result);
            double op2 = DoublePop(&result);
            push(&result, CreateNode(0, getResult(op1, op2, CharPop(&head)), emptyChar));
        }
    }

    return result->value;
}

int main()
{
    while (true)
    {
        printf("entrer une expression :");
        char* expression = malloc(100);
        scanf("%s" , expression);
        double result = solve(infixToPosfix(expression));
        printResult(result);
    }


    

    
    
    return 0;
}

