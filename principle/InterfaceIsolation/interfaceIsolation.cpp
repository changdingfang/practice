 //***************************************************************
 // @file:    interfaceIsolation.cpp
 // @author:  dingfang
 // @date    2018-05-23 11:07:40
 //***************************************************************
 
//客户端不应该依赖它不需要的接口
//类间的依赖关系应该建立在最小的接口上 //接口细化, 接口纯洁

# include <iostream>
# include <string>

using namespace std;

//一个美女的定义
//把外貌和气质分开来写,  因为每个人注重的东西不一样
//(认为外貌好的为美女, 则只继承IPettyGirl,  只认为有气质就是美女的就只
//继承IGreatTemperamentGirl,  如果认为两者兼备的才算美女, 这两个都继承)
//这样可以达到接口分离的效果,  

class IPettyGirl
{
public:
	//有漂亮的脸蛋
	virtual void goodLooking() = 0;

	//身材很棒
	virtual void niceFigure() = 0;
};

class IGreatTemperamentGirl
{
public:
	//很有气质
	virtual void greatTemperament() = 0;
};

class PettyGirl 
: public IPettyGirl, public IGreatTemperamentGirl
{//长得好看又有气质的美女 
public:
	PettyGirl(string name)
	: name_(name)
	{ }

	void goodLooking()
	{	cout << "脸蛋好看" << endl;	}

	void niceFigure()
	{	cout << "身材很棒" << endl;	}

	void greatTemperament()
	{	cout << "有气质" << endl;	}

	//搜索美女的条件
	void print()
	{
		cout << "名字:" << name_ << endl;
		cout << "属性: " << endl;
		cout << "	";
		goodLooking();
		cout << "	";
		niceFigure();
		cout << "	";
		greatTemperament();
	}

private:
	//名字
	string name_;
};

int main(void)
{
	PettyGirl girl("如花");
	girl.print();

	return 0;
}
