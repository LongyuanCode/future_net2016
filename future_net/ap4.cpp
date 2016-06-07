#include <iostream>
#include <algorithm>
#include <cstring>
#include <climits>
#include "route.h"
#include "ap.h"

using namespace std;

int apSum1;
const int INF = 200000;

int w[verMax][verMax];//Ȩֵ
//int lx1[verMax]={1098,1099,1099,1100,1100,1099}, ly1[verMax]={0,1,0,0,0,1}; //����
//int linky1[verMax]={3,-1,1,2,4,5};//��¼��iƥ��Ķ���
bool visx1[verMax], visy1[verMax];
int slack1[verMax];//�ɳ���
//int forb[verMax][2]={{0,1}};
//int must[verMax][2]={{2,3}};

void init1()
{
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			w[i][j]=s[i][j];
		}

	/*for (int i = 0; i < verNum; i++)
	{
		visx1[i] = 1;
		visy1[i] = 1;
	}*/

	for (int i = 0; i < n0->forbNum; i++)
	{
		w[n0->forb[i][0]][n0->forb[i][1]] -= INF;
		//visx1[n0->forb[i][0]]=0;
		//visy1[n0->forb[i][1]]=0;
	}

	for (int i = 0; i < n0->mustNum; i++)
	{
		for (int j = 0; j < verNum; j++)
		{
			if (j != n0->must[i][1])
			{
				w[n0->must[i][0]][j] -= INF;
			}
		}
	}
}

bool find1(int x) //�������㷨
{
	visx1[x] = true;
	int ni = node[x].outDeg + ((node[x].state & demandIndex) == 0);

	for (int i = 0; i < ni; i++) //node[x].outDeg+((node[x].state&demandIndex)==0)
	{
		int y = node[x].t[i]; //node[x].t[i];

		if (visy1[y])
		{
			continue;
		}

		int t = n0->lx[x] + n0->ly[y] - w[x][y];//��t==0����Ϊ���Ȩƥ�䣻

		if (t == 0)
		{
			visy1[y] = true;

			if (n0->linky[y] == -1 || find1(n0->linky[y]))
			{
				n0->linky[y] = x;
				return true;        //�ҵ������
			}
		}

		else
			if (slack1[y] > t)
			{
				slack1[y] = t;
			}
	}

	return false;                   //û���ҵ�����죨˵������xû�ж�Ӧ��ƥ�䣬���걸ƥ��(�����ͼ���걸ƥ��)������
}

void KM1()                //��������ƥ���ֵ
{
	init1();
	int x = n0->forb[n0->forbNum - 1][0];
	n0->linky[n0->forb[n0->forbNum - 1][1]]=-1;

	for (int i = 0; i < verNum; i++)
	{
		slack1[i] = INF;    //�ɳں�����ʼ��Ϊ�����
	}

	while (1)
	{
		memset(visx1, 0, sizeof(visx1));
		memset(visy1, 0, sizeof(visy1));

		if (find1(x))                   //�ҵ�����죬�˳�
		{
			break;
		}

		int d = INF;

		for (int i = 0; i < verNum; i++)         //û�ҵ�����l������(������������ͼ�ı�)��������
		{
			if (!visy1[i] && d > slack1[i])
			{
				d = slack1[i];
			}
		}

		for (int i = 0; i < verNum; i++) //�޸�x�Ķ���
		{
			if (visx1[i])
			{
				n0->lx[i] -= d;
			}
		}

		for (int i = 0; i < verNum; i++) //�޸�y�Ķ���
		{
			if (visy1[i])
			{
				n0->ly[i] += d;
			}
			else
			{
				slack1[i] -= d;    //�޸Ķ���󣬲��ڽ������е�y�����slackֵ��Ҫ��ȥd��
			}
		}
	}

	int result = 0;

	for (int i = 0; i < n; i++)
	{
		if (n0->linky[i] > -1)
		{
			result += addWeigh + 100 - w[n0->linky[i]][i];
		}
	}

	n0->apSum=result;
}

