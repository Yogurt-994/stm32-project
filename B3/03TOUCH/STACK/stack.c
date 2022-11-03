#include<stdio.h>
#include<stdlib.h>
#include<lcd.h>
#include<malloc.h>
#include<math.h>
#include<stack.h>
//Õ»
struct dataStack
{
	int data[max];
	int top;//Ö¸ÕëÖ¸ÏòÕ»¶¥µØÖ·
};

struct symStack
{
	char symbol[max];
	int top;
};


int X;
//Õ»µÄ³õÊ¼»¯
void InitDataStack(struct dataStack* Stacknum)
{
	Stacknum->top = -1;
}

void InitSymStack(struct symStack* Stacksym)
{
	Stacksym->top = -1;
}


//ÈëÕ»
void Popdata(struct dataStack* Stacknum, int num)
{
	Stacknum->top++;
	Stacknum->data[Stacknum->top] = num;
}

void Popsym(struct symStack* Stacksym, char sym)
{
	Stacksym->top++;
	Stacksym->symbol[Stacksym->top] = sym;
}


//³öÕ»
int Pushdata(struct dataStack* Stacknum)
{
	int x = Stacknum->data[Stacknum->top--];
	return x;
}

char Pushsym(struct symStack* Stacksym)
{
	char c = Stacksym->symbol[Stacksym->top--];
	return c;
}


//²é¿´Õ»¶¥
int loaddata(struct dataStack* Stacknum)
{
	return Stacknum->data[Stacknum->top];
}

char loadsym(struct symStack* Stacksym)
{
	return Stacksym->symbol[Stacksym->top];
}


//ÓÅÏÈ¼¶
int sympriority(char ch)
{
	if (ch == '(')  return 1;
	else if (ch == '+' || ch == '-')  return 2;
	else if (ch == '*' || ch == '/')  return 3;
	else if (ch == ')')  return 4;
	return 0;
}


//ÔËËã
int math(int x, int y,char ch)
{
	int sum = 0;
	switch (ch)
	{
	case'+':sum = x + y; break;
	case'-':sum = x - y; break;
	case'*':sum = x * y; break;
	case'/':sum = x / y; break;
	}
	return sum;
}


int STACKS(char *t)
{
	struct dataStack data;
	struct symStack symbol;
	InitDataStack(&data);
	InitSymStack(&symbol);
	int i, j;
	float sum, x, y;
	char *ch=t;
	i = 0, j = 0, sum = 0;
	char v[max] = { 0 };//Ôİ´æ×Ö·û×év
	char str[max];
	char *c=str;
	for(i=0;*(ch+i)!='\0';i++)
	{
		*(c+i)=*(ch+i);
	}
	str[i] = '\0';//Ä©Î²¼ÓÖÕÖ¹·ûºÅ
	for (i = 0; str[i] != '\0'; i++)//ÔËĞĞµ½×Ö·û´®Ä©Î²½áÊøÑ­»·
	{
		if (i == 0 && str[i] == '-')
		{
			v[j++] = str[i];
		}
		else if (str[i] == '(' && str[i + 1] == '-')
		{
			i++;
			v[j++] = str[i++];
			while ((str[i] >= '0' && str[i] <= '9')||str[i]=='.')
			{
				v[j] = str[i];
				j++; i++;
			}
			Popdata(&data, atof(v));//atofº¯ÊıÉ¨Ãè×Ö·ûÖĞµÄintÊı×ÖÊä³ö£
			while (j > 0)//Çå¿ÕÔİ´æ×Ö·û×év
			{
				v[j] = 0;
				j--;
			}
			if (str[i] != ')')
			{
				i--;
				Popsym(&symbol, '(');
			}
		}
		else if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
		{
			while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
			{
				v[j] = str[i];
				i++; j++;
			}
			Popdata(&data, atof(v));
			while (j > 0)//Çå¿ÕÔİ´æ×Ö·û×év
			{
				v[j] = 0;
				j--;
			}
			i--;
		}
		else
		{
			//Èô¿ÕÕ»
			if (symbol.top == -1)
			{
				Popsym(&symbol, str[i]);
			}
			//ÈôÎª·ûºÅ'('
			else if (sympriority(str[i]) == 1)
			{
				Popsym(&symbol, str[i]);//ÈëÕ»
			}
			//ÈôÎª·ûºÅ'+'or'-'
			else if (sympriority(str[i]) == 2)
			{
				if (sympriority(loadsym(&symbol)) == 1)//ÈôÎªÕ»¶¥'('ÈëÕ»
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 2)//ÈôÕ»¶¥Îª'+'£¬'-'³öÕ»ÔËËã
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sumÈëÊı¾İÕ»
					}
					Popsym(&symbol, str[i]);//ĞÂ·ûºÅÈë·ûºÅÕ»
				}
				else if (sympriority(loadsym(&symbol)) == 3)//ÈôÕ»¶¥Îª'*'£¬'/'³öÕ»ÔËËã
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sumÈëÊı¾İÕ»
					}
					Popsym(&symbol, str[i]);//ĞÂ·ûºÅÈë·ûºÅÕ»
				}
			}
			//ÈôÎª'*'or'/'
			else if (sympriority(str[i]) == 3)
			{
				if (sympriority(loadsym(&symbol)) == 1)//ÈôÎªÕ»¶¥'('ÈëÕ»
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 2)//ÈôÕ»¶¥Îª'+'£¬'-'³öÕ»ÔËËã
				{
					Popsym(&symbol, str[i]);
				}
				else if (sympriority(loadsym(&symbol)) == 3)//ÈôÕ»¶¥Îª'*'£¬'/'³öÕ»ÔËËã
				{
					while (symbol.top >= 0 && data.top >= 1)
					{
						x = Pushdata(&data);
						y = Pushdata(&data);
						sum = math(y, x, Pushsym(&symbol));
						Popdata(&data, sum);//sumÈëÊı¾İÕ»
					}
					Popsym(&symbol, str[i]);//ĞÂ·ûºÅÈë·ûºÅÕ»
				}
			}
			//ÈôÎª')'
			else if (sympriority(str[i]) == 4)
			{
				while (sympriority(loadsym(&symbol)) != 1)//Ñ­»·³öÕ»Ö±µ½³öÏÖ'('
				{
					x = Pushdata(&data);
					y = Pushdata(&data);
					sum = math(y, x, Pushsym(&symbol));
					Popdata(&data, sum);//sumÈëÊı¾İÕ»
				}
				Pushsym(&symbol);//´ËÊ±Ö¸ÕëÖ¸Ïò'(',Ê¹Æä³öÕ»
			}
		}

	}

	while (symbol.top != -1)
	{
		x = Pushdata(&data);
		y = Pushdata(&data);
		sum = math(y, x, Pushsym(&symbol));
		Popdata(&data, sum);//sumÈëÊı¾İÕ»
	}
	
	char *a=(char*)malloc(sizeof(char));//¿ª±Ù¶¯Ì¬Êı×é£¬·ÀÖ¹ÎŞ·¨Êä³ö×îºó½á¹û
	int l = Pushdata(&data);
	X=l;
	printf("\t[½á¹û]result:%d\r\n",X);
	sprintf(a,"%d",l);//×ªintÎª×Ö·û´®£¬Êä³ö
	LCD_ShowString(2,80,200,24,24,(u8*)a);//ÏÔÊ¾Êä³ö
	free(a);
	free(str);
	return 0;
}
