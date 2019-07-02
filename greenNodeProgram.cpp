#include "greenNode.h"

int main(int argc, char * argv[]){
	GreenNode gn;
	int option;
	while(true){
		printf("Select an option:\n");
		printf("1. Send file\n");
		printf("2. Does the file exist?\n");
		printf("3. Is the file complete?\n");
		printf("4. Get file\n");
		printf("5. Find file\n");
		printf("6. Delete file\n");
		
		scanf("%d", &option);

		switch(option){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				printf("Option %d is not valid\n", option);
				break;
		}
	}
	return 0;
}
