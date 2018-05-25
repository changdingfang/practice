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
//
//修改需求: 
//现在是H1的喇叭响不响由客户决定, 
//H2的喇叭是不会响的

# include <iostream>

using namespace std;

//悍马模型抽象类
class HummerModel
{
protected:
	//需要启动
	virtual void start() = 0;

	// 需要停止
	virtual void stop() = 0;

	// 需要有喇叭
	virtual void alarm() = 0;

	// 需要有引擎声音
	virtual void engineBoom() = 0;

	//喇叭是否要响, 默认为响
	virtual bool isAlarm()
	{	return true;	}

public:
	// 需要会跑
	void run()
	{//悍马H1模型和悍马H2模型都有相同的这样run操作
	//所以就不定义为纯虚函数了, 在这里当模板使用
	//这...模板方法模式
		//先启动
		this->start();

		//会听到引擎作响
		this->engineBoom();

		//判断是否要响
		if (isAlarm())
		{
			//遇到人需要鸣笛提示
			this->alarm();
		}

		//到达目的地要停止
		this->stop();
	}
};

//悍马H1模型类
class HummerH1Model
: public HummerModel
{
public:
	HummerH1Model()
	: isAlarmFlag_(true)
	{}

	//让客户设置是否要响喇叭
	void setAlarm(bool isAlarmFlag)
	{	this->isAlarmFlag_ = isAlarmFlag;	}

protected:
	void start()
	{	cout << "悍马H1启动..." << endl;	}

	void stop()
	{	cout << "悍马H1停止." << endl;	}

	void alarm()
	{	cout << "悍马H1发出哔哔的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "悍马H1引擎嗡嗡的响" << endl;	}

	bool isAlarm()
	{	return isAlarmFlag_;	}

private:
	bool isAlarmFlag_;
};

//悍马H2模型类
class HummerH2Model
: public HummerModel
{
protected:
	void start()
	{	cout << "悍马H2启动..." << endl;	}

	void stop()
	{	cout << "悍马H2停止." << endl;	}

	void alarm()
	{	cout << "悍马H2发出滴滴的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "悍马H2引擎嗡嗡的响" << endl;	}

	//H2喇叭不会响
	bool isAlarm()
	{	return false;	}
};

int main(void)
{
	//制造悍马H1模型
	HummerH1Model *hummerH1 = new HummerH1Model();
# if 1
	//设置成0
	hummerH1->setAlarm(0);
	hummerH1->run();
#endif

	cout << endl;
# if 1
	//设置成1
	hummerH1->setAlarm(1);
	hummerH1->run();
# endif

	cout << endl;
	//制造悍马H2模型
	HummerModel *hummerH2 = new HummerH2Model();
	hummerH2->run();

	return 0;
}
