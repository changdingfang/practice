 //***************************************************************
 // @file:    replace.cpp
 // @author:  dingfang
 // @date    2018-05-21 14:52:17
 //***************************************************************
 
///所有引用基类的地方必须能透明地使用其子类的对象.
# include <iostream>

using namespace std;

class Tmp
{
public:
	void tmp()
	{
		cout << "hello world  Tmp" << endl;
	}
};

class TmpSon
: public Tmp
{
public:
	void tmp()
	{
		cout << "hello world TmpSon" << endl;
	}
};

class Father
{
public:
	//void print(Tmp *tmp)
	//virtual void print(TmpSon *tmp)
	void print(TmpSon *tmp)
	{
		cout << "Father is runing..." << endl;
	}
	void f(int num)
	{
		cout << "Father f()" << endl;
	}
};

class Son
: public Father
{
public:
	using Father::print;// 子类扩展父类的方法 //如果不加的话, 子类不能对父类进行重载
	//using Father::f;
	//void print(TmpSon *tmp)
	void print(Tmp*tmp)
	{//有了上面的using Father::print ,  这里就可以重载父类的print方法
		cout << "Son is runing..." << endl;
	}
	void f(char num)
	{//上面没有using Father:;f;  ,则这里重写了父类的方法,  不会再调用父类的方法了
		cout << "Son f()" << endl;
	}
};

void print(Tmp *tmp)
{
	cout << "Tmp print " << endl;
}

void print(TmpSon *tmp)
{
	cout << "TmpSon print" << endl;
}

int main(void)
{
	Tmp *k = new Tmp();
	TmpSon *k1 = new TmpSon();
	Father *f = new Father();
	//f->print(k);
	f->print(k1);

	Son *s = new Son();
//	s->print(k);
	s->print(k1);


	int f1 = 0;
	//char ch = 'k';
	f->f(f1);
	s->f(f1);

	print(k1);
	print(k);

	delete k;
	delete k1;
	delete f;
	delete s;

	return 0;
}
