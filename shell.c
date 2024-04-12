/*----------------------------------------------------------------

* Autor: Miguel Marines

* Proyecto: Shell

*--------------------------------------------------------------*/

// Librerias o biliotecas para implementar operaciones.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Constantes para el manejo del input de número de caracteres por linea, comandos y tokens.
#define MAX_INPUT_BUFFER 512
#define MAX_NUMBER_COMMANDS 100
#define MAX_NUMBER_TOKENS 50

// Constantes para el manejo de los espacios y saltos de linea.
#define SEPARATOR ";\n"
#define TOKEN_SEPARATOR " \n"


/* Tokeniza una linea y pone los elemnentos en un arreglo,
se asegura que el arreglo termine en NULL y regresa el número de tokens en el arreglo. */
int tokenize_line(char* tokens_line[], char* str, const char* delimiter)
{
	int count_tokens = 0;
	char* token = strtok(str, delimiter);

	while (token != NULL)
	{
		tokens_line[count_tokens++] = token;
		token = strtok(NULL, delimiter);
	}

	tokens_line[count_tokens] = NULL;

	return count_tokens;
}


// Crea tokens y procesos que ejecutan los comandos.
int execute_command(char *cmd)
{
	char* args[MAX_NUMBER_TOKENS];
	int executed_command_result;

	tokenize_line(args, cmd, TOKEN_SEPARATOR);
	
	const char* executable = args[0];

	pid_t child_PID = fork();
	
	if(child_PID == 0)
	{
		execvp(executable, args);
		printf("command not found: %s\n", executable);
		exit(0);
	}

	else
	{
		pid_t parent_PID;

		do
		{
			parent_PID = wait(&executed_command_result);
		}
		while(parent_PID != child_PID);

		return executed_command_result;
	}

	return executed_command_result;
}


/* Main donde se valida el número de parametros, se selecciona si el input es stdin o file, 
se seleciona modo interactivo o modo lote, además se llama a las funciones para ejecutar los
comandos introducidos por el usuario en el mini shell. */
int main(const int argc, const char* argv[])
{

	int interactive_shell = 1;
	int number_commands;

	char line[MAX_INPUT_BUFFER];
	char* commands[MAX_NUMBER_COMMANDS];
	char* output_commands;

	FILE* input;

	if(argc == 1)
	{
		input = stdin;
	}
	else if (argc == 2)
	{
		input = fopen(argv[1], "r");
		if(input == NULL)
		{
			printf("file: %s does not exist or cannot be opened\n", argv[1]);
			return -1;
		}
		interactive_shell = 0;
	}
	else
	{
		puts("error: wrong number of parameters");
		puts("to run the interactivie mini shell: ./shell");
		puts("to run the lot mini shell: ./shell fileName");
		return -1;
	}

	do
	{
	 	if(interactive_shell == 1)
	 	{
			printf("mini-shell> ");
		}
	 	
	 	output_commands = fgets(line, MAX_INPUT_BUFFER, input);
		
		if(strncmp("quit", line, 4) == 0 || output_commands == NULL)
		{
			break;
		}
		
		number_commands = tokenize_line(commands, line, SEPARATOR);
		
		for(int i = 0; i < number_commands; i++)
		{
		 	execute_command(commands[i]);
		}

	}
	while(output_commands != NULL);

	if(interactive_shell == 1)
	{
		puts("mini shell terminated and closed");
	} 

	return 0;
}
