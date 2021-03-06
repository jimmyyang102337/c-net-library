#include"AppendFile.h"
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include"Logging.h"
#include<fcntl.h>
#include<sys/stat.h>
AppendFile::AppendFile(std::string filename):fp_(fopen(filename.c_str(),"ae")),writtenBytes_(0)
{
	setbuffer(fp_,buffer_,sizeof(buffer_));
}
AppendFile::~AppendFile()
{
	fclose(fp_);	
}

void AppendFile::append(const char *logline,const size_t len)
{
	size_t n=write(logline,len);
	size_t remain=len-n;
	while(remain>0)
	{
		size_t x=write(logline+n,remain);
		if(x==0)
		{
			int err=ferror(fp_);
			/*if(err)
				sprintf(stderr,"AppendFile::append() failed %s\n",strerror_tl(err));
			*/
			break;
		}
		n+=x;
		remain=len-n;
	}
	writtenBytes_+=len;
}
void AppendFile::Flush()
{
	fflush(fp_);	
}

size_t AppendFile::write(const char *logline,size_t len)
{
	return fwrite_unlocked(logline,1,len,fp_);	
}
