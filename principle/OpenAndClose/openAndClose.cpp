 //***************************************************************
 // @file:    openAndClose.cpp
 // @author:  dingfang
 // @date    2018-05-23 18:15:22
 //***************************************************************
 
//开闭原则
///一个软件实体 如类. 模块. 和函数应该对外扩展开放, 对修改关闭.
////


# include <iostream>
# include <string>
# include <list>

using namespace std;

//卖书的例子

//书籍类
class IBook
{
public:
	IBook()	{	}
	virtual ~IBook() {	}

	//获取书的名字
	virtual string getName() = 0;

	//获取书的价格
	virtual int getPrice() = 0;

	//获取书的 
	virtual string getAuthor() = 0;
};

//小说类
class NovelBook
: public IBook
{
public:
	NovelBook(string bookName, string author, int price)
	: bookName_(bookName)
	, author_(author)
	, price_(price)
	{	}

	string getName()
	{	return bookName_;	}	

	string getAuthor()
	{	return author_;		}

	int getPrice()
	{	return price_;		}

private:
	string	bookName_;
	string 	author_;
	int 	price_;
};

//书店类
class BookStore
{
public:
	//进书
	void addBook(IBook *book)
	{
		bookList_.push_back(book);
	}

	// 出售书
	void outBook()
	{
		IBook *tmp = bookList_.back();
		cout << "书籍名称: "	<< tmp->getName()		<< "\t\t"
			 << "书籍作者: "	<< tmp->getAuthor()		<< "\t\t"
			 << "书籍价格: $"	<< tmp->getPrice() / 100.0 		 <<  "\t\t"
			 << endl;
		bookList_.pop_back();
		delete tmp;
	}

private:
	list<IBook *> bookList_;
};

///如果需求有改变,  比如要打折
// 40元以上的打九折,  其它打八折 
// //就进行扩展, //而不修改原有的类
class DaZheBook
: public NovelBook 
{//扩展 打折优惠
public:
	DaZheBook(string bookName, string author, int price)
	: NovelBook(bookName, author, price)
	{	}

	int getPrice()
	{//重写获取价格的方法,  给予打折逻辑
		if (NovelBook::getPrice() > 4000)
		{	return NovelBook::getPrice() * 0.9;	}
		else
		{	return NovelBook::getPrice() * 0.8;	}
	}
};

int main(void)
{
	BookStore *bs = new BookStore();
	//基础的进入和出售
	bs->addBook(new NovelBook("天龙八部", "金庸", 3200));
	bs->addBook(new NovelBook("巴黎圣母院", "雨果", 5600));
	bs->addBook(new NovelBook("悲惨世界", "雨果", 3500));
	bs->addBook(new NovelBook("金瓶梅", "兰陵笑笑生", 4300));

	bs->outBook();
	bs->outBook();
	bs->outBook();
	bs->outBook();

	//打折优惠//进行了扩展,  没有修改原来的基类
	bs->addBook(new DaZheBook("天龙八部", "金庸", 3200));
	bs->addBook(new DaZheBook("巴黎圣母院", "雨果", 5600));
	bs->addBook(new DaZheBook("悲惨世界", "雨果", 3500));
	bs->addBook(new DaZheBook("金瓶梅", "兰陵笑笑生", 4300));

	bs->outBook();
	bs->outBook();
	bs->outBook();
	bs->outBook();

	return 0;
}
