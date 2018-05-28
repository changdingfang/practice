 //***************************************************************
 // @file:    mediator.cpp
 // @author:  dingfang
 // @date    2018-05-28 11:30:47
 //***************************************************************
 //
 //中介者模式
 //用一个中介对象封装一系列的对象交互, 中介者使各对象不需
 //要显示的相互作用, 从而使其耦合松散, 而且可以独立的改变它们之间的交互
 //////
 //例子 :  进货, 销售 ,  和仓库 的一些列流程
 //销售类需要知道仓库剩余多艘啊, 进货也需要知道仓库还有多少存货, 
 //
 //如果没有中介者, 这样交纵复杂的关系增大了耦合, 修改起来非常的危险
 
# include <stdlib.h>
# include <time.h>

# include <iostream>

using namespace std;

class AbstractMediator;

//抽象同事类
class AbstractColleague
{
};

//采购
class Purchase
{
public:
	Purchase(AbstractMediator *mediator)
	: mediator_(mediator)
	{}

	//采购惠普电脑
	int buyHP(int number);

	//停止采购惠普电脑 
	void refuseBuyHP();

private:
	AbstractMediator *mediator_;
};

//销售
class Sale
{
public:
	Sale(AbstractMediator *mediator)
	: mediator_(mediator)
	{}

	//销售电脑
	void saleHP(int number);

	//获取销售的状态, 100表示销售很好, 0 表示滞销
	int getSaleStatus();

	//打折出售
	void offSale();

private:
	AbstractMediator *mediator_;
};

//仓库
class Stock
{
public:
	Stock(AbstractMediator * mediator)
	: mediator_(mediator)
	{}

	//获取库存数量
	int getStockNumber() const
	{	return COMPUTER_NUMBER;	}

	//增加库存
	void increase(int number)
	{
		COMPUTER_NUMBER += number;
		cout << "库存数量: " << COMPUTER_NUMBER << endl;
	}

	//降低库存
	void decrease(int number)
	{
		COMPUTER_NUMBER -= number;
		cout << "库存数量: " << COMPUTER_NUMBER << endl;
	}

	int isStock();

private:
	//电脑的库存
	static int COMPUTER_NUMBER;
	AbstractMediator *mediator_;
};

//int Stock::COMPUTER_NUMBER = 100;
int Stock::COMPUTER_NUMBER = 160;

//抽象中介者
class AbstractMediator
{
public:
# if 1
	AbstractMediator()
	: sale_(new Sale(this))
	, stock_(new Stock(this))
	, purchase_(new Purchase(this))
	{} 
# endif

	virtual void execute(string str, int number = 0) = 0;

protected:
# if 1
	Sale		*sale_;
	Stock		*stock_;
	Purchase	*purchase_;
# endif
};

class Mediator
: public AbstractMediator
{
public:
	void execute(string str, int number = 0)
	{
		if (str == "buyHP" && number > 0)
		{
			int ret;
			if (sale_->getSaleStatus() > 60)
			{//检查销售状态, 看销售是否良好
				ret = purchase_->buyHP(number);//采购进行采购
				stock_->increase(ret);//采购后增加到库存
			}
			else
			{//销售情况不良好, 则只进一半货
				ret = purchase_->buyHP(number / 2);//采购进行采购
				stock_->increase(ret);//采购后增加内存
			}
		}
		else if (str == "offSale")
		{
			sale_->offSale();
			srand(time(NULL));
			//打折后卖出的电脑数量
			int num = rand() % stock_->getStockNumber();
			cout << "打折出售电脑的数量: " << num << endl;
			stock_->decrease(num);
		}
		else if (str == "saleHP" && number > 0)
		{
			cout << "出售: " << number << "台" << endl;
			stock_->decrease(number);
		}
	}
};

int Purchase::buyHP(int number)
{
	cout << "成功采购: " << number << "台"  << endl;
	return number;	
}

void Purchase::refuseBuyHP()
{	cout << "停止购买惠普电脑" << endl;	}

void Sale::saleHP(int number)
{	mediator_->execute("saleHP", number);	}

int Sale::getSaleStatus()
{
	srand(time(NULL));
	int status = rand() % 101;
	cout << "销售状态: " << status << endl;
	return status;
}

void Sale::offSale()
{	cout << "打折出售惠普电脑" << endl;	}

int Stock::isStock()
{
	if (COMPUTER_NUMBER > 150)
	{//库存上限就不能再购买了
		cout << "存货太多, 不要再采购啦" << endl;	
		//打折出售
		cout << "存货: " << COMPUTER_NUMBER << endl;
		mediator_->execute("offSale");
		return 1;
	}
	else if (COMPUTER_NUMBER < 50)
	{//库存太少就要进货
		mediator_->execute("buyHP", 80);
		return 2;
	}
	return 0;
}

int main(void)
{
	//中介者 
	Mediator *mediator = new Mediator();

	//仓库
	Stock *stock = new Stock(mediator);
	int ret = stock->isStock();
	if (0 == ret)
	{
		Sale *sale = new Sale(mediator);
		sale->saleHP(30);
	}


	return 0;
}
