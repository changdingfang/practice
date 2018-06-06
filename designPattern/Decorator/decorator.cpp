 //***************************************************************
 // @file:    decorator.cpp
 // @author:  dingfang
 // @date    2018-06-06 16:30:05
 //***************************************************************
 
//装饰模式
//动态的给对象添加一些额外的职责, 就增加功能来说, 装饰模式相比生成子类更加的灵活
///////
//例子,  成绩单
////考试分数很低, 需要装饰一下, 避免回家挨揍

# include <iostream>

using namespace std;

//成绩单抽象类
class SchoolReport
{
public:
	SchoolReport() {}
	virtual ~SchoolReport() {}

	//成绩单
	virtual void report() = 0;

	//成绩单需要家长签字
	virtual void sign(string name) = 0;
};

//小学成绩单
class XiaoSchoolReport
: public SchoolReport
{
public:
	XiaoSchoolReport() {}
	~XiaoSchoolReport() {}

	void report()
	{
		cout << ">>>>石炉小学<<<<" << endl
			 << "尊敬炉民家长:" << endl
			 << "成绩单详情:" << endl
			 << "语文: 60, 数学: 66, 体育: 80" << endl
			 << "家长签字: " << endl;
	}

	void sign(string name)
	{	cout << "家长签名: " << name << endl;	}
};

//装饰基类
class Decorator
: public SchoolReport
{
public:
	Decorator(SchoolReport *sr)
	: sr_(sr)
	{}
	~Decorator() 
	{
		delete sr_;
		sr_ = NULL;
	}

	void report()
	{	this->sr_->report();	}

	void sign(string name)
	{	this->sr_->sign(name);	}
	
private:
	SchoolReport *sr_;
};

//最高成绩装饰lei
class HighScoreDecorator
: public Decorator
{
public:
	HighScoreDecorator(SchoolReport *sr) 
	: Decorator(sr)
	{}
	~HighScoreDecorator() {}

	void reportHighScore()
	{
		cout << "最高成绩:" << endl
			 << "语文: 68, 数学: 70, 体育: 80" << endl;
	}

	void report()
	{
		//先展示最高成绩, 发现最高成绩也很低, 就不会怪我的成绩低了
		this->reportHighScore();
		//展示我的成绩, 已经有最高成绩座位铺垫
		this->Decorator::report();
	}
};

//我的成绩排名装饰
class SortDecorator
: public Decorator
{
public:
	SortDecorator(SchoolReport *sr) 
	: Decorator(sr)
	{}
	~SortDecorator() {}

	void reportSort()
	{
		cout << "我排名第12名" << endl;
	}

	void report()
	{
		//先给看我的成绩 
		this->Decorator::report();
		//再给看我的排名, 一看, 还不错,
		this->reportSort();
	}
};

int main(void)
{
	//小学成绩单
	SchoolReport *sr = new XiaoSchoolReport();
	//sr->report();	
	sr = new HighScoreDecorator(sr);
	sr = new SortDecorator(sr);
	sr->report();
	sr->sign("炉神");

	return 0;
}
