 //***************************************************************
 // @file:    singleton.cpp
 // @author:  dingfang
 // @date    2018-05-24 15:33:44
 //***************************************************************
 
//单例模式
//举例子  皇帝(唯一)
//
# include <iostream>

using namespace std;

//皇帝类
class Emperor
{
public:
	static Emperor *getInstance()
	{
		if (NULL == emperor_)
		{
			emperor_ = new Emperor();
		}
		return emperor_;
	}

	//皇帝发话
	void say()
	{
		cout << "我是皇帝, 你们都要听我的" << endl;
	}

private:
	//构造函数私有化, 防止多个实例产生
	Emperor() {	}
	~Emperor() 
	{
		if (NULL != emperor_)
		{
			delete emperor_;
			emperor_ = NULL;
		}
	}

private:
	static Emperor *emperor_;
};

///饿汉模式 , 保证线程安全
Emperor *Emperor::emperor_ = new Emperor();
//Emperor *Emperor::emperor_ = Emperor::getInstance(); 
int main(void)
{
	//Emperor *emperor = new Emperor();//已经把构造函数设置为私有,就不能在外部创建对象
	Emperor *emperor = Emperor::getInstance();
	emperor->say();

	return 0;
}
