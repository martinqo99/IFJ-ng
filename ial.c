#include "ial.h"

// ----------- knuth moris pratt algorithm -----------
int find_string(STRING T, STRING P)
{
	// empty patern or string
	if(P.data[0] == '\0' || T.data[0] == '\0')
		return 0;
	
	int tmp = -1;
	int T_pos = 0;
	int* P_index = gcMalloc(sizeof(P.size));
	P_index[0] = tmp;

	for(unsigned int i=1;i<strlen(P.data);i++)
	{
		while((tmp > -1) && (P.data[tmp+1] != P.data[i]))
			tmp = P_index[tmp];
		if(P.data[tmp+1] == P.data[i])
			tmp++;
		P_index[i] = tmp;
	}

	tmp = -1;
	for(T_pos = 0; T_pos < (int)strlen(T.data); T_pos++)
	{
		while(tmp > -1 && P.data[tmp+1] != T.data[T_pos])
			tmp = P_index[tmp];
		if(P.data[tmp+1] == T.data[T_pos])
            tmp++;
		if(tmp == (int)strlen(P.data) - 1)
		{
            gcFree(P_index);
            return T_pos - tmp;
		}
	}
	gcFree(P_index);
	return -1;
}

void quicksort(STRING text, int left, int right)
{
	int i,j;
	int pm,tmp;
	i = left;
	j = right;
	
	pm = text.data[(i+j)/2];
	do
	{
		while(text.data[i] < pm)
			i = i + 1;
		while(text.data[j] > pm)
			j = j - 1;
		if(i <= j)
		{
			tmp = text.data[i];
			text.data[i] = text.data[j];
			text.data[j] = tmp;
			i = i + 1;
			j = j - 1;
		}
	}while(i < j);
	
	if(left < j)
		quicksort(text, left, j);
	if(i < right)
		quicksort(text, i, right);
}

void sort_string(STRING text)
{
	int left = 0;
	int right = strlen(text.data)-1;
	
	quicksort(text, left, right);
}