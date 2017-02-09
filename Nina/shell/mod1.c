#include <stdlib.h>
#include <stdio.h>


#define change_state_from_null(newstate, spec) \
	i = 0; \
	tmp = calloc(1, sizeof(string_list)); \
	tmp->word = calloc(word_size, sizeof(char)); \
	if (spec) { tmp->word[0] = spec; i++; } \
	tmp->word[i] = c; \
	tmp->word[i+1] = '\0'; \
	i++; \
	result = tmp; \
	state = newstate; \
	break


#define error_lexem do \
{ \
	printf("Неожиданная лексема\n"); \
	return NULL; \
} while (0)


#define change_state(newstate, spec) \
	tmp->next = calloc(1, sizeof(string_list)); \
	tmp = tmp->next; \
	word_size = 5; \
	tmp->word = calloc(word_size, sizeof(char)); \
	i = 0; \
	if (spec) { tmp->word[0] = spec; i++; } \
	tmp->word[i] = c; \
	tmp->word[i+1] = '\0'; \
	i++; \
	state = newstate; \
	break


typedef struct string_list
{
	char *word;
	struct string_list *next;

}string_list;


// Ф-я, разбивающая строку на список лексем
string_list *parse()
{
	int word_size = 5;
	string_list *tmp = NULL;
	string_list *result = NULL;	
	
	int c = getchar();
	
	int i, state = 0;
	for (i = 0; c != EOF && c != '\n';)
	{
		switch(state)
		{
			case 0:

				switch(c)
				{
					case '<':
						change_state_from_null(4, 3);
					case '>':
						change_state_from_null(4, 1);
					case '(':
						change_state_from_null(5, 8);
					case '&':
					case '|':
					case ';':
					case ')':
						error_lexem;
					default:
						if(isspace(c))
							break;
						else 
							change_state_from_null(2, 0);
				}
				break;
			
			case 1:

				switch(c)
				{
					case ')':
						change_state(6, 9);
					case '(':
						change_state(5, 8);
					case '>':
						change_state(4, 1);
					case '<':
						change_state(4, 3);
					case '&':
						change_state(3, 4);
					case '|':
						change_state(3, 6);
					case ';':
						change_state(3, 10);
					default:
						if(isspace(c))
							break;
						else 
							change_state(2, 0);
				}
				break;		

			case 2:

				switch(c)
				{
					case ')':
						change_state(6, 9);
					case '(':
						change_state(5, 8);
					case '>':
						change_state(4, 1);
					case '<':
						change_state(4, 3);
					case '&':
						change_state(3, 4);
					case '|':
						change_state(3, 6);
					case ';':
						change_state(3, 10);
					default:
						if(isspace(c))
							state = 1;
						else
						{
							if(i+1 >= word_size)
							{
								word_size *= 2;
								tmp->word = realloc(tmp->word, word_size*sizeof(char));
							}
							tmp->word[i] = c;
							tmp->word[i+1] = '\0';
							i++;
						}
				}
				break;

			case 3:

				switch(tmp->word[1])
				{
					case '&':
						switch(c)
						{
							case '|':
							case ';':
								error_lexem;
							case '&':
								if(i == 3)
									error_lexem;
								else
								{
									tmp->word[0] = 5;	
									tmp->word[2] = c;
									i++;
									break;
								}
							case ')':
								change_state(6, 9);
							case '>':
								change_state(4, 1);
							case '<':
								change_state(4, 3);
							default:
								if(isspace(c))
								{
									state = 1;
									break;
								}
								else
									change_state(2, 0);
						}
						break;

					case '|':
						switch(c)
						{
							case ';':
							case ')':
								error_lexem;
							case '|':
								if(i == 3)
									error_lexem;
								else
								{
									tmp->word[0] = 7;	
									tmp->word[2] = c;
									i++;
									break;
								}
							case '>':
								change_state(4, 1);
							case '<':
								change_state(4, 3);
							default:
								if(isspace(c))
								{
									state = 1;
									break;
								}
								else
									change_state(2, 0);
						}
						break;

					case ';':
						switch(c)
						{
							case '&':
							case '|':
							case ';':
								error_lexem;
							case ')':
								change_state(6, 9);
							case '>':
								change_state(4, 1);
							case '<':
								change_state(4, 3);
							default:
								if(isspace(c))
								{
									state = 1;
									break;
								}
								else
								change_state(2, 0);
						}
						break;

					default: 
						error_lexem;
				}
				break;


			case 4:

				switch(tmp->word[1])
				{
					case '>':
						if(i == 3)
							error_lexem;
						switch(c)
						{
							case '>':
								change_state(4, 2);
							case '&':
								change_state(3, 4);
							case '|':
								change_state(3, 6);
							default:
								if(isspace(c))
								{
									state = 1;
									break;
								}
								else
									change_state(2, 0);
						}
						break;

					case '<':
						switch(c)
						{
							case '|':
							case ';':
								error_lexem;
							case '<':
								if(i == 5)
									error_lexem;
								else
									change_state(4, 3);
							case '>':
								change_state(4, 1);

							default:
								if(isspace(c))
								{
									state = 1;
									break;
								}
								else
									change_state(2, 0);
						}
						break;
						
					default:
						error_lexem;
				}
				break;



			case 5:
			case 6:
				switch(c)
				{
					case '(':
						change_state(5, 8);
					case ')':
						change_state(6, 9);
					case '>':
						change_state(4, 1);
					case '<':
						change_state(4, 3);
					case '&':
						change_state(3, 4);
					case '|':
						change_state(3, 6);
					case ';':
						change_state(3, 10);
					default:
						if(isspace(c))
							{
								state = 1;
								break;
							}
							else
								change_state(2, 0);
				}
				break;

			
		}
				
		c = getchar();

	}

	return result;
}

// Ф-я, печатающая и затем удаляющая список
int print_list(string_list *list)
{
	string_list* next;
	while(list)
	{
		if(list->word[0] < 11)
			puts(list->word+1);
		else
		printf("%s\n", list->word);
		next = list->next;
		free(list);
		list = next;
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	while(1){
		printf("Введите строку\n");
		string_list *tmp = parse();
		if (tmp)
			print_list(tmp);
	}
	return 0;
}

