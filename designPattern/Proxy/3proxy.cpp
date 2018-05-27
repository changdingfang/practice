 //***************************************************************
 // @file:    proxy.cpp
 // @author:  dingfang
 // @date    2018-05-26 12:48:24
 //***************************************************************
 
//代理模式
//为其它对象提供一种代理以控制对这个对象的访问
/////////
//例子 :  打游戏 
//打游戏需要打的时间太长会很累, 但是又想升级, 所以, 就找个代练来帮打
//
//
/////强制代理  
//// 强制代理要求只能从真实用户获取到代理, 不能访问真实用户

# include <iostream>

using namespace std;

//玩家抽象类
class IGamePlayer
{
public:
	IGamePlayer() {}
	virtual ~IGamePlayer() {}

	///游戏需要登录的
	virtual void login() = 0;

	///游戏需要打怪
	virtual void killBoss() = 0;

	//游戏可以升级
	virtual void upgrade() = 0;

	virtual IGamePlayer *getProxy() = 0;
};

//游戏代练类//游戏代练也是要通过正常的步骤来打游戏, 所以继承IGamePlayer
class GamePlayerProxy
: public IGamePlayer
{//操控一个了类,,
public:
	GamePlayerProxy(IGamePlayer *gamePlayer)
	: gamePlayer_(gamePlayer)
	{}

	void login()
	{	gamePlayer_->login();		}

	void killBoss()
	{	gamePlayer_->killBoss();	}

	void upgrade()
	{	gamePlayer_->upgrade();		}

	IGamePlayer *getProxy()
	{	return this;	}

private:
	IGamePlayer *gamePlayer_;
};

///玩家具体类
class GamePlayer
: public IGamePlayer
{
public:
	GamePlayer(string name)
	: gameProxy_(NULL)
	, playName_(name)
	{}
	~GamePlayer() 
	{
		delete gameProxy_;
	}

	void login()
	{
		if (this->isProxy())
		{	cout << "login....game" << endl;	}
		else
		{	cout << "请使用代理访问" << endl;	}
	}

	void killBoss()
	{
		if (this->isProxy())
		{	cout << "kill Boss" << endl;	}
		else
		{	cout << "请使用代理访问" << endl;	}
	}

	void upgrade()
	{
		if (this->isProxy())
		{	cout << playName_ << "upgrade" << endl;	}
		else
		{	cout << "请使用代理访问" << endl;	}
	}

	IGamePlayer *getProxy()
	{
		if (this->gameProxy_ == NULL)
		{
			this->gameProxy_ = new GamePlayerProxy(this);
		}
		return gameProxy_;	
	}

private:
	bool isProxy()
	{	return (gameProxy_ == NULL) ? false : true;	}

private:
	//玩家昵称
	IGamePlayer *gameProxy_;
	string playName_;
};

int main(void)
{
	//有一个玩家 
	IGamePlayer *gamePlayer = new GamePlayer("dingfang");

	//直接访问gamePlayer的方法
	////会提示让使用代理访问, 所以不能直接访问
	gamePlayer->login();
	gamePlayer->killBoss();
	gamePlayer->upgrade();

	cout << endl;

	//创建一个代理, 通过代理访问试试
	////发现也是不能访问的, ,因为这个代理不是玩家指定的, 
	IGamePlayer *proxy = new GamePlayerProxy(gamePlayer);
	proxy->login();
	proxy->killBoss();
	proxy->upgrade();

	cout << endl;

	//获取该玩家的代练 
	////这个玩家指定的代理就可以正常访问玩家的方法//可以进行正常的游戏
	IGamePlayer *gamePlayerProxy = gamePlayer->getProxy();

	gamePlayerProxy->login();
	gamePlayerProxy->killBoss();
	gamePlayerProxy->upgrade();

	delete gamePlayer;
	delete proxy;

	return 0;
}
