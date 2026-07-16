#include "filestream.h"
#include <fstream>
#include <iostream>
//#include <sstream>
std::string filestream::date;
std::string filestream::usedtimestr;
long long filestream::usedtime;
void filestream::readfile()
{
	std::ifstream in;
	in.open("time.txt");
	if (!in.is_open())
	{
		std::cout << "can't find file" << std::endl;
		date = "emty";
		usedtime = 0;
		return;
	}
	//std::stringstream ss;
	in >> date >> usedtimestr;//读取文件中的日期和使用时间
	usedtime = stoll(usedtimestr);
}

void filestream::outToFile()
{
	std::ofstream out;
	out.open("time.txt");
	if (!out.is_open())
	{
		std::cout << "can't find file" << std::endl;
		return;
	}
	out << date << std::endl << usedtime;
}