 //***************************************************************
 // @file:    abstractFactory.cpp
 // @author:  dingfang
 // @date    2018-05-25 17:33:17
 //***************************************************************
 
//抽象工厂模式
////
//还是女娲造人
//增加性别属性
# include <iostream>

using namespace std;

class Human
{
public:
	Human() {}
	virtual ~Human() {}

	//人类的肤色
	virtual void getColor() = 0;

	//人类的语言
	virtual void talk() = 0;

	//人类的性别
	virtual void getSex() = 0;
};

//黑人抽象类 
class AbstractBlackHuman
: public Human
{
public:
	void getColor()
	{	cout << "黑人是黑色的皮肤" << endl;	}

	void talk()
	{	cout << "黑人说啥?" <<endl;	}
};

//白人抽象类 
class AbstractWhiteHuman
: public Human
{
public:
	void getColor()
	{	cout << "白人的皮肤是白色的" << endl;	}

	void talk()
	{	cout << "白人说hello world" <<endl;	}
};

//黄人抽象类
class AbstractYellowHuman
: public Human
{
public:
	void getColor()
	{	cout << "黄人的皮肤是黄色的" << endl;	}

	void talk()
	{	cout << "黄人说 你好?" <<endl;	}
};

//黑男人
class MaleBlackHuman
: public AbstractBlackHuman
{
public:
	void getSex()
	{	cout << "男人" << endl;	}
};

//黑女人
class FemaleBlackHuman
: public AbstractBlackHuman
{
public:
	void getSex()
	{	cout << "女人" << endl;	}
};

//白男人
class MaleWhiteHuman
: public AbstractWhiteHuman
{
public:
	void getSex()
	{	cout << "男人" << endl;	}
};

//白女人
class FemaleWhiteHuman
: public AbstractWhiteHuman
{
public:
	void getSex()
	{	cout << "女人" << endl;	}
};

//黄男人
class MaleYellowHuman
: public AbstractYellowHuman
{
public:
	void getSex()
	{	cout << "男人" << endl;	}
};

//黄女人
class FemaleYellowHuman
: public AbstractYellowHuman
{
public:
	void getSex()
	{	cout << "女人" << endl;	}
};

//八卦炉抽象类
class HumanFactory
{
public:
	HumanFactory() {}
	virtual ~HumanFactory() {}

	//创建黑种人的工厂
	virtual Human *createBlackHuman() = 0;

	//创建白种人的工厂
	virtual Human *createWhiteHuman() = 0;
	
	//创建黄种人的工厂
	virtual Human *createYellowHuman() = 0;
};

//生产男人的工厂具体类
class MaleFactory
: public HumanFactory
{
public:
	Human *createBlackHuman()
	{	return new MaleBlackHuman();	}

	Human *createWhiteHuman()
	{	return new MaleWhiteHuman();	}

	Human *createYellowHuman()
	{	return new MaleYellowHuman();	}
};

//生产女人的工厂具体类

class FemaleFactory
: public HumanFactory
{
public:
	Human *createBlackHuman()
	{	return new FemaleBlackHuman();	}

	Human *createWhiteHuman()
	{	return new FemaleWhiteHuman();	}

	Human *createYellowHuman()
	{	return new FemaleYellowHuman();	}
};

int main(void)
{
	//女人制造工厂
	HumanFactory *maleFactory = new MaleFactory();
	//男人制造工厂
	HumanFactory *femalefactory = new FemaleFactory();

	//造一个黑男人
	Human *maleBlack = maleFactory->createBlackHuman();
	maleBlack->getColor();
	maleBlack->talk();
	maleBlack->getSex();

	//造一个黑女人
	Human *femaleBlack = femalefactory->createBlackHuman();
	femaleBlack->getColor();
	femaleBlack->talk();
	femaleBlack->getSex();

	//############################
	//白种人和黄种人制造黑种人的过程是一模一样的, 只是换了调用的方法
	//############################

	return 0;
}
