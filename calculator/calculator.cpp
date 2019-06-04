#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_MAX 1000
#define NODE_MAX 500
#define NUM_OPT 20
#define LEN_OPT 6
#define NUM_OPS 200
#define M_E 200
#define M_PI 200

typedef enum {
	Opd = 0,
	Opt = 1
}Type;

typedef struct {
	int opt;
	double opd;
	Type tp;
}Node;

char brackets[NUM_OPS];            //����ջ����������Ƿ�ƥ��
int bb = 0;
int input(Node *in);                              //��������ַ���������
int translate(char *p, Node *re, int *len);       //���ַ���ת���ɲ������Ͳ�����
double translateopd(char *p, int *len);            //ת��������
int translateopt(char *p, int *len);               //ͬ��
int cmplen(const char *s, const char *d);             //�Ƚϼ����ַ�������
int calculate(Node *n, double *r);                 //������ʽ��ֵ
int sopt(int opr, double *opd);                    //����Ŀ�����
int dopt(int opr, double *opd1, double opd2);      //����˫Ŀ�����
int prio(int c);                                   //�ж���������ȼ�

int input(Node *in) {	                 //  ���ʽ�����ת��   �������벢����Ϊ�������Ͳ�����������ʽ��
	//input->string	
	char ins[INPUT_MAX] = { 0 };
	int insi = 0;	char temp_in = 0;	int len = 0;
	while (' ' == (ins[0] = getchar()));		//��⿪ͷ�Ŀո�	
	if (ins[0] != '\n')		        //���س�	
	{
		do {
			temp_in = getchar();
			if (ins[insi] == ' '&&temp_in == ' ')			//���Զ���ո�				
				continue;
			ins[++insi] = temp_in;
		} while (temp_in != '\n');
		ins[insi] = 0;
	}
	insi = 0;
	if (ins[0] == '\n')			//����س�ֱ�����0	
		in->tp = Opd, in->opd = 0.0, in++;
	else
		//ѹ����ʽ��		
		while (ins[insi])
		{
			if (translate(ins + insi, in++, &len))
				insi += len;
			else
				return 0;
		}
	if ((in - 1)->opt != '=')        	//��ӵȺ�	
		in->opt = '=', in->tp = Opt, in++;
	in->opt = -1, in->tp = Opt;
	return 1;
}

int translate(char *p, Node *re, int *len)                //�����ַ���Ϊ�������������
{
	if (*p >= '0'&&*p <= '9')
	{
		re->tp = Opd;
		re->opd = translateopd(p, len);
		return 1;
	}
	else if (*p == 'e' || *p == 'p')
	{
		if (*p == 'e')
		{
			re->tp = Opd;
			re->opd = M_E;
			*len = 1;
			if (p[1] == ' ')
				(*len)++;	//ʮ����Ҫ���˴����������			
			return 1;
		}
		else
		{
			if (p[1] == 'i')
			{
				re->tp = Opd;
				re->opd = M_PI;
				*len = 2;
				if (p[2] == ' ')
					(*len)++;
				return 1;
			}
			else
				return 0;
		}
	}
	else
	{
		re->tp = Opt;
		re->opt = translateopt(p, len);
		if (re->opt)
			return 1;
		else
			return 0;
	}
}

double translateopd(char *p, int *len)            //���������
{
	int flag = 0;
	int pointnum = 1;
	double temp = 0;
	int i = 0;
	do {
		if (!flag&&p[i] != '.')
			temp = temp * 10 + p[i++] - '0';
		else if (!flag&&p[i] == '.')
			flag = 1, i++;
		else
			temp += (p[i++] - '0')*pow(0.1, pointnum), pointnum++;
	} while ((p[i] >= '0'&&p[i] <= '9') || p[i] == '.');
	if (p[i] == ' ')
		i++;
	*len = i;
	return temp;
}

int translateopt(char *p, int *len)              //���������
{
	char fu[NUM_OPT][LEN_OPT] = { "+","-","*","/","^","(",")","sin","cos","tan","asin","acos","atan","ln","lg","=" };
	int fu_int[NUM_OPT] = { '+','-','*','/','^','(',')','s','c','t','s' + 'a','c' + 'a','t' + 'a','n','g','=' };
	int i = 0;
	for (i = 0; i < NUM_OPT; i++)
	{
		if (*len = cmplen(p, fu[i]))
			break;
	}
	if (i == NUM_OPT)
		return 0;
	if (p[*len] == ' ')
		(*len)++;
	return fu_int[i];
}

int cmplen(const char *s, const char *d)         //�ַ����Ƚϣ������ַ�����
{
	char *dd = (char*)d;
	while (*d)
		if (*(d++) != *(s++))
			return 0;
	return d - dd;
}


int calculate(Node *n, double *r)             //���㺯�����
{	//˫��ջ	
	double OPRD[NUM_OPS] = { 0 };
	int OPRT[NUM_OPS] = { 0 };
	int db = 0;
	int tb = 0;
	int top = 0;
	double a = 0.0, b = 0.0;
	int o = 0;
	int len = 0;
	Node *nb = n;
	if (n->tp == Opt && n->opt == '-')
		OPRD[db] = 0.0, db++;
	while (1)
	{
		if (n->tp == Opd)				//�ж�ǰ����û�и���
		{
			OPRD[db] = n->opd, db++, n++;
		}
		else
		{						//˫Ŀ�����           
			if (prio(n->opt))
			{
				if (tb)
					top = OPRT[tb - 1];
				if (!tb)
				{
					if (n->opt == '=' || n->opt == ')')
					{
						if (n->opt == ')')
						{
							bb--;
							if (bb < 0)
							{
								printf("Bracket does not match!");
								return 0;
							}
						}
						break;
					}
					else
						OPRT[tb] = n->opt, tb++, n++;
					continue;
				}
				if (prio(top) == 0)
				{
					db--;
					a = OPRD[db];
					tb--;
					o = OPRT[tb];
					if (sopt(o, &a))
					{
						OPRD[db] = a, db++;
						continue;
					}
					else
						return 0;
				}
				if (prio(top) >= prio(n->opt))
				{
					db--;
					b = OPRD[db];
					db--;
					a = OPRD[db];
					tb--;
					o = OPRT[tb];
					if (dopt(o, &a, b))
					{
						OPRD[db] = a, db++;
					}
					else
						return 0;
				}
				else
				{
					OPRT[tb] = n->opt, tb++, n++;
				}
			}
			else
			{								//��Ŀ�����������                
				double x = 0.0;
				if (n->opt == '(')
				{
					brackets[bb] = '(', bb++;
					if (len = calculate(n + 1, &x))
					{
						OPRD[db] = x, db++;
						n += len, n++;
						if (n->tp == Opt && n->opt == -1)
						{
							printf("Bracket does not match!");
							return 0;
						}
					}
					else
						return 0;
				}
				else
				{
					OPRT[tb] = n->opt, tb++;
					n++;
				}
			}
		}
	}
	*r = OPRD[db - 1];
	return n - nb + 1;
}

int sopt(int opr, double *opd)            //��Ŀ����Ĵ���
{
	switch (opr)
	{
	case 's':
		*opd = sin(*opd);
		break;
	case 'c':
		*opd = cos(*opd);
		break;
	case 't':
		*opd = tan(*opd);
		break;
	case 'a' + 's':
		if (*opd < -1 || *opd>1)
		{
			printf("Beyond asin()!");
			return 0;
		}
		else
			*opd = asin(*opd);
		break;
	case 'a' + 'c':
		if (*opd < -1 || *opd>1)
		{
			printf("Beyond acos()!");
			return 0;
		}
		else
			*opd = acos(*opd);
		break;
	case 'a' + 't':
		if (*opd > -3.141592654 / 2 && *opd < 3.141592654 / 2)
			*opd = atan(*opd);
		else
		{
			printf("Beyond atan()!");
			return 0;
		}
		break;
	case 'n':
		if (*opd > 0)
			*opd = log(*opd);
		else
		{
			printf("Beyond ln()!");
			return 0;
		}
		break;
	case 'g':
		if (*opd > 0)
			*opd = log10(*opd);
		else
		{
			printf("Beyond lg()!");
			return 0;
		}
		break;
	}
	return 1;
}

int dopt(int opr, double *opd1, double opd2)           //˫Ŀ����Ĵ���
{
	switch (opr)
	{
	case '+':
		*opd1 = *opd1 + opd2;
		break;
	case '-':
		*opd1 = *opd1 - opd2;
		break;
	case '*':
		*opd1 = (*opd1)*opd2;
		break;
	case '/':
		if (opd2 > pow(0.1, 8) || opd2 < 0 - pow(0.1, 8))
			*opd1 = (*opd1) / opd2;
		else
		{
			printf("Error 1/0 !");
			return 0;
		}
		break;
	case '^':
		*opd1 = pow(*opd1, opd2);
		break;
	}
	return 1;
}

int prio(int c)                //�ж����ȼ�
{
	switch (c)
	{
	case '+':
	case '-':
	case '=':
	case ')':
		return 1;
		break;
	case '*':
	case '/':
		return 2;
		break;
	case '^':
		return 3;
		break;
	default:
		return 0;
		break;
	}
}
int main()
{
	int c = 0;
	while (1)
	{
		Node nodes[NODE_MAX] = { 0 };
		double r = 0.0;		//�������ջ		
		bb = 0;
		printf("����: ");		//����		
		if (!input(nodes))
		{
			printf("Input Error!");
			continue;
		}		//����		
		if (calculate(nodes, &r))
		{
			if (bb)
			{
				printf("Bracket does not match!");
				continue;
			}
			if (floor(r) == r)
				printf("%d", (int)r);
			else
				printf("%.4f", r);
		}
		c = getchar();
		if (c == 'q')	         //������������q�˳�			
			break;
		else if (c == 'c')	     //������������c����			
			system("clear");
		if (c != '\n')
			getchar();
	}
	printf("Quit...\n");
	return 0;
}



