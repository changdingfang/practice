 //***************************************************************
 // @file:    command.cpp
 // @author:  dingfang
 // @date    2018-06-01 11:15:30
 //***************************************************************
 
//命令模式
//将一个请求封装成一个对象, 从而让你使用不同的请求把客户端参数化,
//对请求排队或者记录请求日志, 可以提供命令的撤销和恢复功能
////////
//例子   一个开发团队
//开发团队里面有 产品需求组, 美工组, 和代码实现组

# include <iostream>

using namespace std;


//组的抽象类
class Group
{
public:
	Group() {}
	virtual ~Group() {}

	//找到某个组
	virtual void find() = 0;

	//添加需求
	virtual void add() = 0;

	//删除某个需求
	virtual void del() = 0;
	
	//修改需求 
	virtual void change() = 0;

	//整个项目的计划 
	virtual void plan() = 0;
};

//产品需求组
class RequirementGroup
: public Group
{
public:
	void find()
	{	cout << "找到产品需求组" << endl;	}

	void add()
	{	cout << "客户要求产品添加需求" << endl;	}

	void del()
	{	cout << "客户要求去掉某个需求" << endl;	}

	void change()
	{	cout << "客户要求修改某个需求" << endl;	}

	void plan()
	{	cout << "客户要知道需求变更计划" << endl;	}
};

//美工组
class PageGroup
: public Group
{
public:
	void find()
	{	cout << "找到美工组" << endl;	}

	void add()
	{	cout << "客户要美工求添加页面" << endl;	}

	void del()
	{	cout << "客户要求去掉某个页面" << endl;	}

	void change()
	{	cout << "客户要求修改某个页面" << endl;	}

	void plan()
	{	cout << "客户要知道美工变更计划" << endl;	}
};

//代码实现组
class CodeGroup
: public Group
{
public:
	void find()
	{	cout << "找到代码实现组" << endl;	}

	void add()
	{	cout << "客户要代码求添加功能" << endl;	}

	void del()
	{	cout << "客户要求去掉某个功能" << endl;	}

	void change()
	{	cout << "客户要求修改某个功能" << endl;	}

	void plan()
	{	cout << "客户要知道代码实现组变更计划" << endl;	}
};

//命令封装对象抽象类
class Command
{
public:
	Command()
	: rg_(new RequirementGroup())
	, pg_(new PageGroup())
	, cg_(new CodeGroup())
	{}

	virtual ~Command()
	{
		delete rg_;
		rg_ = NULL;
		delete pg_;
		pg_ = NULL;
		delete cg_;
		cg_ = NULL;
	}
	virtual void execute() = 0;

protected:
	RequirementGroup	*rg_;
	PageGroup			*pg_;
	CodeGroup			*cg_;
};

//删除网页命令类
class DelPageCommand
: public Command
{
public:
	void execute()
	{ //删除一个页面的命令
		pg_->find();
		pg_->del();
		pg_->plan();
	}
};

//增加需求命令
class AddRequirementCommand
: public Command
{
public:
	void execute()
	{//增加一个需求的命令
		rg_->find();
		rg_->add();
		rg_->plan();
	}
};

// 每一个命令都封装成一个对象,
//////
//增加一个网页的命令
//删除一个需求的命令
//增加一个功能的命令
//删除一个功能的命令
//////
//

//负责人 负责和客户交流接头
class Invoker
{
public:
	//接收到客户发达的命令, 
	void setCommand(Command *command)
	{
		this->command_ = command;
	}

	//执行命令
	void Action()
	{
		this->command_->execute();
	}

private:
	Command *command_;//命令的基类指针, 可以接收子类的任何一个命令
};

int main(void)
{
	//定义一个负责人
	Invoker *dingfang = new Invoker();

	//增加需求的命令
	Command *command = new AddRequirementCommand();

	//负责人接到命令
	dingfang->setCommand(command);

	//负责人执行命令
	dingfang->Action();

	delete command;
	command = NULL;

	cout << "############################################################" << endl;

	//删除一个网页命令
	command = new DelPageCommand();
	dingfang->setCommand(command);
	dingfang->Action();

	delete command;
	command = NULL;
	delete dingfang;
	dingfang = NULL;

	return 0;
}
