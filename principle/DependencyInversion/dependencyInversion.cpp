 //***************************************************************
 // @file:    dependencyInversion.cpp
 // @author:  dingfang
 // @date    2018-05-21 18:10:28
 //***************************************************************
 
/////搞成模块不应该依赖底层模块, 两者都应该依赖其抽象
////抽象不应该依赖细节
/////细节应该依赖抽象

# include <iostream>
using std::cout;
using std::endl;

class ICar
{
public:
	virtual void car() = 0;
};

class BenZCar
: public ICar
{
public:
	void car()
	{
		cout << "Benz car runing..." << endl;
	}
};

class BMWCar 
: public ICar
{
public:
	void car()
	{
		cout << "BMW car runing..." << endl;
	}
};

class IDriver
{
public:
	virtual void driver(ICar *) = 0;
};

class Driver
: public IDriver
{
public:
	void driver(ICar *car)
	{
		cout << "drivering...." << endl;
		car->car();
	}
};

int main(void)
{
	Driver *dingfang = new Driver();
	ICar *benz = new BenZCar();
	ICar *bmw = new BMWCar();

	dingfang->driver(benz);
	dingfang->driver(bmw);

	return 0;
}
