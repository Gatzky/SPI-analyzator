#include "cli.h"
#include "usart.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// head of CLI command list
static CLI_CommandItem *head = NULL;

// char buffer where command will be stored
static char commandBuffer[100];

/**
 * This function searches the CLI command list and tries to find a descriptor for the provided command.
 * The command format is case-insensitive.
 * Returns pointer to @ref CLI_MenuItem structure if given command was found in the CLI command list.
 *
 * @param command pointer to command (string)
 *
 * @retval pointer to @ref CLI_MenuItem structure desrcibing the command, if command was found
 * @retval NULL if the given command does not match any command regitstered in the CLI command list 
 */
static CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command);

/**
 * @bref This function is responsible for collecting the command reading in from USART.
 *
 * This function should check wether the USART interface has some new data. If it does
 * this function reads new characters and stores them in a command buffer. This function
 * is also responsible for providing echo of the input characters back to the buffer.
 *
 * The function exits when:
 * - no more characters are available to read from USART - the function returns false
 * - new line was detected - this function returns true
 *
 * @retval true if command was collected
 * @retval false if command is yet incomplete
 */
static bool CLI_StoreCommand(void);

/**
 * @brief This function converts string to a lowercase
 *
 * @param dst pointer where converted null terminated string will be stored
 * @param src pointer to string which will be converted
 */
static void CLI_StringToLower(char *dst, const char *src);
	
	
	
void CLI_Proc(void){
	
	if (CLI_StoreCommand()) {
		char tempCommandBuffer[100];
		char args[100];
		char *token;
		char s [2] = " ";
		
		token=strtok(commandBuffer,s);
		strcpy(tempCommandBuffer,token);
		token=strtok(NULL,s);
		strcpy(args,token);
		
		CLI_CommandItem *item = CLI_GetMenuItemByCommandName(tempCommandBuffer);
		
		if (item) {
			item->callback(args);
		}
	}
}

bool CLI_AddCommand(CLI_CommandItem *item){
	if (item->callback == NULL || item->commandName == NULL){
		return false;
	}  

	item->next = NULL;
	if (head == NULL){
		head = item;
	} 
	else{
		CLI_CommandItem *temp=head;
		while(!(temp->next == NULL)){
			temp = temp->next;
		}
		temp->next = item;
	}
	return true;
}

void CLI_PrintAllCommands(void){
	CLI_CommandItem *temp=head;
	while(temp!=NULL){
		USART_WriteString(temp->commandName);
		temp = temp->next;
	}
}

CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
	CLI_CommandItem *temp=head;
	char tempCommand[100];
	CLI_StringToLower(tempCommand,command);
	while(temp!=NULL){
		char tempCommandName[100];
		
		CLI_StringToLower(tempCommandName,temp->commandName);
		if (strcmp(tempCommand, tempCommandName)==0){
			return temp;
		}
		temp = temp->next;
	}

	return NULL;
};

void CLI_StringToLower(char *dst, const char *src){
	uint16_t i=0;
	while(src[i]!=NULL){
		dst[i]=tolower(src[i]);
		i++;
	}
	dst[i] = 0;
}

bool CLI_StoreCommand(){
	char temp=0;
	static int i;
	
	if(USART_GetChar(&temp)) {
		commandBuffer[i]=temp;
		USART_PutChar(temp);
		if (temp == '\n' || temp == '\r') {
			commandBuffer[i]=0;
			i = 0;
			return true;
			
		}
		i++;
		if (i >= sizeof(commandBuffer)) {
			i = 0;
		}
	}
	return false;
}

