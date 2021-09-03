#include <stdio.h>
#include <stdlib.h>
int week1,week7;
//输出某年日历
int main()
{
	int i,j;
	int year,sum;//year输入年份，sum与2017.1.1之间差的天数，week星期几
	int a[13]={31,28,31,30,31,30,31,31,30,31,30,31,29};
	do {
	printf("please input the year whose calender you want to know:");
	scanf("%d",&year);
	if (!year)
	{
		break;
	}
	sum=disofyear(year,2017);
	if(year>2017)
	{
		week1=sum%7;
	}else{
		week1=(7-sum%7)%7;
	}
	week7=(week1+181+leap(year))%7;
	printf("|------------------- The Calender Of Year %4d -------------------|\n",year);
	for(i=1;i<=6;i++)
	{
		j=i+6;
		printf("| %2d SUN MON TUE WED THU FRI SAT  %2d  SUN MON TUE WED THU FRI SAT |\n",i,j);
		print(i==2&&leap(year)?a[12]:a[i-1],a[j-1]);
	}
	printf("|-----------------------------------------------------------------|\n");
	} while (1);
	system("pause");
	return 0;
}

//年之间差的天数
int disofyear(int a,int b)
{
	int i,sub,sum=0,next;//sub年数差，sum差的总天数,next下一年年数
	int qian=a,hou=b,t;
	if(qian>hou)
	{
		t=qian;
		qian=hou;
		hou=t;
	}
	next=qian;
	sub=hou-qian;
	for(i=0;i<sub;i++,next++)
	{
		sum=sum+365+leap(next);
	}
	return sum;
}

//判断是否为闰年，是1，否0
int leap(int year)
{
	int a3;
	a3=(year%4==0&&year%100!=0)||year%400==0;
	return a3;
}

//日历输出
int print(int day1,int day2)
{
	int i,j,n1,n2;
	n1=n2=1;
	i=1;
	while(n1<=day1||n2<=day2)
	{
		printf("|    ");//2+3
		for(j=0;j<7;j++)//开始一行
		{
			if(i==1&&j<week1||n1>day1)
			{
				printf("    ");//4
			}else{
				printf(" %2d ",n1);
				week1=++week1%7;
				n1++;
			}
		}
		printf("    ");//4
		for(j=0;j<7;j++)
		{
			if(i==1&&j<week7||n2>day2)
			{
				printf("    ");//4
			}else{
				printf(" %2d ",n2);
				week7=++week7%7;
				n2++;
			}
		}
		printf(" |\n");
		i++;
	}
	return 0;
}
