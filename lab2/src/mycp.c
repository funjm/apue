#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFSIZE 16

int main(int argc, char *argv[])
{
    //打开两个文件；源文件fps、目的文件fpd
    FILE *fps, *fpd;
    int len = 0 ,ret = 0, pos = 0;
    int sfd, dfd;
    char buf[BUFSIZE];
    struct stat  * sts, std;


    //命令行参数要大于3个
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <src_file> <dest_file>",argv[0]);
        exit(1);
    }

    //打开源文件，获取fd
    //判断是否为目录
    sfd = open(argv[1], O_DIRECTORY);
    if(sfd>0)
    {       
        printf("open()：It's a dircetory 源文件为目录文件\n");
        exit(1);
    } 

    sfd = open(argv[1], O_RDONLY);
    if(sfd < 0 )
    {
        if(errno == ENOENT)
            fprintf(stderr, "open(): %s 源文件不存在\n", strerror(errno));
        else
            fprintf(stderr, "open(): %s\n", strerror(errno));
        exit(1);
    }
    //   if(sfd < 0 )
    // {
    //     if(errno == ENOENT)
    //     {
    //         printf("before fprintf\n");
    //         fprintf(stderr, "open(): %s 源文件不存在\n", strerror(errno));
    //         printf("after fprintf\n");
    //     }
            
    //     else
    //         fprintf(stderr, "open(): %s\n", strerror(errno));
    //     exit(1);
    // }

    //打开目标文件
    dfd = open(argv[2], O_RDONLY);

    if(dfd < 0)
    {
        // 打开文件失败,创建文件
        dfd = open(argv[2], O_WRONLY|O_CREAT,0700); 
    }
    else
    {
        // 打开文件成功,目的文件已存在
        char flag = 'y';
        printf("目的文件已存在，是否覆盖？(y\\n):");
        scanf("%c", &flag);
        if(flag == 'y')
        {
            //覆盖
            dfd = open(argv[2], O_WRONLY|O_TRUNC,0700);    
        }            
        else
        {
            //合并
            dfd = open(argv[2], O_WRONLY|O_APPEND,0700);
        }
    }  

     // 循环读写  
    while(1)
    {
        // memset(buf, 0,sizeof buf);
        len = read(sfd, buf, BUFSIZE);
        if(len < 0 )
        {
            perror("open()");
            break;
        }

        //读完了
        if(len == 0 )
            break;           
        //防止少写了        
        // printf("len : %d\n", len);
        ret = write(dfd, buf, len);
        if(ret < 0)
        {
            perror("open()");
            exit(1);
        }        
    }

    close(dfd);
    close(sfd);
    exit(0);
}

