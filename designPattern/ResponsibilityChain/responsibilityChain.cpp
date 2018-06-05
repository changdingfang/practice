 //***************************************************************
 // @file:    responsibilityChain.cpp
 // @author:  dingfang
 // @date    2018-06-05 18:02:09
 //***************************************************************
 
//责任链模式
//使用多个对象都有机会处理请求, 从而避免了请求的发送者和接收者之间的耦合关系
//. 将这些对象连成一条链, 并沿着这条链传递该请求, 知道有对象处理它为止.
/////
//例子  古代女人的三从
//女人想要做一件事, 
//在未嫁的情况下要听从父亲的安排, 父亲同意则可以去做
//在嫁了的情况下, 要听从丈夫的安排, 丈夫同意则可以去做
//在丈夫挂了的情况下, 要听从儿子的安排, 儿子同意了才可以去做
///////

# include <stdlib.h>
# include <time.h>

# include <iostream>
# include <list>

using namespace std;

const int FATHER_LEVEL_REQUEST = 1;
const int HUSBAND_LEVEL_REQUEST = 2;
const int SON_LEVEL_REQUEST = 3;

class IWomen
{
public:
	IWomen() {}
	virtual ~IWomen() {}

	virtual int getType() = 0;
	virtual string getRequest() = 0;

};

class Women
: public IWomen
{
public:
	Women(int type, string request)
	: type_(type)
	, request_(request)
	{}

	int getType()
	{	return type_;	}

	string getRequest()
	{	return request_;	}

private:
	int type_;
	string request_;
};

class Handler
{
public:
	Handler(int level)
	: level_(level)
	{}
	virtual ~Handler() {}

	void handleMessage(IWomen *women)
	{
		if (women->getType() == this->level_)
		{//该对象即是能够处理的对象
			this->response(women);
		}
		else
		{
			if (this->nextHandler_ != NULL)
			{//如果后续还有能处理的对象, 则继续往下走
				this->nextHandler_->handleMessage(women);
			}
			else
			{//后续没有能够处理的对象, 则表示不同意
				cout << "请示到最后也没有, 则表示不同意 !" << endl;
			}
		}
	}

	void setNext(Handler *handler) 
	{	this->nextHandler_ = handler;	}

	virtual void response(IWomen *) = 0;

protected:
private:
	Handler *nextHandler_;

	int level_;
}; 

class Father
: public Handler
{
public:
	Father()
	: Handler(FATHER_LEVEL_REQUEST)
	{}

	void response(IWomen *women)
	{
		cout << "--------------父亲---------------" << endl;
		cout << women->getRequest() << endl;
		cout << "父亲表示同意" << endl;
	}
};

class Husband
: public Handler
{
public:
	Husband()
	: Handler(HUSBAND_LEVEL_REQUEST)
	{}

	void response(IWomen *women)
	{
		cout << "--------------丈夫---------------" << endl;
		cout << women->getRequest() << endl;
		cout << "丈夫表示同意" << endl;
	}
};

class Son 
: public Handler
{
public:
	Son()
	: Handler(SON_LEVEL_REQUEST)
	{}

	void response(IWomen *women)
	{
		cout << "--------------儿子---------------" << endl;
		cout << women->getRequest() << endl;
		cout << "儿子表示同意" << endl;
	}
};


int main(void)
{
	list<IWomen *> listWomen;
	srand(time(NULL));
	for (int i = 0; i < 5; ++i)
	{
		listWomen.push_back(new  Women(rand() % 4 + 1, "上街买东西啊, 让不让去"));
	}
	Handler *father =  new Father();
	Handler *husband = new Husband();
	Handler *son = new Son();

	//写上责任链关系
	father->setNext(husband);
	husband->setNext(son);

	size_t len = listWomen.size();
	
	for (size_t i = 0; i < len; ++i)
	{
		IWomen *tmp = listWomen.back();
		father->handleMessage(tmp);
		listWomen.pop_back();
		delete tmp;
	}

	delete father;
	father = NULL;
	delete husband;
	husband = NULL;
	delete son;
	son = NULL;

	return 0;
}
