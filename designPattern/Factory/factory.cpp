 //***************************************************************
 // @file:    factory.cpp
 // @author:  dingfang
 // @date    2018-05-24 20:31:03
 //***************************************************************
 
//工厂模式
//
//举例子,  女娲造人
//女娲造黑人, 白人, 黄人,
//通过八卦炉烧制成人

# include <iostream>

using namespace std;

//人类 抽象类
class Human
{
public:
	//人的肤色
	virtual void getColor() = 0;
	//人说设么语言
	virtual void talk() = 0;
};

//黑人
class BlackHuman
: public Human
{
	void getColor()
	{	cout << "黑人长得黑" << endl;	}

	void talk()
	{	cout << "黑人说黑话" << endl;	}	
};
//白人
class WhiteHuman
: public Human
{
public:
	void getColor()
	{	cout << "白人长得白" << endl;	}

	void talk()
	{	cout << "白人说白话" << endl;	}
};
//黄人 
class YellowHuman
: public Human
{
public:
	void getColor()
	{	cout << "黄人长得黄" << endl;	}

	void talk()
	{	cout << "换人说黄话" << endl;	}
};

//八卦炉抽象类
class AbstractHumanFactory
{
public:
	AbstractHumanFactory() {	}
	virtual ~AbstractHumanFactory() {	}

	virtual void createHuman(int ) = 0;
};

//生产人的工厂类
class HumanFctory
//: public AbstractHumanFactory
{
public:
	Human *createHuman(int type)
	{
		switch (type)
		{//根据传的type类型, 生产不同的人类
			//这个方法是每增加一个新的生产类, 都要在这里添加上去
			case 1:		return new BlackHuman();
			case 2:		return new WhiteHuman();
			case 3:		return new YellowHuman();
			default :	return NULL;
		}
	}
};

int main(void)
{
	HumanFctory *humanFactory = new HumanFctory();
	Human *humanBlock = humanFactory->createHuman(1);
	humanBlock->getColor();
	humanBlock->talk();

	Human *humanWhite = humanFactory->createHuman(2);
	humanWhite->getColor();
	humanWhite->talk();

	Human *humanYellow = humanFactory->createHuman(3);
	humanYellow->getColor();
	humanYellow->talk();

	return 0;
}
