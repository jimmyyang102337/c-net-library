#include"../Logging.h"
#include<iostream>
#include<thread>
#include<unistd.h>
#include<vector>
#include"../LogFile.h"
using namespace std;

FILE* g_file;
std::unique_ptr<LogFile> g_logFile=NULL;

void dummyOutput(const char* msg, int len)
{
	    if (g_file)
			  {
				fwrite(msg, 1, len, g_file);
		      }
		else
			{
				g_logFile->append(msg,len);
			}
}
void fun(int j){
	for(int i=0;i<100000;++i){
		LOG_INFO<<j;
		}
	
	}
int main()
{
	Logger::setOutput(dummyOutput);
	g_logFile.reset(new LogFile("main",500,true));
	LOG_INFO<<getppid();
	vector<thread> v;
	for(int i=0;i<4;++i)
		{
		v.push_back(thread(fun,i));
	//	v[i].detach();
		}
	for(int i=0;i<4;++i)
		v[i].join();
//	g_file=fopen("log.txt","w");

//	Logger::setOutput(dummyOutput);
	{
/*	LOG_TRACE<<"trace";
	LOG_DEBUG<<"debug";
	LOG_INFO<<"INFO";
	LOG_WARN<<"warn";
	LOG_ERROR<<"ERROR";
*/
}
//	cout<<"main finished"<<endl;
//	fclose(g_file);
	//sleep(5);
	return 1;
	
	}
