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
};

///玩家具体类
class GamePlayer
: public IGamePlayer
{
public:
	GamePlayer(string name)
	: playName_(name)
	{}
	~GamePlayer() {}

	void login()
	{	cout << "login....game" << endl;	}

	void killBoss()
	{	cout << "kill Boss" << endl;	}

	void upgrade()
	{	cout << playName_ << "upgrade" << endl;	}

private:
	//玩家昵称
	string playName_;
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

private:
	IGamePlayer *gamePlayer_;
};

int main(void)
{
	//有一个玩家 
	GamePlayer *gamePlayer = new GamePlayer("dingfang");
	//产生一个代练
	GamePlayerProxy *gamePlayerProxy = new GamePlayerProxy(gamePlayer);

	gamePlayerProxy->login();
	gamePlayerProxy->killBoss();
	gamePlayerProxy->upgrade();

	delete gamePlayer;
	delete gamePlayerProxy;

	return 0;
}
