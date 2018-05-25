 //***************************************************************
 // @file:    templateMethodPattern.cpp
 // @author:  dingfang
 // @date    2018-05-25 20:39:04
 //***************************************************************
 
//模板方法模式
//
//定义一个操作中的算法的框架, 而将一些步骤延迟到子类中, 使得子类可以不改变
//一个算法的结构即可重定义该算法的某些特定步骤
////
//例子 :   制造悍马模型
////制造悍马H1模型和悍马H2模型

# include <iostream>

using namespace std;

//悍马模型抽象类
class HummerModel
{
public:
	//需要启动
	virtual void start() = 0;

	// 需要停止
	virtual void stop() = 0;

	// 需要有喇叭
	virtual void alarm() = 0;

	// 需要有引擎声音
	virtual void engineBoom() = 0;

	// 需要会跑
	void run()
	{//悍马H1模型和悍马H2模型都有相同的这样run操作
	//所以就不定义为纯虚函数了, 在这里当模板使用
	//这...模板方法模式
		//先启动
		this->start();
		//会听到引擎作响
		this->engineBoom();
		//遇到人需要鸣笛提示
		this->alarm();
		//到达目的地要停止
		this->stop();
	}
};

//悍马H1模型类
class HummerH1Model
: public HummerModel
{
public:
	void start()
	{	cout << "悍马H1启动..." << endl;	}

	void stop()
	{	cout << "悍马H1停止." << endl;	}

	void alarm()
	{	cout << "悍马H1发出哔哔的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "悍马H1引擎嗡嗡的响" << endl;	}
};

//悍马H2模型类
class HummerH2Model
: public HummerModel
{
public:
	void start()
	{	cout << "悍马H2启动..." << endl;	}

	void stop()
	{	cout << "悍马H2停止." << endl;	}

	void alarm()
	{	cout << "悍马H2发出滴滴的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "悍马H2引擎嗡嗡的响" << endl;	}
};

int main(void)
{
	//制造悍马H1模型
	HummerModel *hummerH1 = new HummerH1Model();
	hummerH1->run();

	//制造悍马H2模型
	HummerModel *hummerH2 = new HummerH2Model();
	hummerH2->run();

	return 0;
}
