 //***************************************************************
 // @file:    2singleton.cpp
 // @author:  dingfang
 // @date    2018-05-24 16:09:11
 //***************************************************************
 
//产生多个实例,有上限
//生产多个皇帝实例

# include <stdlib.h>
# include <time.h>
# include <pthread.h>

# include <iostream>
# include <vector>
# include <string>
# include <sstream>

using namespace std;

class Emperor;

//皇帝的最大数量 
const int MaxEmperorNum = 4;

//存放每一个皇帝实例
vector<Emperor *> emperorVec;

class Emperor
{
public:
	static Emperor *getInstance()
	{
		//保证线程安全
		pthread_once(&once_, EmperorList);
		//随机返回某个皇帝
		return emperorVec[rand() % 4];
	}

	void say()
	{
		cout << name_ << endl;
	}

private:
	Emperor() {}
	Emperor(string name)
	: name_(name)
	{}
	~Emperor()
	{
		int len = emperorVec.size();
		for (int i = 0; i < len; ++i)
		{
			delete emperorVec.back();
			emperorVec.pop_back();
		}
	}

	static void EmperorList()
	{
		for (int i = 0; i < MaxEmperorNum; ++i)
		{
			stringstream ss;
			//让每个皇帝的名字都不一样
			ss << "皇帝" << i; 
			emperorVec.push_back(new Emperor(ss.str()));
		}
	}
	
private:
	string name_;
	static pthread_once_t once_;
};

pthread_once_t Emperor::once_ = PTHREAD_ONCE_INIT;

int main(void)
{
	srand(time(NULL));

	for (int i = 0; i < MaxEmperorNum; ++i)
	{
		Emperor *emperor = Emperor::getInstance();
		emperor->say();
	}

	return  0;
}
