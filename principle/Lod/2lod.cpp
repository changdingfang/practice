 //***************************************************************
 // @file:    2lod.cpp
 // @author:  dingfang
 // @date    2018-05-23 17:41:26
 //***************************************************************
 //
 //朋友之间也不能无所不知,  也要有些距离, 
 
# include <stdlib.h>
# include <time.h>

# include <iostream>

using namespace std;

class Wizard
{
public:
	int install()
	{
		srand(time(NULL));
		if (first() > 50)
		{
			cout << "first success !" << endl;
			if (second() > 50)
			{
				cout << "second success !" << endl;
				if (third() > 50)
				{
					cout << "third success !" << endl;
					cout << "install success !" << endl;
					return 0;
				}
			}
		}
		cout << "install fail" << endl;
		return -1;
	}

private:
	//安装应用三步
	int first()
	{	return rand() % 100;	}

	int second()
	{	return rand() % 100;	}

	int third()
	{	return rand() % 100;	}
}; 
class Install
{
public:
	int install(Wizard *wizard)
	{
		//通过Wizard的install方法来调用 安装应用三步方法,  
		//而不是通过Install的install方法来调用Wizard的 安装应用三步方法, ..为了让Install知道更少的方法, 降低耦合
		//在以后修改的时候更加的安全和方便
		wizard->install();

		delete wizard;

		return 0;
	}
};

int main(void)
{
	Install *install = new Install();
	install->install(new Wizard());

	delete install;

	return 0;
}
