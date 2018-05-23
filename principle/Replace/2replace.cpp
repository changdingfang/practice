 //***************************************************************
 // @file:    replace.cpp
 // @author:  dingfang
 // @date    2018-05-21 14:52:17
 //***************************************************************
 
# include <iostream>

using namespace std;

class Father
{
public:
	virtual ~Father() {	}

	virtual void print()
	{
		cout << "Father is runing..." << endl;
	}
};

class Son
: public Father
{
public:
	void print()
	{
		cout << "Son is runing..." << endl;
	}
};

# if 1
void print(Father *f)
{
	f->print();
	cout << ">>>print function<<<" << endl;
}
# endif

int main(void)
{
	Father *f = new Son();
	f->print();
	print(f);

	cout << endl;

	Son *s = new Son();
	s->print();
	print(s);

	delete f;
	delete s;

	return 0;
}
