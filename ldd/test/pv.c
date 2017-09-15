#include <stdio.h>
#include <stdlib.h>
#define N 10

struct buffer
{
	int *data;//数据区
	int size;//数据区大小
	int count;//数据数
	int i_p;//生产者指针
	int i_c;//消费者指针
};

struct buffer buffer;

//生产者
int producer()
{
	int data=0;
	//生产数据
	printf("请输入数据:");
	scanf("%d",&data);
	//放入缓冲池
	if(buffer.count<buffer.size)
	{
		buffer.data[buffer.i_p]=data;
		buffer.i_p++;
		if(buffer.i_p==buffer.size)
		{
			buffer.i_p=0;
		}
		buffer.count++;
		printf("数据生产成功!\n");
	}
}

//消费者
int consumer()
{
	int data;
	//取出数据
	if(buffer.count>0)
	{
		data=buffer.data[buffer.i_c];
		buffer.i_c++;
		if(buffer.i_c==buffer.size)
		{
			buffer.i_c=0;
		}
		buffer.count--;
		printf("获取数据成功\n");
	}
	//消费数据
	printf("得到数据%d\n",data);
}

int main()
{
	int sel=0;//用户选择
	//初始化buffer
	buffer.size=N;
	buffer.data=malloc(buffer.size*sizeof(int));
	buffer.count=0;
	buffer.i_p=0;
	buffer.i_c=0;
	
	do
	{
		printf("请输入用户选择:1.生产数据; 2.消费数据; 0.退出\n");
		scanf("%d",&sel);
		switch(sel)
		{
		case 0:
			break;
		case 1:
			producer();
			break;
		case 2:
			consumer();
			break;
		default:
			break;
		}
	}while(sel);
	return 0;
}
