 //***************************************************************
 // @file:    analysis.h
 // @author:  dingfang
 // @date    2018-09-07 17:06:34
 //***************************************************************
 
#pragma once

# include <iostream>
# include <string>
# include <fstream>
# include <vector>

using namespace std;

struct VectorQuestion
{
public:
	VectorQuestion()    {   }
	~VectorQuestion()   {   }
	string              stem_;      //题干
	string              answer_;    //答案
	vector<string>      option_;    //选项

	int		stem_id_;		//题干的id
};


class ChoiceQuestion
{
public:
	ChoiceQuestion()	
	: vecQ_()
	{	};
	~ChoiceQuestion()	{	};

	//初始化
	void init(const string , const string);

	//执行解析
	int execute();

	//写入文件
	int writeFile();

	//打印题
	void printQuestion();

	//获取vecQ
	inline vector<VectorQuestion> getVecQ()	{	return vecQ_;	}

private:
	//获取答案的选项 
	int get_answer_option(string );

	//去除前面的 "." 或者 "、"
	int find_dian(string& param);

	//去除末尾的空格
	int remove_space(string&);

	//获取选项
	int get_option(string, VectorQuestion &);

	//获取选项 
	int get_option_content(char option, string param, string &optionStr);
	//是否是选项 
	bool is_option(string option, size_t pos);

private:
	vector<VectorQuestion>	vecQ_;

	string		answerOption_;//答案的选项

	string		writeFileName_;
	string		readFileName_;
};

