 //***************************************************************
 // @file:    analysis.cpp
 // @author:  dingfang
 // @date    2018-09-07 17:06:34
 //***************************************************************
 
#pragma once
# include "analysis.h"

void ChoiceQuestion::init(const string readFileName, const string writeFileName)
{
	readFileName_	= readFileName;
	writeFileName_	= writeFileName;
}

void ChoiceQuestion::printQuestion()
{
	cout << "vecQ.size() : " << vecQ_.size() << endl;
	for (int i = 0; i < vecQ_.size(); ++i)
	{
		cout << "stem: ===" << vecQ_[i].stem_ << "===" << endl;
		cout << "answer: ===" << vecQ_[i].answer_ << "===" << endl;
		for (int j = 0; j < vecQ_[i].option_.size(); ++j)
		{
			cout << "option: ===" << vecQ_[i].option_[j] << "===" << endl;
		}
	}
}

int ChoiceQuestion::writeFile()
{
	//ofstream ofs("./damo2.data", ios::app);
	ofstream ofs(writeFileName_.c_str());
	if (!ofs.good())
	{
		cout << "error ofs.file()" << endl;
		return -1;
	}
# if 1
	for (int i = 1; i < vecQ_.size(); ++i)
	{
		ofs << vecQ_[i].stem_	<< '\n'
			<< vecQ_[i].answer_	<< '\n';
		for (int j = 0; j < vecQ_[i].option_.size(); ++j)
		{
			ofs << vecQ_[i].option_[j] << '\n';
		}
	}
# endif
	ofs.close();
}

int ChoiceQuestion::execute()
{
	ifstream ifs(readFileName_.c_str());
	if (!ifs.good())
	{
		cout << "error ofs.file()" << endl;
		return -1;
	}

	string			line;

	//string			stem;	//题干
	string			answer;	//答案
	vector<string>	option;	//选项

	int 	count		= 0;
	bool	isNewStem	= true;	//用于鉴别是否是新题

	VectorQuestion		tmpVecQue;
	while (getline(ifs, line))
	{
		isNewStem = false;
		if (line[0] <= '9' && line[0] >= '0')
		{
			//cout << "================================================" << endl;
			isNewStem = true;
			vecQ_.push_back(tmpVecQue);
			tmpVecQue.stem_ = "";
			tmpVecQue.option_.clear();
			tmpVecQue.answer_ = "";
			answerOption_ = "";
		}
		else
		{//如果开始不是0~9, 则认为是选项
			get_option(line, tmpVecQue);
		}
# if 0
		if (count++ == 18)
		{
			break;
		}
# endif
		//---cout << line << endl;
		if (!isNewStem)
		{
			size_t answerPos = line.find(this->answerOption_);
			if (get_option_content(this->answerOption_[0], line, answer))
			{//如果没找到答案, 则应该在下一行的内容中
				if (get_option_content(this->answerOption_[0] + 32, line, answer))
				{
					continue;
				}
			}
			//=========================
			//得到答案
			tmpVecQue.answer_ = answer;
			//=========================
			//---cout << "ansert: ===" << answer << "====" << endl;
			continue;
		}

		int ret = 0;
		if (!(ret = find_dian(line)))
		{
			remove_space(line);
		}
	
		if (0 == ret)
		{
			//cout << "pos : " << pos << endl;
			//line = line.substr(pos + 1,  line.size());

			size_t stemPos = 0;
			stemPos = line.rfind("（");
			if (line.size() - stemPos > 9)
			{
				stemPos = line.rfind("(");
			}
			//cout << "stemPos : " << stemPos << endl;
			if (string::npos != stemPos)
			{
				//=========================
				//获取题干
				tmpVecQue.stem_ = line.substr(0, stemPos);
				remove_space(tmpVecQue.stem_);
				//=========================
				//---cout << "stem:" << tmpVecQue.stem_ << endl;
				//////

				//获取答案选项
				string tmp;
				tmp = line.substr(stemPos, line.size());
				//cout << "tmp : " << tmp << endl;
				get_answer_option(tmp);
				//---cout << answerOption_ << endl;
			}
		}
	}
	ifs.close();
}

int ChoiceQuestion::get_answer_option(string tmp)
{
	size_t answerPos = -1;
	if (tmp.find("A") != string::npos)
	{
		answerPos = tmp.find("A");
	}
	else if (tmp.find("B") != string::npos)
	{
		answerPos = tmp.find("B");
	}
	else if (tmp.find("C") != string::npos)
	{
		answerPos = tmp.find("C");
	}
	else if (tmp.find("D") != string::npos)
	{
		answerPos = tmp.find("D");
	}
# if 0
	else if (tmp.find("a") != string::npos)
	{
		answerPos = tmp.find("a");
	}
# endif
	else
	{
		cout << "error find a b c d" << endl;
	}
	if (answerPos != -1)
	{
		this->answerOption_ = tmp.substr(answerPos, 1);
	}
}

int ChoiceQuestion::find_dian(string& param)
{
	size_t pos = param.find('.');
	if (pos != string::npos && pos < 4)
	{
		param = param.substr(pos + 1, param.size());
		return 0;
	}
	pos = param.find("、");
	if (pos != string::npos && pos < 4)
	{
		param = param.substr(pos + 3, param.size());
		return 0;
	}
	pos = param.find("．");
	if (pos != string::npos && pos < 4)
	{
		param = param.substr(pos + 3, param.size());
		return 0;
	}

	return -1;
}

int ChoiceQuestion::remove_space(string& param)
{
	//去除前端的空格
	size_t spacePos = param.find_first_not_of(' ');
	if (spacePos != string::npos)
	{
		param = param.substr(spacePos, param.size());
	}
	//去除末尾的空格
	spacePos = param.find_last_not_of(' ');
	if (spacePos != string::npos)
	{
		param = param.substr(0, spacePos + 1);
		return 0;
	}

	return -1;
}

int ChoiceQuestion::get_option(string param, VectorQuestion &vecQue)
{
	char option = 'A';
	string optionStr = "";
	//if (optionPos != string::npos && ( 0 == optionPos || param[optionPos - 1] == ' '))
	for (int i = 0; i < 4; ++i)
	{
		if (0 == get_option_content(option, param, optionStr))
		{
			vecQue.option_.push_back(optionStr);
			//---cout << "optionStr : " << optionStr << endl;
		}
		optionStr = "";
		++option;
	}
# if 1
	option = 'a';
	for (int i = 0; i < 4; ++i)
	{
		if (0 == get_option_content(option, param, optionStr))
		{
			vecQue.option_.push_back(optionStr);
			//---cout << "optionStr : " << optionStr << endl;
		}
		optionStr = "";
		++option;
	}
# endif

	return 0;
}

int ChoiceQuestion::get_option_content(char option, string param, string &optionStr)
{
	size_t optionPos = 0;

	do
	{
		size_t optionPosConst = param.find(option, optionPos);
		optionPos = optionPosConst;
		if (is_option(param, optionPosConst))
		{
			do
			{
				size_t nextPos = param.find(option + 1, optionPos);
				if (nextPos == string::npos)
				{
					optionStr = param.substr(optionPosConst, param.size());
					find_dian(optionStr);
					remove_space(optionStr);
					break;
				}
				else
				{
					if (param[nextPos - 1] == ' ')
					{// 找到了下一个选项的位置
						optionStr = param.substr(optionPosConst, nextPos - optionPosConst - 1);
						find_dian(optionStr);
						remove_space(optionStr);
						break;
					}
					else
					{//找到了, 但不是选项,而是选项内容的字母//此时需要重新寻找
						optionPos = nextPos + 1;
					}
				}
			} while (true);
			return 0;
		}
		else
		{//没找到选项, 而是找到选项的内容了, 需要充下一个位置再开始找, 直到找到末尾
			++optionPos;
			if (optionPosConst == string::npos)
			{
				break;
			}
		}
	} while (true);
	return -1;
}

bool ChoiceQuestion::is_option(string option, size_t pos)
{
	if (pos != string::npos)
	{
		if (0 == pos || option[pos - 1] == ' ')
		{
			return true;
		}
	}

	return false;
}
