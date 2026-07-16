#include <iostream>
#include <cstdio>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include "filestream.h"
using namespace std;
const string TVip = "192.168.2.212";
string connectstr = "adb connect " + TVip, uptimestr = "adb -s " + TVip + " shell uptime", closestr = "adb -s " + TVip + " shell input keyevent 26", getdatestr = "echo %DATE%";


string execCmd(const char* cmd) {
	FILE* pipe = _popen(cmd, "r");
	if (!pipe)
	{
		return "";
	}
	char buffer[1024];
	string result;
	fgets(buffer, 1024, pipe);
	result += buffer;
	if (!result.empty() && result.back() == '\n') result.pop_back();
	_pclose(pipe);
	return result;
}

long long getruntime(string timestr) {
	//获取时间
	stringstream ss(timestr);
	string   temp,token;
	int minute = 0,hour = 0,day = 0;
	while (ss >> token)
	{
		if (token == "up") break;
	}
	ss >> temp;
	getline(ss, token, ',');
	if (token == "days" || token == "day")//&& 
	{
		day = stoi(temp);
		getline(ss, token, ':');
		hour = stoi(token);
		getline(ss, token, ',');
		minute = stoi(token);
	}
	else if(token == "min")
	{
		minute = stoi(temp);
	}
	else
	{
		string ttemp;
		stringstream ss2(temp);
		getline(ss2, ttemp, ':');
		hour = stoi(ttemp);
		getline(ss2, ttemp, ',');
		minute = stoi(ttemp);
	}
	//计算时间
	long long result;
	result = day * 86400 + hour * 3600 + minute * 60;
	return result;
}

void test() {
	string connectstr = "adb connect " + TVip, uptimestr = "adb -s " + TVip + " shell uptime", closestr = "adb -s " + TVip + " shell input keyevent 26";
	cout << execCmd(connectstr.c_str()) << endl;
	long long begin_uptime;
	string getstr = execCmd(uptimestr.c_str());
	begin_uptime = getruntime(getstr);
	cout << getstr << begin_uptime << endl;
}

void limit_funtion(long long daily_limit_time,long long &usedtime){
	cout << "the limit time today remain " << (daily_limit_time - usedtime) / 60 << " minute" << endl
		<< "please input this times limit:(hours minutes)" << endl;
	int h, m,limitTime;
	long sleeptime;
	bool input_circulation = true;
	while (input_circulation)
	{
		cin >> h >> m;
		limitTime = h * 3600 + m * 60;
		if (limitTime+usedtime >=daily_limit_time)
		{
			cout << "more than dailiy limit time,please input again" << endl;
			continue;
		}
		input_circulation = false;
	}
	sleeptime = (limitTime / 2) * 1000;
	execCmd(connectstr.c_str());
	long long begin_uptime;
	string getstr = execCmd(uptimestr.c_str());
	begin_uptime = getruntime(getstr);
	cout << getstr << begin_uptime<< endl;
	while (true)
	{
		Sleep(sleeptime);
		long long now_uptime;
		getstr = execCmd(uptimestr.c_str());
		now_uptime = getruntime(getstr);
		usedtime += (now_uptime - begin_uptime);
		cout << getstr << endl <<now_uptime<<endl<<"used time:"<< usedtime<<endl;
		if ((now_uptime - begin_uptime) >= limitTime)
		{
			execCmd(closestr.c_str());
			break;
		}
	}
}



int main()
{
	const long long daily_limit_time = 7200;
	filestream in_and_out;
	//读文件功能
	in_and_out.readfile();
	if (in_and_out.date == execCmd(getdatestr.c_str()))
	{
		if (in_and_out.usedtime >= daily_limit_time)
		{
			cout << "out of today's time,close the tv" << endl;
			execCmd(closestr.c_str());
		}
		else
		{
			limit_funtion(daily_limit_time, in_and_out.usedtime);
		}
	}
	
	else
	{
		in_and_out.usedtime = 0;
		limit_funtion(daily_limit_time, in_and_out.usedtime);
	}
		
	//写文件
	in_and_out.date = execCmd(getdatestr.c_str());
	in_and_out.outToFile();
	//test();
	return 0;
}