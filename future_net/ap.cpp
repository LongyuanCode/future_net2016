#include <iostream>
#include <algorithm>
#include <cstring>
#include <climits>
#include "route.h"
#include "ap.h"

using namespace std;

int apSum;

const int INF = 200000;

int s[verMax][verMax];//Ȩֵ
//int lx[verMax], ly[verMax]; //����
//int linky[verMax];//��¼��iƥ��Ķ���
bool visx[verMax], visy[verMax];
int slack[verMax];//�ɳ���

void init()
{
	memset(&root, 0, sizeof(judgeNode));
	memset(root.linky, -1, sizeof(root.linky)); //��¼��iƥ��Ķ���
	//memset(root.ly, 0, sizeof(root.ly)); ///��ʼ������yΪ0

	for (int i = 0; i < n; i++)
	{
		root.lx[i] = -INF;

		for (int j = 0; j < n; j++)
		{
			if (s[i][j] > root.lx[i])
			{
				root.lx[i] = s[i][j];    ///��ʼ������xΪ�붥��Xi�����ıߵ����Ȩ
			}
		}
	}
}

bool find(int x)//�������㷨
{
	visx[x] = true;
	int ni=node[x].outDeg + ((node[x].state & demandIndex) == 0);
	for (int i = 0; i < ni; i++)
	{
		int y = node[x].t[i];

		if (visy[y])
		{
			continue;
		}

		int t = root.lx[x] + root.ly[y] - s[x][y];//��t==0����Ϊ���Ȩƥ�䣻

		if (t == 0)
		{
			visy[y] = true;

			if (root.linky[y] == -1 || find(root.linky[y]))
			{
				root.linky[y] = x;
				return true;        //�ҵ������
			}
		}

		else
			if (slack[y] > t)
			{
				slack[y] = t;
			}
	}

	return false;                   //û���ҵ�����죨˵������xû�ж�Ӧ��ƥ�䣬���걸ƥ��(�����ͼ���걸ƥ��)������
}

void KM()                //��������ƥ���ֵ
{
	init();

	for (int i = 0; i < n; i++)
		if ((node[i].state & demandIndex) == 0)
		{
			visx[i] = true;
			visy[i] = true;
			root.linky[i] = i;
		}

	for (int m = 0; m < inSet; m++)
	{
		int x = vertexDemand[demandIndex - 1][m];

		for (int i = 0; i < n; i++)
		{
			slack[i] = INF;    //�ɳں�����ʼ��Ϊ�����
		}

		while (1)
		{
			memset(visx, 0, sizeof(visx));
			memset(visy, 0, sizeof(visy));

			if (find(x))                    //�ҵ�����죬�˳�
			{
				break;
			}

			int d = INF;

			for (int i = 0; i < n; i++)         //û�ҵ�����l������(������������ͼ�ı�)��������
			{
				if (!visy[i] && d > slack[i])
				{
					d = slack[i];
				}
			}

			for (int i = 0; i < n; i++) //�޸�x�Ķ���
			{
				if (visx[i])
				{
					root.lx[i] -= d;
				}
			}

			for (int i = 0; i < n; i++) //�޸�y�Ķ���
			{
				if (visy[i])
				{
					root.ly[i] += d;
				}
				else
				{
					slack[i] -= d;    //�޸Ķ���󣬲��ڽ������е�y�����slackֵ��Ҫ��ȥd��
				}
			}
		}

	}

	int result = 0;

	for (int i = 0; i < n; i++)
	{
		if (root.linky[i] > -1)
		{
			result += addWeigh + 100 - s[root.linky[i]][i];
		}
	}

	root.apSum=result;
}

void AP()
{
	memset(s, 0, sizeof(s));
	int visit[verMax] = {0};

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			if (dis[i][j] != 0)
			{
				s[i][j] = addWeigh + 100 - dis[i][j];
			}
			else
				if (i != j || node[i].state & demandIndex)
				{
					s[i][j] = addWeigh + 100-INF;
				}
				else
				{
					s[i][j] = addWeigh + 100;
				}
		}

	s[vertexDemand[0][1]][vertexDemand[0][0]] = addWeigh + 100;
	KM();
}

