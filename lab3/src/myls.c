#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>   
#include <sys/types.h>


#define CURDIRSIZE 1024
#define TIMEBUFSIZE 1024
#define FMTSTRSIZE 1024

static int selfflag  = 0, hideflag = 0;

void print_type(mode_t st_mode)
{
    char type;
    //文件类型
    if(S_ISREG(st_mode))
    {
        type =  '-';
    }else if(S_ISDIR(st_mode))
    {
        type = 'd';
    }else if(S_ISSOCK(st_mode))
    {
        type = 's';
    }else if(S_ISCHR(st_mode))
    {
        type = 'c';
    }else if(S_ISBLK(st_mode))
    {
        type = 'b';
    }else if(S_ISFIFO(st_mode))
    {
        type = 'P';
    }else if(S_ISLNK(st_mode))
    {
        type = 'l';
    }else{
        type = '?';
    }
    printf("%c",type);
}

void print_perm(mode_t st_mode)
{
    char type[9];
    memset(type, '-', sizeof type);
    //文件user权限
    if((S_IRUSR & st_mode) == S_IRUSR) 
    {
        type[0] = 'r';
    }
    if((S_IWUSR & st_mode) == S_IWUSR) 
    {
        type[1] = 'w';
    }
    if((S_IXUSR & st_mode) == S_IXUSR) 
    {
        type[2] = 'x';
    }
    if((S_IRWXU & st_mode) == S_IRWXU) 
    {
        type[2] = 's';
    }
    //文件group权限
    if((S_IRGRP & st_mode) == S_IRGRP) 
    {
        type[3] = 'r';
    }
    if((S_IWGRP & st_mode) == S_IWGRP) 
    {
        type[4] = 'w';
    }
    if((S_IXGRP & st_mode) == S_IXGRP) 
    {
        type[5] = 'x';
    }
    if((S_IRWXG & st_mode) == S_IRWXG) 
    {
        type[5] = 's';
    }
       //文件other权限
    if((S_IROTH & st_mode) == S_IROTH) 
    {
        type[6] = 'r';
    }
    if((S_IWOTH & st_mode) == S_IWOTH) 
    {
        type[7] = 'w';
    }
    if((S_IXOTH & st_mode) == S_IXOTH) 
    {
        type[8] = 'x';
    }
    if((S_IRWXO & st_mode) == S_IRWXO) 
    {
        type[8] = 's';
    }
    printf("%-9s",type);
}

void print_link(nlink_t st_nlink)
{
    printf(" %-2lu", st_nlink);
}

void print_username(uid_t st_uid)
{
    struct passwd *passwdres = getpwuid(st_uid); 
    printf(" %-5s",passwdres->pw_name);

}

void print_grname(gid_t st_gid)
{
    struct group *groupres = getgrgid(st_gid); 
    printf(" %-5s",groupres->gr_name);
}

void print_filename(struct dirent *cur)
{
    printf(" %-5s\n",cur->d_name);
}

void print_time(struct timespec mtime)
{
    struct tm *tm;
    char buf[TIMEBUFSIZE];
    tm = localtime(&mtime.tv_sec);
    if(tm->tm_year > (2020 - 1900))
        printf(" %-2d月 %-2d %-2d:%-2d",tm->tm_mon + 1 , tm->tm_mday, tm->tm_hour, tm->tm_min);
    else
        printf(" %-2d月 %-2d %-4d",tm->tm_mon + 1 , tm->tm_mday, tm->tm_year + 1900);
}

int main(int argc, char ** argv)
{
	
	FILE *fp = stdout;
    DIR *dp;
	char cur_dir[FMTSTRSIZE];
	struct dirent *cur;
    int c;

    while(1)
    {
        c = getopt(argc, argv, "hs");//:号代表有参，-号代表非选项传参， h代表打印隐藏文件，s代表打印自身
		if(c < 0)						
			break;
		switch(c)
        {
            case 'h':
                hideflag = 1;
                break;

            case 's':
                selfflag  = 1;
                break;
        }

    }

    //取得当前工作路径
	if(getcwd(cur_dir,CURDIRSIZE) == NULL)
    {
        fprintf(stderr, "getcwd(): %s\n", strerror(errno));
    }
    //打开目录，关联目录流
	dp = opendir(cur_dir);
	if(dp == NULL)
	{
		perror("Error code = %d\n");
		exit(1);
	}
    //读取目录流dir标识的目录下文件
	while((cur = readdir(dp)) != NULL  )
    {
        if(cur->d_name[0] != '.' || (selfflag && hideflag) || (selfflag && (cur->d_name[1] == '.' || !cur->d_name[1]))
            || (hideflag && (cur->d_name[1] != '.' && !cur->d_name[1] )) )
        {
            struct stat statres;
            if(lstat(cur->d_name, &statres) < 0)
            {
                perror("stat()");
			    continue;
            }
            
            print_type(statres.st_mode);
            print_perm(statres.st_mode);
            print_link(statres.st_nlink);
            print_username(statres.st_uid);
            print_grname(statres.st_gid);
            print_time(statres.st_mtim);
            print_filename(cur);
        }
	}
	closedir(dp);

	exit(0);
}