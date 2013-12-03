#include "lib.h"

ITEM boolval(ITEM item)
{
	ITEM new;
	new.type = TYPE_BOOL;
	if(item.type == TYPE_NULL)
	{
		new.value.valBool = false;
	}
	else if(item.type == TYPE_BOOL)
	{
		new.value.valBool = item.value.valBool;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		if(item.value.valInt == 0)
			new.value.valBool = false;
		else
			new.value.valBool = true;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		if(item.value.valDouble == 0.0)
			new.value.valBool = false;
		else
			new.value.valBool = true;
	}
	else if(item.type == TYPE_STRING)
	{
		if(strCompare(item.value.valString,""))
			new.value.valBool = false;
		else
			new.value.valBool = true;
	}
	else
	{
		new.type = TYPE_OTHER;
	}
	return new;
}

ITEM intval(ITEM item)
{
	ITEM new;
	new.type = TYPE_DIGIT_INT;
	if(item.type == TYPE_NULL)
	{
		new.value.valInt = 0;
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			new.value.valInt = 1;
		else
			new.value.valInt = 0;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		new.value.valInt = item.value.valInt;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		new.value.valInt = (int)item.value.valDouble;
	}
	else if(item.type == TYPE_STRING)
	{
		int size = strlen(item.value.valString.data);
		int i;
		int num;
		new.value.valInt = 0;
		
		for(i = 0; i < size; i++ )
		{
			if(isspace(item.value.valString.data[i]))
			{
			}
			else if(isdigit(item.value.valString.data[i]))
			{
				num = item.value.valString.data[i] - '0';
				new.value.valInt = new.value.valInt*10+num;
			}
			else
				return new;
		}
	}
	else
	{
		new.type = TYPE_OTHER;
	}
	return new;
}

ITEM doubleval(ITEM item)
{
	ITEM new;
	new.type = TYPE_DIGIT_DOUBLE;
	if(item.type == TYPE_NULL)
	{
		new.value.valDouble = 0.0;
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			new.value.valDouble = 1.0;
		else
			new.value.valDouble = 0.0;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		new.value.valDouble = (double)item.value.valInt;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		new.value.valDouble = item.value.valDouble;
	}
	else if(item.type == TYPE_STRING)
	{
		
		int i;
		int start = 0;
		int size = strlen(item.value.valString.data);
		
		for(i = 0; i < size; i++ )
		{
			if(isdigit(item.value.valString.data[i]))
			{
				start = i;
				break;
			}
		}
		
		
		STRING number;
		strInit(&number);
		
		int err;
// 		printf("%d, %d\n",start,size);
		number = get_substring(item.value.valString, start, size, &err);
		
		/*
		if(err !=0)
		{}//ERROR
		
		int state = 0;
		size = strlen(number.data);
		int maybe_plus_minus=0;
		for(i=0;i<size;i++)
		{
			char cur_char = number.data[i];
			switch (state)
			{
				case 0:
					if(isdigit(cur_char))
					{
					}
					else if(cur_char == '.')
					{
						state = 1;
					}
					else if((cur_char == 'e' || cur_char == 'E'))
					{
						state = 2;
						maybe_plus_minus = 1;
					}
					else
					{
						err = i;
						break;
					}
				break;
				case 1:
					if(isdigit(cur_char))
					{
					}
					else if((cur_char == 'e' || cur_char == 'E'))
					{
						state = 2;
						maybe_plus_minus = 1;
					}
					else
					{
						err = i;
						break;
					}
				break;
				case 2:
					if((cur_char == '+' || cur_char == '-') && maybe_plus_minus)
					{
						maybe_plus_minus = 0;
					}
					else if(isdigit(cur_char))
					{
						maybe_plus_minus = 0;
					}
					else
					{
						err = i;
						break;
					}
				break;
				default:
					err = 0;
					break;
				break;
			}
		}
		*/
		
		
// 		if(err != 0)
// 		{
// 			number = get_substring(item.value.valString, start-1, start+end, &err);
// 			if(err !=0)
// 			{}//ERROR
// 		}
		
// 		printf("cisclo: %s\n",number.data);
		
		
		double cislo;
		sscanf(number.data, "%lf", &cislo);
// 		printf("cisclo: %f\n",cislo);
		
		
		char pole[50];
		sprintf(pole,"%f",cislo);
		
// 		printf("text: %s\n",pole);
		
		strInitRaw(&new.value.valString, pole);
	}
	else
	{
		new.type = TYPE_OTHER;
	}
	return new;
}

ITEM strval(ITEM item)
{
	ITEM new;
	new.type = TYPE_STRING;
	
	if(item.type == TYPE_NULL)
	{
		strInitRaw(&new.value.valString,"");
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			strInitRaw(&new.value.valString,"1");
		else
			strInitRaw(&new.value.valString,"");
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		char* new_string = NULL;
		sprintf(new_string, "%d", item.value.valInt);
		strInitRaw(&new.value.valString, new_string);
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		char* new_string = NULL;
		sprintf(new_string, "%f", item.value.valDouble);
		strInitRaw(&new.value.valString, new_string);
	}
	else if(item.type == TYPE_STRING)
	{
		strInit(&new.value.valString);
		strCopy(&new.value.valString, &item.value.valString);
	}
	else
	{
		new.type = TYPE_OTHER;
	}
	return new;
}

//funkcni - OK
STRING get_string()
{
	STRING new;

	strInit(&new);
	char znak;
	while((znak = getchar()) != EOF)
	{
		if(znak == '\n')
			break;
		strAppend(&new, znak);
    }
	
	return new;
}

//funkcni - OK // parametr.data
int put_string(int count, ...)
{
    va_list list;
    int j = 0;

    va_start(list,count);

    for(j=0; j<count; j++)
    {
        printf("%s", va_arg(list, char*));
    }

   va_end(list);

   return count;
}

//funkcni - OK
int my_strlen(STRING text)
{
	return strUsed(&text);
}

//funkcni - OK
STRING get_substring(STRING text, int a, int b, int* ERR)
{
	STRING new;
	strInit(&new);
	
	*ERR=0;
	if(a < 0 || b < 0 || a > b || my_strlen(text) <= a || my_strlen(text) < b)
	{
		*ERR=13;
		return new;
	}

	int i;	
	for(i=a;i<b;i++)
		strAppend(&new,text.data[i]);
	return new;
}





