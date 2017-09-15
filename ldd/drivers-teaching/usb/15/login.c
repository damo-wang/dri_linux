#include<stdio.h>
#include<stdlib.h>

int main()
{
 char *date;
 char name[50],pwd[20];
 printf("content-type:text/html;charset=gb2312\n\n");
 printf("<TITLE>登陆结果</TITLE>");
 printf("<H3>登陆结果</h3>");

 date=getenv("QUERY_STRING");
 if(date==NULL)
  printf("<p>错误：数据没有被输入或数据传输发生错误</p>");
 else{
  sscanf(date,"name=%[^&]&pwd=%s",name,pwd);
  printf("<p>name=%s</p>",name);
  printf("<p>pwd=%s</p>",pwd);
  printf("%s",date);
 }
 return 0;
}
