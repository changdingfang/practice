 //***************************************************************
 // @file:    2builder.cpp
 // @author:  dingfang
 // @date    2018-05-26 11:10:24
 //***************************************************************
 
//建造者模式
/////
//例子,  生产各种车模型, 
//每种车的需求都不一样 ,就是每一辆车的操作执行顺序不一样
//比如 宝把车要先引擎响 , 再启动, 再停止
//奔驰车要先启动再引擎响, 再停o止
//
//
////
//添加一个导演类 
//导演类可以生产指定的车模型(操作顺序也有)

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

//增加一个导演类
class Director
{//每一种类型都有不同的操作方式
//通过排列组合可知道有非常多的操作方式, 根据需求, 定义需要的方式
//在需要的时候, 可以很简单的创建出来
public:
	Director()
	: benzBuilder_(new BenzBuilder())
	, bmwBuilder_(new BMWBuilder())
	{}
	~Director()
	{
		delete benzBuilder_;
		delete bmwBuilder_;
	}
	//获取奔驰A模型
	Model *getBenzAModel()
	{
		sequence_.clear();
		sequence_.push_back("start");
		sequence_.push_back("engineBoom");
		sequence_.push_back("stop");

		benzBuilder_->setSequence(sequence_);

		return benzBuilder_->getModel();
	}
	//获取奔驰B模型
	Model *getBenzBModel()
	{
		sequence_.clear();
		sequence_.push_back("start");
		sequence_.push_back("alarm");
		sequence_.push_back("stop");

		benzBuilder_->setSequence(sequence_);

		return benzBuilder_->getModel();
	} 
	//获取宝马C模型
	Model *getBMWCMOdel()
	{
		sequence_.clear();
		sequence_.push_back("start");
		sequence_.push_back("stop");

		bmwBuilder_->setSequence(sequence_);
		
		return bmwBuilder_->getModel();
	}
	//获取宝马D模型
	Model *getBMWDMOdel()
	{
		sequence_.clear();
		sequence_.push_back("start");

		bmwBuilder_->setSequence(sequence_);
		
		return bmwBuilder_->getModel();
	}

private:
	vector<string> sequence_;
	BenzBuilder *benzBuilder_;
	BMWBuilder *bmwBuilder_;
};

int main(void)
{
	Director *director = new Director();

	int i;
	for (i = 0; i < 100; ++i)
	{//造100台宝马C类车
		//director->getBMWCMOdel()->run();
		director->getBMWCMOdel();
	}

	for (i = 0; i < 20; ++i)
	{//造20台奔驰A类的车
		//director->getBenzAModel()->run();
		director->getBenzAModel();
	}

	//##########################
	///同样的方法可以扩展多重操作类型的车, ...
	//......................
	//##########################

	return 0;
}
