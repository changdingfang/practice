 //***************************************************************
 // @file:    builder.cpp
 // @author:  dingfang
 // @date    2018-05-26 10:22:37
 //***************************************************************
 
//建造者模式
//将一个复杂的对象的构造与它的表示分离, 
//使得同样的构造过程可以创建不同的表示
/////////
//例子,  生产各种车模型, 
//每种车的需求都不一样 ,就是每一辆车的操作执行顺序不一样
//比如 宝把车要先引擎响 , 再启动, 再停止
//奔驰车要先启动再引擎响, 再停o止
//

# include <iostream>
# include <vector>

using namespace std;

//模型抽象类 
class Model
{
protected:
	Model() {}
	virtual ~Model() {}

	//车子启动
	virtual void start() = 0;
	
	//车子停止 
	virtual void stop() = 0;

	//喇叭
	virtual void alarm() = 0;

	// 需要有引擎声音
	virtual void engineBoom() = 0;

public:
	// 动起来 
	void run()
	{
		for (unsigned long i = 0; i < sequence_.size(); ++i)
		{
			if (sequence_[i] == "start")
			{	this->start();	}
			else if (sequence_[i] == "stop")
			{	this->stop();	}
			else if (sequence_[i] == "alarm")
			{	this->alarm();	}
			else if (sequence_[i] == "engineBoom")
			{	this->engineBoom();	}
			else
			{	cout << "出现问题啦" << endl;	}
		}
	}

	//设置需求
	void setSequence(vector<string> sequence)
	{
		this->sequence_ = sequence;
	}

private:
	//存放车子模型的不同需求顺序
	vector<string> sequence_;
};

//奔驰车模型类
class BenzModel
: public Model
{
public:
	void start()
	{	cout << "奔驰启动..." << endl;	}

	void stop()
	{	cout << "奔驰停止." << endl;	}

	void alarm()
	{	cout << "奔驰发出哔哔的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "奔驰引擎嗡嗡的响" << endl;	}
};

//宝马车模型类
class BMWModel
: public Model
{
public:
	void start()
	{	cout << "宝马启动..." << endl;	}

	void stop()
	{	cout << "宝马停止." << endl;	}

	void alarm()
	{	cout << "宝马发出哔哔的鸣笛声..." << endl;	}

	void engineBoom()
	{	cout << "宝马引擎嗡嗡的响" << endl;	}
};

//建造者抽象类
class Builder
{
public:
	Builder() {}
	virtual ~Builder() {}

	//获取车模型
	virtual Model * getModel() = 0;

	//设置车的操作顺序
	virtual void setSequence(vector<string>) = 0;
};

//奔驰建造者lei
class BenzBuilder
: public Builder
{
public:
	BenzBuilder()
	: benzModel_(new BenzModel())
	{}
	~BenzBuilder()
	{	delete benzModel_;	}

	Model *getModel()
	{	return benzModel_;	}

	void setSequence(vector<string> sequence)
	{	benzModel_->setSequence(sequence);	}

private:
	BenzModel *benzModel_;
};

//宝马车建造者类 
class BMWBuilder
: public Builder
{
public:
	BMWBuilder() 
	: bmwModel_(new BMWModel())
	{}
	~BMWBuilder() 
	{	delete bmwModel_;	}

	Model *getModel()
	{	return bmwModel_;	}

	void setSequence(vector<string> sequence)
	{	bmwModel_->setSequence(sequence);	}
	
private:
	BMWModel *bmwModel_;
};

int main(void)
{
	//创建奔驰建造者
	Builder *benz = new BenzBuilder();
	//创建宝马建造者
	Builder *bmw = new BMWBuilder();

	//设置操作顺序 
	vector<string> sequence;
	sequence.push_back("start");
	sequence.push_back("alarm");
	sequence.push_back("stop");

	benz->setSequence(sequence);
	Model *benzModel = benz->getModel();
	benzModel->run();

	cout << endl;

	//把宝马车和奔驰车用同一套操作步骤
	bmw->setSequence(sequence);
	Model *bmwModel = bmw->getModel();
	bmwModel->run();

	return 0;
}
