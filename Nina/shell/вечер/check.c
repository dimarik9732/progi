// Ф-я проверки
int check(string_list *list)
{
	int state = 0, count_skob = 0, c;

	while(list)
	{
		c = list->word[0];

		switch(state)
		{
			case 0: //Нач
				switch(c)
				{
					case 1:
					case 2:
					case 3:
						state = 1;
						break;
					case 8:
						count_skob++;
						break;
					case 9:
						count_skob--;
						break;
				}

			case 1: // Ожидание имени
				if(c <= 10)
				{
					printf("Не хватает имени\n");
					return 1;
				}
				else 
					state = 2;
				break;

			case 2: // Ожидание имени или | или && или || или ; или &
				switch(c)
				{
					case 8:
						count_skob++;
						break;
					case 9:
						count_skob--;
						break;
				}
				break;
		}

		list = list->next;
	}
		
	if(count_skob != 0)
	{
		printf("Не закрыли все скобки\n");
		return 1;
	}

	return 0;
}