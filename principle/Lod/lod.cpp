 //***************************************************************
 // @file:    lod.cpp
 // @author:  dingfang
 // @date    2018-05-23 16:27:24
 //***************************************************************
 
/////一个对象应该对其它对象有最少的了解
//
//只和朋友交流 朋友之间也有juli

# include <iostream>
# include <list>

using namespace std;

//老师要去体育委员点名

class Student;
class GroupLeader;
class Teacher
{
public:
	//体育委员类似老师类的朋友, 可进行交流
	//下达命令给体育委员
	////不学生类不是老师类的朋友, 不要进行交流,, 所以不要让学生类在老师类里面出现
	void command(GroupLeader *);
};

class GroupLeader
{
public:
	GroupLeader(list<Student *> stList)
	: stList_(stList)
	{	}

	//清学生的数量
	void count();

private:
	list<Student *> stList_;
};

class Student
{
	//学生什么doubuzuo
};

void Teacher::command(GroupLeader *leader)
{
	leader->count();
}
void GroupLeader::count()
{
	cout << "学生的数量:" << stList_.size() << endl;
}

int main(void)
{
	list<Student *> st;
	for (int i = 0; i < 8; ++i)
	{
		st.push_back(new Student());
	}
	Teacher *teacher = new Teacher();
	//老师下达明林 , 让体育委员清点人数
	teacher->command(new GroupLeader(st));

	int len = st.size();
	for (int i = 0; i < len; ++i)
	{
		delete st.back();
		st.pop_back();
	}

	return 0;
}
