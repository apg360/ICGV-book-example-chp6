#include <stdio.h>
#include <stdlib.h>

//characters
void functionxyz_char(char**     outVariable)
{
    *outVariable = malloc( 2 * sizeof(char) );
    (*outVariable)[0] = 'A';
    (*outVariable)[1] = 'B';
    printf("Inside functionxyz_char \n");
    printf("theVariablechar[0] = %c \n",(*outVariable)[0]);
    printf("theVariablechar[1] = %c \n",(*outVariable)[1]);
    printf("Address[0] = %p \n", &((*outVariable)[0]));
    printf("Address[1] = %p \n", &((*outVariable)[1]));
}

//strings
void functionxyz_string(char***     outVariable)
{
    *outVariable = malloc( 2 * sizeof(char*) );
    (*outVariable)[0] = "A";
    (*outVariable)[1] = "B";
    printf("Inside functionxyz_string \n");
    printf("theVariablestring[0] = %s \n",(*outVariable)[0]);
    printf("theVariablestring[1] = %s \n",(*outVariable)[1]);
    printf("Address[0] = %p \n", &((*outVariable)[0]));
    printf("Address[1] = %p \n", &((*outVariable)[1]));
}

int main() {
    /*
    foo[x]     = *(foo + x)
    *(foo[x])  = *( *(foo + x) )
    (*foo)[x]  = *(  *foo + x  )
    
    if you do *outVariable[0] it works necause *outVariable[0] is basically **outVariable
    So &theVariable[1] is & (theVariable[1])  and not (&theVariable)[1]
    
    variable char    = character value									=> char x = 'A';
    variable char*   = character pointer / character array              => char *x = { 'A', 'B', 'C', 0 };
    function argument char**  = reference value to character array
    
    variable char*   = string                                           => char *y = "ABC";
    variable char**  = string pointer / string array					=> char **y = {"ABC", "DEF", "GHI", 123};
    function argument char*** = reference value to string array

    char**  reference to character array
    char*** reference to string array
    */
    
    char*   theVariablechar       = NULL;
    functionxyz_char(&theVariablechar);
   
    printf("after function return, Address[0] = %p \n", &theVariablechar[0]);
    printf("after function return, Address[1] = %p \n", &theVariablechar[1]);
    printf("theVariablechar[0] = %c \n",theVariablechar[0]);
    printf("theVariablechar[1] = %c \n",theVariablechar[1]);
    printf("\n --------- \n\n");
    
    char**   theVariablestring    = NULL;
    functionxyz_string(&theVariablestring);
    
    printf("after function return, Address[0] = %p \n", &theVariablestring[0]);
    printf("after function return, Address[1] = %p \n", &theVariablestring[1]);
    printf("theVariablestring[0] = %s \n",theVariablestring[0]);
    printf("theVariablestring[1] = %s \n",theVariablestring[1]);
    
    return 0;
}
/*
Inside functionxyz_char 
theVariablechar[0] = A 
theVariablechar[1] = B 
Address[0] = 0x5634e70382a0 
Address[1] = 0x5634e70382a1 
after function return, Address[0] = 0x5634e70382a0 
after function return, Address[1] = 0x5634e70382a1 
theVariablechar[0] = A 
theVariablechar[1] = B 

 --------- 

Inside functionxyz_string 
theVariablestring[0] = A 
theVariablestring[1] = B 
Address[0] = 0x5634e70386d0 
Address[1] = 0x5634e70386d8 
after function return, Address[0] = 0x5634e70386d0 
after function return, Address[1] = 0x5634e70386d8 
theVariablestring[0] = A 
theVariablestring[1] = B
*/
