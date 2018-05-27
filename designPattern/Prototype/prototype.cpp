 //***************************************************************
 // @file:    prototype.cpp
 // @author:  dingfang
 // @date    2018-05-27 12:40:43
 //***************************************************************
 
//原型模式
//用原型实例指定创建对象的种类, 并且通过拷贝这些原型创建新的对象.
/////

# include <iostream>

using namespace std;

class Prototype
{
public:
	Prototype() {}
	virtual ~Prototype() {}

	//复制自身
	virtual Prototype *clone() = 0;
};

class ConcretePrototype
: public Prototype
{
public:
	ConcretePrototype() {}
	~ConcretePrototype() {}

	//拷贝构造函数
	ConcretePrototype(const ConcretePrototype &rhs)
	{	this->counter_ = rhs.counter_;	}

	///复制自身
	Prototype *clone()
	///调用拷贝构造函数
	{	return new ConcretePrototype(*this);	}

private:
	int counter_;
};

int main(void)
{
	//生成对象
	Prototype *conProA = new ConcretePrototype();

	//复制自身
	Prototype *conProB = conProA->clone();

	delete conProA;
	delete conProB;
	conProA = NULL;
	conProB = NULL;

	return 0;
}
