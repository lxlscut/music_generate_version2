// wav.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include"stdio.h"
#include"malloc.h"
#include"stdlib.h"
#include"math.h"
#include <windows.h>
#include<mmsystem.h>
#include"string.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;


#pragma comment(lib, "WINMM.LIB")

#define PI 3.14159265358979323846

#define LEN sizeof(struct XPT)          	//结构体size
typedef unsigned char  BYTE;
typedef unsigned short WORD;
struct XPT             //结构体链表的结点定义
{
	int X_seq;                  //小节序号
	int X_type;                 //类型编码
	int X_event;	            //事件编码
	float T_start;              //开始时刻
	float T_end;	            //结束时刻
	float T_scale;	            //时间比例
	float X_scale;	            //空间比例
	float N_row;	            //显示行数
	float N_col;	            //显示列数
	float X_table;              //所在表格区域
	float X_row;	            //所在行
	float X_col;	            //所在列
	int X_tone;                 //音调
	int Beat_note;	            //小节拍数
	int Note_div;	            //音符分数
	int Beat_minute;            //分钟拍数
	float X_vol;                //音量
	int X_grid;	                //小网格标识
	float Color_fore;	        //前景颜色
	float Color_back;	        //背景颜色
	int X_line;                 //线型
	struct XPT *pXptNext;       //指向下一个
};


/*定义一个新的结构体链表*/
struct PIANO             //结构体链表的结点定义
{
	int X_type;                 //类型编码
	int X_event;	            //事件编码
	float T_start;              //开始时刻
	float T_end;	            //结束时刻
	int X_tone;                 //音调
	int Beat_minute;            //分钟拍数
	float X_vol;                //音量
	int wav;                    //对应的音频文件
	float time;                   //音频持续的时间
	float  X_table;               //所在表格区域    
	struct PIANO *pXptNext;     //指向下一个
};
/**
 * @brief  创建一个新的结构体链表，头指向空
 * @param
 * @retval struct XPT* 链表头
 */
struct XPT* addxptlist()      //创建链表
{
	struct XPT *XptdateHead = 0, *XptdateTail = 0;
	XptdateHead = (XPT *)malloc(sizeof(XPT));//分配地址
	XptdateTail = XptdateHead;   //若是空链表则首位都一致
	XptdateTail->pXptNext = NULL;//结束创建
	return XptdateHead;
}
struct PIANO* LLL()      //创建链表
{
	struct PIANO *XptdateHead = 0, *XptdateTail = 0;
	XptdateHead = (PIANO *)malloc(sizeof(PIANO));//分配地址
	XptdateTail = XptdateHead;   //若是空链表则首位都一致
	XptdateTail->pXptNext = NULL;//结束创建
	return XptdateHead;
}


/**
 * @brief  在结构体链表的末尾添加一个结点
 * @param  结构体链表的头
 * @param  要加入的结点
 * @retval 无
 */
void Tailadd(struct XPT *XptHead, struct XPT *Xpttemp)             //末尾添加节点
{
	struct XPT *temp;
	temp = XptHead;
	while (temp->pXptNext != NULL)
	{
		temp = temp->pXptNext;
	}
	temp->pXptNext = Xpttemp;
	Xpttemp->pXptNext = NULL;
}
void Tailadd1(struct PIANO *XptHead, struct PIANO *Xpttemp)             //末尾添加节点
{
	struct PIANO *temp;
	temp = XptHead;
	while (temp->pXptNext != NULL)
	{
		temp = temp->pXptNext;
	}
	temp->pXptNext = Xpttemp;
	Xpttemp->pXptNext = NULL;
}
/**
 * @brief  打印输出结构体链表记录的有效信息
 * @param  结构体链表的头
 * @retval 无
 */
void printfXPT(struct XPT *head)             //打印结构体链表
{
	struct XPT *temp = head, *p = temp->pXptNext;
	while (p != NULL)
	{
		printf("%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%d\t%d\t%f\t%d\t%f\t%f\t%d\t%c\n"
			, p->X_seq, p->X_type, p->X_event, p->T_start, p->T_end, p->T_scale
			, p->X_scale, p->N_row, p->N_col, p->X_table, p->X_row, p->X_col
			, p->X_tone, p->Beat_note, p->Note_div, p->Beat_minute, p->X_vol
			, p->X_grid, p->Color_fore, p->Color_back, p->X_line, 'E');
		p = p->pXptNext;
	}
}
/**
 * @brief  打印PIANO结构体链表记录的有效信息
 * @param  结构体链表的头
 * @retval 无
 */
void printfPIANO(struct PIANO *head)
{
	int i = 10;
	struct PIANO *temp = head, *p = temp->pXptNext;
	while (p != NULL)
		//while(i--)
	{
		printf("%d\t%f\t%f\t%d\t%d\t%f\t%d\t%f\t\n\%d\t\n"
			, p->X_event, p->T_start, p->T_end,
			p->X_tone, p->Beat_minute, p->X_vol, p->wav, p->time, p->X_type
		);
		p = p->pXptNext;
	}
}

/**
 * @brief  把XPT文件的信息读出，存到结构体链表中
 * @param  要存储导入信息的结构体链表的头
 * @retval 无
 */
int readXPT(struct XPT *XptHead, const char *feilname2)            //读出XPT【参数1:结构体链表的头】
{
	FILE *fp;
	fp = fopen(feilname2, "rb");    //打开文件
	int i = 10;
	if (!fp)             //判断是否打开成功
	{
		printf("XPT文件打开失败!\n");
		exit(-1);
	}
	if (feof(fp) == 0)             //读出头信息
	{
		char head[90];
		int explain[6] = { 0,1,2,3,4,5 };
		float end = 0x01;

		fread(head, sizeof(head), 1, fp);
		fread(explain, sizeof(int), 6, fp);
		fread(&end, sizeof(float), 1, fp);

		printf("%s\n", head);
		for (int i = 0; i < 5; i++)
		{
			printf("%d\t", explain[i]);
		}
		printf("%f\n", end);
		printf("文件头打印成功");
	}
	printf("文件打开成功");
	while (feof(fp) == 0)
	{
		XPT *xptdatey;              //新建一个节点结构体
		xptdatey = (XPT *)malloc(sizeof(XPT));      //动态分配内存空间
		fread(xptdatey, sizeof(struct XPT), 1, fp);
		if (feof(fp) != 0)                   //如果到了文件末尾（总是多读出一组数据）
		{
			free(xptdatey);                  //释放多写的一段
			break;
		}
		Tailadd(XptHead, xptdatey);          //把结构体xptdatey1添加进结构体链表Xptdate		
	}
	fclose(fp);          //关闭文件
	//printfXPT(XptHead);          //打印结构体链表
}
/**
 * @brief  从XPT文件中获取与音频合成有关的信息
 * @param  xpt链表，piano链表
 * @retval 无
 */
void newlink(struct XPT *aim, struct PIANO *aim1)
{
	int i = 210;
	struct XPT *temp = aim, *p = temp->pXptNext;
	printf("\n1\n");
	while (p != NULL)
		//while(i--)
	{
		struct PIANO *aim2;
		aim2 = (PIANO *)malloc(sizeof(PIANO));      //动态分配内存空间		
		aim2->X_type = p->X_type;	            //事件类型
		aim2->X_event = p->X_event;	            //事件编码
		aim2->X_tone = p->X_tone;
		aim2->T_start = p->T_start * 60 * 44100 / p->Beat_minute * 2;     //文件开始位置float型,16位双声道
		aim2->T_end = p->T_end * 60 * 44100 / p->Beat_minute * 2;			//文件结束位置float型，16位双声道
		aim2->Beat_minute = p->Beat_minute;
		aim2->X_vol = p->X_vol;
		aim2->wav = p->X_event - p->X_tone;
		aim2->time = aim2->T_end - aim2->T_start;
		aim2->X_table = p->X_table;
		Tailadd1(aim1, aim2);
		p = p->pXptNext;
	}
	printf("有效使用值为:");
	printfPIANO(aim1);
}


void readlink(struct XPT *aim)//
{
	struct XPT *temp = aim, *p = temp->pXptNext;
	int a[10], i = 0;
	while (p != NULL)
	{
		a[i] = p->X_tone;
		p = p->pXptNext;
		i++;
	}
	for (int i = 0; i < 10; i++)
	{
		printf("%d ", a[i]);
	}
}

/**
 * @brief  把pcm文件转换成为wav文件，所需要的文件头
 * @param  pcmlen:裸数据长度，wavlen：返回的wav长度，字节数，samplerate：采样率,channum：通道数, bitpersample：量化比特
 * @retval wav数据数组
 */

char* addToWavhead(unsigned int pcmlen, unsigned int *wavlen, unsigned int samplerate, short channum, short bitpersample)
{
	//44字节wav头
	char *wav = (char *)malloc(44);
	//wav文件多了44个字节
	*wavlen = pcmlen + 44;
	//添加wav文件头
	memcpy(wav, "RIFF", 4);
	*(int *)((char*)wav + 4) = pcmlen + 36;  //存 数据长度
	memcpy(((char*)wav + 8), "WAVEfmt ", 8); // 注意有个空格
	*(int *)((char*)wav + 16) = 16;  //该子块后面还有16个字节。int 为4个字节
	*(short *)((char*)wav + 20) = 1; // 待存入的格式 1是 pcm 格式，short 为2个字节
	*(short *)((char*)wav + 22) = channum; //通道数2，单通道(Mono)值为1，双通道(Stereo)值为2
	*(int *)((char*)wav + 24) = samplerate;  //采样率44100，如8k，44.1k等
	*(int *)((char*)wav + 28) = samplerate * channum *bitpersample / 8;;  //  每秒所需的字节数，其值=SampleRate * NumChannels * BitsPerSample/8  
	*(short *)((char*)wav + 32) = bitpersample * channum / 8;     //    每个采样需要的字节数
	*(short *)((char*)wav + 34) = bitpersample;   // 每个采样点的量化bit
	strcpy((char*)((char*)wav + 36), "data");
	*(int *)((char*)wav + 40) = pcmlen;   // 裸数据的长度

	return wav;
}


/**
 * @brief  根据xpt文件合成pcm文件
 * @param  将要合成的文件名，所需的xpt文件名，所需的wav文件名
 * @retval 无
 */
int www(const char *feilname, const char *feilname2, const char *wavefilename, int n, float T_slip)
{
	//int n;
	int nump = 0;////////音频中最后一个点的位置
	float max = 0;//////单个音频文件的音量最大值
	//printf("请输入音频幅度（范围为0-15）：");
	//scanf("%d", &n);
	if (n < 0 || n>15)
	{
		printf("幅度值输入错误");
		exit(-1);
	}
	if (T_slip < 0)
	{
		printf("滚降时长设定错误");
		exit(-1);
	}
	int change = pow(2, n) - 1;/////////换算后的播放位数
	FILE *ff, *fe;
	int  sp, ep, tp, mp;///开始点，结束点，持续点数，音调。
	float vp;///////////音量
	unsigned char str[90][20] = {
		"E:\\pcm\\0.pcm","E:\\pcm\\1.pcm","E:\\pcm\\2.pcm","E:\\pcm\\3.pcm","E:\\pcm\\4.pcm","E:\\pcm\\5.pcm","E:\\pcm\\6.pcm","E:\\pcm\\7.pcm","E:\\pcm\\8.pcm","E:\\pcm\\9.pcm",
		"E:\\pcm\\10.pcm","E:\\pcm\\11.pcm","E:\\pcm\\12.pcm","E:\\pcm\\13.pcm","E:\\pcm\\14.pcm","E:\\pcm\\15.pcm","E:\\pcm\\16.pcm","E:\\pcm\\17.pcm","E:\\pcm\\18.pcm","E:\\pcm\\19.pcm",
		"E:\\pcm\\20.pcm","E:\\pcm\\21.pcm","E:\\pcm\\22.pcm","E:\\pcm\\23.pcm","E:\\pcm\\24.pcm","E:\\pcm\\25.pcm","E:\\pcm\\26.pcm","E:\\pcm\\27.pcm","E:\\pcm\\28.pcm","E:\\pcm\\29.pcm",
		"E:\\pcm\\30.pcm","E:\\pcm\\31.pcm","E:\\pcm\\32.pcm","E:\\pcm\\33.pcm","E:\\pcm\\34.pcm","E:\\pcm\\35.pcm","E:\\pcm\\36.pcm","E:\\pcm\\37.pcm","E:\\pcm\\38.pcm","E:\\pcm\\39.pcm",
		"E:\\pcm\\40.pcm","E:\\pcm\\41.pcm","E:\\pcm\\42.pcm","E:\\pcm\\43.pcm","E:\\pcm\\44.pcm","E:\\pcm\\45.pcm","E:\\pcm\\46.pcm","E:\\pcm\\47.pcm","E:\\pcm\\48.pcm","E:\\pcm\\49.pcm",
		"E:\\pcm\\50.pcm","E:\\pcm\\51.pcm","E:\\pcm\\52.pcm","E:\\pcm\\53.pcm","E:\\pcm\\54.pcm","E:\\pcm\\55.pcm","E:\\pcm\\56.pcm","E:\\pcm\\57.pcm","E:\\pcm\\58.pcm","E:\\pcm\\59.pcm",
		"E:\\pcm\\60.pcm","E:\\pcm\\61.pcm","E:\\pcm\\62.pcm","E:\\pcm\\63.pcm","E:\\pcm\\64.pcm","E:\\pcm\\65.pcm","E:\\pcm\\66.pcm","E:\\pcm\\67.pcm","E:\\pcm\\68.pcm","E:\\pcm\\69.pcm",
		"E:\\pcm\\70.pcm","E:\\pcm\\71.pcm","E:\\pcm\\72.pcm","E:\\pcm\\73.pcm","E:\\pcm\\74.pcm","E:\\pcm\\75.pcm","E:\\pcm\\76.pcm","E:\\pcm\\77.pcm","E:\\pcm\\78.pcm","E:\\pcm\\79.pcm",
		"E:\\pcm\\80.pcm","E:\\pcm\\81.pcm","E:\\pcm\\82.pcm","E:\\pcm\\83.pcm","E:\\pcm\\84.pcm","E:\\pcm\\85.pcm","E:\\pcm\\86.pcm","E:\\pcm\\87.pcm","E:\\pcm\\88.pcm","E:\\pcm\\89.pcm" };



	//通过键盘输入合成音乐时音符滚降的时长（ms）	
/*float T_slip = 0.0;
printf("请设置合成音乐时音符滚降的时长（ms）\n");
printf("T_slip=");
scanf("%f", &T_slip);*/

	float FS, Tp_cos;
	int  Np_cos, N_point;

	FS = 44100;
	//根据键盘输入合成音乐时音符滚降的时长（ms），产生一个周期为Tp = 4*T_slip 的cos信号，只需要1/4个周期
	Tp_cos = 4 * T_slip * 10e-3;
	Np_cos = int(FS * Tp_cos);
	N_point = Np_cos / 4;
	float *Win_cos;
	Win_cos = (float *)malloc(sizeof(float)*N_point);      //动态分配内存空间

	for (int i = 0; i < N_point; i++)//滚降窗数组
	{
		Win_cos[i] = cos(2.0*PI*i / Np_cos);
	}

	static float proce[52428800];///////////过渡时期的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		proce[i] = 0;
	}
	static short int pianosong[52428800];//经过比例放缩处理过后的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		pianosong[i] = 0;
	}

	static short int trans[2500000];//每次读取出的数据传输数组
	for (int i = 0; i < 2500000; i++)//数组初始化
	{
		trans[i] = 0;
	}
	struct XPT *MUSIC;    //定义相应的谱结构体链表
	MUSIC = addxptlist();   //创建结构体链表
	struct PIANO *SELECT;  //筛选有用信息
	SELECT = LLL();
	readXPT(MUSIC, feilname2);	  //读出相应的结构体链表信息
	printf("编译成功\n");
	newlink(MUSIC, SELECT);   //筛选操作

	struct PIANO *temp = SELECT, *p = temp->pXptNext;//链表操作
	while (p != NULL)
	{
		mp = p->wav;          //音调
		sp = int(p->T_start);      //开始点
		ep = int(p->T_end);         //结束点
		vp = p->X_vol;        //音量
		tp = int(p->time);         //持续点数
		if (p->X_type == 1)//如果是音频信息
		{
			const char *q = (const   char*)(char*)str[mp];
			ff = fopen(q, "rb");
			if (!ff)             //判断是否打开成功
			{
				printf("pcm源文件错误!\n");
				exit(-1);
			}
			else                //可以成功打开则读取数据
			{
				fread(trans, sizeof(trans), 1, ff);//中间数组？只读这么多数据出来吗？2500000
				for (int i = sp; i < ep; i++)//数据读取成功后进行叠加
				{
					proce[i] = proce[i] + trans[i - sp] * vp;
				}

				for (int i = ep; i < N_point + ep; i++)//滚余弦部分的叠加
				{
					//proce[i] = proce[i] + trans[ep-sp] * vp * Win_cos[i - ep];//这种是以最后一点的值来滚余弦；
					proce[i] = proce[i] + trans[i - sp] * vp * Win_cos[i - ep];//这种是以一段的值来滚余弦；
				}

				for (int i = 0; i < 2500000; i++)//叠加完毕后对传输数组清零，防止影响到下一个数组的数据。
				{
					trans[i] = 0;
				}
				printf("写入成功\n");
				fclose(ff);
			}
			if (nump < ep)
			{
				nump = ep;//算出最后一个点的位置
				if (ep > 483183820)
				{
					printf("文件太大！！！");
					exit(-1);
				}
			}
		}
		else
		{
			int g = 0;//无用判断
		}
		p = p->pXptNext;
	}
	for (int i = 0; i < nump; i++)//////找出过渡数组中的绝对值最大值
	{
		if (max <= fabs(proce[i]))
		{
			max = fabs(proce[i]);
		}
	}
	printf("the max is %f ", max);//打印出最大值
	for (int i = 0; i < nump; i++)//对过渡数组进行转换，生成最终要写入的数组
	{
		pianosong[i] = proce[i] / max * change;
	}

	free(Win_cos);
	printf("此处运行正常！");
	fe = fopen(feilname, "wb");
	if (!fe)
	{
		printf("pcm文件生成失败");
		exit(-1);
	}
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);
	printf("the sum of point is %d\n", nump);//打印出点的数目

	unsigned int pcmlen = nump * 2;
	unsigned int wavlen;
	unsigned int samplerate = 44100;
	short channum = 2;
	short bitpersample = 16;
	char *wave_head;//[44]

	wave_head = addToWavhead(pcmlen, &wavlen, samplerate, channum, bitpersample);

	fe = fopen(wavefilename, "wb");
	if (!fe)
	{
		printf("wav文件生成失败");
		exit(-1);
	}
	fwrite(wave_head, sizeof(char) * 44, 1, fe);//将最终的数组写入
	fseek(fe, sizeof(char) * 44, SEEK_SET);
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);

	delete(pianosong);
	delete(proce);
	return 0;


}

int wwl(const char *feilname, const char *feilname2, const char *wavefilename, int n, float T_slip)
{
	//int n;
	int nump = 0;////////音频中最后一个点的位置
	float max = 0;//////单个音频文件的音量最大值
	if (n < 0 || n>15)
	{
		printf("幅度值输入错误");
		exit(-1);
	}
	if (T_slip < 0)
	{
		printf("滚降时长设定错误");
		exit(-1);
	}
	int change = pow(2, n) - 1;/////////换算后的播放位数
	FILE *ff, *fe;
	int  sp, ep, tp, mp;///开始点，结束点，持续点数，音调。
	float vp;///////////音量
	unsigned char str[90][20] = {
		"E:\\pcm\\0.pcm","E:\\pcm\\1.pcm","E:\\pcm\\2.pcm","E:\\pcm\\3.pcm","E:\\pcm\\4.pcm","E:\\pcm\\5.pcm","E:\\pcm\\6.pcm","E:\\pcm\\7.pcm","E:\\pcm\\8.pcm","E:\\pcm\\9.pcm",
		"E:\\pcm\\10.pcm","E:\\pcm\\11.pcm","E:\\pcm\\12.pcm","E:\\pcm\\13.pcm","E:\\pcm\\14.pcm","E:\\pcm\\15.pcm","E:\\pcm\\16.pcm","E:\\pcm\\17.pcm","E:\\pcm\\18.pcm","E:\\pcm\\19.pcm",
		"E:\\pcm\\20.pcm","E:\\pcm\\21.pcm","E:\\pcm\\22.pcm","E:\\pcm\\23.pcm","E:\\pcm\\24.pcm","E:\\pcm\\25.pcm","E:\\pcm\\26.pcm","E:\\pcm\\27.pcm","E:\\pcm\\28.pcm","E:\\pcm\\29.pcm",
		"E:\\pcm\\30.pcm","E:\\pcm\\31.pcm","E:\\pcm\\32.pcm","E:\\pcm\\33.pcm","E:\\pcm\\34.pcm","E:\\pcm\\35.pcm","E:\\pcm\\36.pcm","E:\\pcm\\37.pcm","E:\\pcm\\38.pcm","E:\\pcm\\39.pcm",
		"E:\\pcm\\40.pcm","E:\\pcm\\41.pcm","E:\\pcm\\42.pcm","E:\\pcm\\43.pcm","E:\\pcm\\44.pcm","E:\\pcm\\45.pcm","E:\\pcm\\46.pcm","E:\\pcm\\47.pcm","E:\\pcm\\48.pcm","E:\\pcm\\49.pcm",
		"E:\\pcm\\50.pcm","E:\\pcm\\51.pcm","E:\\pcm\\52.pcm","E:\\pcm\\53.pcm","E:\\pcm\\54.pcm","E:\\pcm\\55.pcm","E:\\pcm\\56.pcm","E:\\pcm\\57.pcm","E:\\pcm\\58.pcm","E:\\pcm\\59.pcm",
		"E:\\pcm\\60.pcm","E:\\pcm\\61.pcm","E:\\pcm\\62.pcm","E:\\pcm\\63.pcm","E:\\pcm\\64.pcm","E:\\pcm\\65.pcm","E:\\pcm\\66.pcm","E:\\pcm\\67.pcm","E:\\pcm\\68.pcm","E:\\pcm\\69.pcm",
		"E:\\pcm\\70.pcm","E:\\pcm\\71.pcm","E:\\pcm\\72.pcm","E:\\pcm\\73.pcm","E:\\pcm\\74.pcm","E:\\pcm\\75.pcm","E:\\pcm\\76.pcm","E:\\pcm\\77.pcm","E:\\pcm\\78.pcm","E:\\pcm\\79.pcm",
		"E:\\pcm\\80.pcm","E:\\pcm\\81.pcm","E:\\pcm\\82.pcm","E:\\pcm\\83.pcm","E:\\pcm\\84.pcm","E:\\pcm\\85.pcm","E:\\pcm\\86.pcm","E:\\pcm\\87.pcm","E:\\pcm\\88.pcm","E:\\pcm\\89.pcm" };



	//通过键盘输入合成音乐时音符滚降的时长（ms）	
	float FS, Tp_cos;
	int  Np_cos, N_point;

	FS = 44100;
	//根据键盘输入合成音乐时音符滚降的时长（ms），产生一个周期为Tp = 4*T_slip 的cos信号，只需要1/4个周期
	Tp_cos = 4 * T_slip * 10e-3;
	Np_cos = int(FS * Tp_cos);
	N_point = Np_cos / 4;
	float *Win_cos;
	Win_cos = (float *)malloc(sizeof(float)*N_point);      //动态分配内存空间

	for (int i = 0; i < N_point; i++)//数组初始化
	{
		Win_cos[i] = cos(2.0*PI*i / Np_cos);
	}

	static float proce[52428800];///////////过渡时期的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		proce[i] = 0;
	}
	static short int trans[2500000];//每次读取出的数据传输数组
	static short int pianosong[52428800];//经过比例放缩处理过后的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		pianosong[i] = 0;
	}
	for (int i = 0; i < 2500000; i++)//数组初始化
	{
		trans[i] = 0;
	}
	struct XPT *MUSIC;    //定义相应的谱结构体链表
	MUSIC = addxptlist();   //创建结构体链表
	struct PIANO *SELECT;  //筛选有用信息
	SELECT = LLL();
	readXPT(MUSIC, feilname2);	  //读出相应的结构体链表信息
	printf("编译成功\n");
	newlink(MUSIC, SELECT);   //筛选操作

	struct PIANO *temp = SELECT, *p = temp->pXptNext;//链表操作
	while (p != NULL)
	{

		mp = p->wav;          //音调
		sp = int(p->T_start);      //开始点
		ep = int(p->T_end);         //结束点
		vp = p->X_vol;        //音量
		tp = int(p->time);         //持续点数
		if (p->X_type == 1 && p->X_table == 3)//如果是音频信息   X_table = 3 表示是低音谱
		{
			const char *q = (const   char*)(char*)str[mp];
			ff = fopen(q, "rb");
			if (!ff)             //判断是否打开成功
			{
				printf("pcm源文件错误!\n");
				exit(-1);
			}
			else                //可以成功打开则读取数据
			{

				fread(trans, sizeof(trans), 1, ff);//中间数组
				for (int i = sp; i < ep; i++)//数据读取成功后进行叠加
				{
					proce[i] = proce[i] + trans[i - sp] * vp;
				}

				for (int i = ep; i < N_point + ep; i++)//滚余弦部分的叠加
				{
					//proce[i] = proce[i] + trans[ep-sp] * vp * Win_cos[i - ep];//这种是以最后一点的值来滚余弦；
					proce[i] = proce[i] + trans[i - sp] * vp * Win_cos[i - ep];//这种是以一段的值来滚余弦；
				}


				for (int i = 0; i < 2500000; i++)//叠加完毕后对传输数组清零，防止影响到下一个数组的数据。
				{
					trans[i] = 0;
				}
				printf("写入成功\n");
				fclose(ff);
			}
			if (nump < ep)
			{
				nump = ep;//算出最后一个点的位置
			}
		}
		else
		{
			int g = 0;//无用判断
		}
		p = p->pXptNext;
	}
	for (int i = 0; i < nump; i++)//////找出过渡数组中的绝对值最大值
	{
		if (max <= fabs(proce[i]))
		{
			max = fabs(proce[i]);
		}
	}
	printf("the max is %f ", max);//打印出最大值
	for (int i = 0; i < nump; i++)//对过渡数组进行转换，生成最终要写入的数组
	{
		pianosong[i] = proce[i] / max * change;
	}

	free(Win_cos);
	printf("此处运行正常！");
	fe = fopen(feilname, "wb");
	if (!fe)
	{
		printf("pcm文件生成失败");
		exit(-1);
	}
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);
	printf("the sum of point is %d\n", nump);//打印出点的数目

	unsigned int pcmlen = nump * 2;
	unsigned int wavlen;
	unsigned int samplerate = 44100;
	short channum = 2;
	short bitpersample = 16;
	char *wave_head;//[44]

	wave_head = addToWavhead(pcmlen, &wavlen, samplerate, channum, bitpersample);


	fe = fopen(wavefilename, "wb");
	if (!fe)
	{
		printf("wav文件生成失败");
		exit(-1);
	}
	fwrite(wave_head, sizeof(char) * 44, 1, fe);//将最终的数组写入
	fseek(fe, sizeof(char) * 44, SEEK_SET);
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);


	return 0;


}

int wwh(const char *feilname, const char *feilname2, const char *wavefilename, int n, float T_slip)
{
	//int n;
	int nump = 0;////////音频中最后一个点的位置
	float max = 0;//////单个音频文件的音量最大值
	/*printf("请输入音频幅度（范围为0-15）：");
	scanf("%d", &n);*/
	if (n < 0 || n>15)
	{
		printf("幅度值输入错误");
		exit(-1);
	}
	int change = pow(2, n) - 1;/////////换算后的播放位数
	if (T_slip < 0)
	{
		printf("滚降时长设定错误");
		exit(-1);
	}
	FILE *ff, *fe;
	int  sp, ep, tp, mp;///开始点，结束点，持续点数，音调。
	float vp;///////////音量
	unsigned char str[90][20] = {
		"E:\\pcm\\0.pcm","E:\\pcm\\1.pcm","E:\\pcm\\2.pcm","E:\\pcm\\3.pcm","E:\\pcm\\4.pcm","E:\\pcm\\5.pcm","E:\\pcm\\6.pcm","E:\\pcm\\7.pcm","E:\\pcm\\8.pcm","E:\\pcm\\9.pcm",
		"E:\\pcm\\10.pcm","E:\\pcm\\11.pcm","E:\\pcm\\12.pcm","E:\\pcm\\13.pcm","E:\\pcm\\14.pcm","E:\\pcm\\15.pcm","E:\\pcm\\16.pcm","E:\\pcm\\17.pcm","E:\\pcm\\18.pcm","E:\\pcm\\19.pcm",
		"E:\\pcm\\20.pcm","E:\\pcm\\21.pcm","E:\\pcm\\22.pcm","E:\\pcm\\23.pcm","E:\\pcm\\24.pcm","E:\\pcm\\25.pcm","E:\\pcm\\26.pcm","E:\\pcm\\27.pcm","E:\\pcm\\28.pcm","E:\\pcm\\29.pcm",
		"E:\\pcm\\30.pcm","E:\\pcm\\31.pcm","E:\\pcm\\32.pcm","E:\\pcm\\33.pcm","E:\\pcm\\34.pcm","E:\\pcm\\35.pcm","E:\\pcm\\36.pcm","E:\\pcm\\37.pcm","E:\\pcm\\38.pcm","E:\\pcm\\39.pcm",
		"E:\\pcm\\40.pcm","E:\\pcm\\41.pcm","E:\\pcm\\42.pcm","E:\\pcm\\43.pcm","E:\\pcm\\44.pcm","E:\\pcm\\45.pcm","E:\\pcm\\46.pcm","E:\\pcm\\47.pcm","E:\\pcm\\48.pcm","E:\\pcm\\49.pcm",
		"E:\\pcm\\50.pcm","E:\\pcm\\51.pcm","E:\\pcm\\52.pcm","E:\\pcm\\53.pcm","E:\\pcm\\54.pcm","E:\\pcm\\55.pcm","E:\\pcm\\56.pcm","E:\\pcm\\57.pcm","E:\\pcm\\58.pcm","E:\\pcm\\59.pcm",
		"E:\\pcm\\60.pcm","E:\\pcm\\61.pcm","E:\\pcm\\62.pcm","E:\\pcm\\63.pcm","E:\\pcm\\64.pcm","E:\\pcm\\65.pcm","E:\\pcm\\66.pcm","E:\\pcm\\67.pcm","E:\\pcm\\68.pcm","E:\\pcm\\69.pcm",
		"E:\\pcm\\70.pcm","E:\\pcm\\71.pcm","E:\\pcm\\72.pcm","E:\\pcm\\73.pcm","E:\\pcm\\74.pcm","E:\\pcm\\75.pcm","E:\\pcm\\76.pcm","E:\\pcm\\77.pcm","E:\\pcm\\78.pcm","E:\\pcm\\79.pcm",
		"E:\\pcm\\80.pcm","E:\\pcm\\81.pcm","E:\\pcm\\82.pcm","E:\\pcm\\83.pcm","E:\\pcm\\84.pcm","E:\\pcm\\85.pcm","E:\\pcm\\86.pcm","E:\\pcm\\87.pcm","E:\\pcm\\88.pcm","E:\\pcm\\89.pcm" };



	//通过键盘输入合成音乐时音符滚降的时长（ms）	


	float FS, Tp_cos;
	int  Np_cos, N_point;

	FS = 44100;
	//根据键盘输入合成音乐时音符滚降的时长（ms），产生一个周期为Tp = 4*T_slip 的cos信号，只需要半个周期
	Tp_cos = 4 * T_slip * 10e-3;
	Np_cos = int(FS * Tp_cos);
	N_point = Np_cos / 4;
	float *Win_cos;
	Win_cos = (float *)malloc(sizeof(float)*N_point);      //动态分配内存空间

	for (int i = 0; i < N_point; i++)//数组初始化
	{
		Win_cos[i] = cos(2.0*PI*i / Np_cos);
	}


	static float proce[52428800];///////////过渡时期的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		proce[i] = 0;
	}
	static short int trans[2500000];//每次读取出的数据传输数组
	static short int pianosong[52428800];//经过比例放缩处理过后的音频数组
	for (int i = 0; i < 52428800; i++)//数组初始化
	{
		pianosong[i] = 0;
	}
	for (int i = 0; i < 2500000; i++)//数组初始化
	{
		trans[i] = 0;
	}
	struct XPT *MUSIC;    //定义相应的谱结构体链表
	MUSIC = addxptlist();   //创建结构体链表
	struct PIANO *SELECT;  //筛选有用信息
	SELECT = LLL();
	readXPT(MUSIC, feilname2);	  //读出相应的结构体链表信息
	printf("编译成功\n");
	newlink(MUSIC, SELECT);   //筛选操作

	struct PIANO *temp = SELECT, *p = temp->pXptNext;//链表操作
	while (p != NULL)
	{

		mp = p->wav;          //音调
		sp = int(p->T_start);      //开始点
		ep = int(p->T_end);         //结束点
		vp = p->X_vol;        //音量
		tp = int(p->time);         //持续点数
		if (p->X_type == 1 && p->X_table == 1)//如果是音频信息    X_table = 1 表示是高音谱
		{
			const char *q = (const   char*)(char*)str[mp];
			ff = fopen(q, "rb");
			if (!ff)             //判断是否打开成功
			{
				printf("pcm源文件错误!\n");
				exit(-1);
			}
			else                //可以成功打开则读取数据
			{

				fread(trans, sizeof(trans), 1, ff);//中间数组
				for (int i = sp; i < ep; i++)//数据读取成功后进行叠加
				{
					proce[i] = proce[i] + trans[i - sp] * vp;
				}

				for (int i = ep; i < N_point + ep; i++)//滚余弦部分的叠加
				{
					//proce[i] = proce[i] + trans[ep-sp] * vp * Win_cos[i - ep];//这种是以最后一点的值来滚余弦；
					proce[i] = proce[i] + trans[i - sp] * vp * Win_cos[i - ep];//这种是以一段的值来滚余弦；
				}


				for (int i = 0; i < 2500000; i++)//叠加完毕后对传输数组清零，防止影响到下一个数组的数据。
				{
					trans[i] = 0;
				}
				printf("写入成功\n");
				fclose(ff);
			}
			if (nump < ep)
			{
				nump = ep;//算出最后一个点的位置
			}
		}
		else
		{
			int g = 0;//无用判断
		}
		p = p->pXptNext;
	}
	for (int i = 0; i < nump; i++)//////找出过渡数组中的绝对值最大值
	{
		if (max <= fabs(proce[i]))
		{
			max = fabs(proce[i]);
		}
	}
	printf("the max is %f ", max);//打印出最大值
	for (int i = 0; i < nump; i++)//对过渡数组进行转换，生成最终要写入的数组
	{
		pianosong[i] = proce[i] / max * change;
	}

	free(Win_cos);
	printf("此处运行正常！");
	fe = fopen(feilname, "wb");
	if (!fe)
	{
		printf("pcm文件生成失败");
		exit(-1);
	}
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);
	printf("the sum of point is %d\n", nump);//打印出点的数目

	unsigned int pcmlen = nump * 2;
	unsigned int wavlen;
	unsigned int samplerate = 44100;
	short channum = 2;
	short bitpersample = 16;
	char *wave_head;//[44]

	wave_head = addToWavhead(pcmlen, &wavlen, samplerate, channum, bitpersample);


	fe = fopen(wavefilename, "wb");
	if (!fe)
	{
		printf("wav文件生成失败");
		exit(-1);
	}
	fwrite(wave_head, sizeof(char) * 44, 1, fe);//将最终的数组写入
	fseek(fe, sizeof(char) * 44, SEEK_SET);
	fwrite(pianosong, sizeof(short int)*nump, 1, fe);//将最终的数组写入
	fclose(fe);


	return 0;


}

int wwl_h(const char *pcmh, const char *pcml, const char *xptname, const char *wavh, const char *wavl, int n, float T_slip)
{
	wwl(pcml, xptname, wavl, n, T_slip);
	wwh(pcmh, xptname, wavh, n, T_slip);
	return 0;
}
int main()
{
	//获得文件路径
	char pctname[500];  //pct路径名
	short int hech_flag = 1;   //合成一个文件，还是高音谱（右手），低音谱（左手）文件：1，一个文件；2：两个文件

	printf("请输入xpt路径名后回车，例如E:\\爱丽丝.XPT\n");  // E:\pxt\致爱丽丝_水平.XPT
	//gets(pctname);
	cin.get(pctname, 500);
	printf("合成1个文件，还是2个文件：高音谱（右手），低音谱（左手）文件。故1：一个文件；2：两个文件 \n");
	scanf("%d", &hech_flag);
	if (hech_flag == 1)
	{
		//合成文件名字自动生成
		char *houzhui;

		char roadstr[500]; //
		char pcmname[500];  //合成的pcm文件的路径名
		char wavname[500];  //合成的wav文件的路径名

		houzhui = strrchr(pctname, '.');
		if (houzhui == NULL)
		{
			printf("文件名格式错误！！！！");
			exit(-1);
		}
		int roadlen = strlen(pctname) - strlen(houzhui);

		memcpy(roadstr, pctname, roadlen);
		roadstr[roadlen] = '\0';
		strcpy(pcmname, roadstr);
		strcpy(wavname, roadstr);
		strcat(pcmname, ".pcm");
		strcat(wavname, ".wav");
		int n;
		printf("请输入音频幅度（范围为0-15）：");
		scanf("%d", &n);
		float T_slip = 0.0;
		printf("请设置合成音乐时音符滚降的时长（ms）\n");
		printf("T_slip=");
		scanf("%f", &T_slip);
		www(pcmname, pctname, wavname, n, T_slip);

	}

	if (hech_flag == 2)
	{
		//合成文件名字自动生成
		char* houzhui;
		char roadstr[500];
		char pcmlname[500];  //合成的低音谱pcm文件的路径名
		char wavlname[500];  //合成的低音谱wav文件的路径名

		char pcmhname[500];  //合成的高音谱pcm文件的路径名
		char wavhname[500];  //合成的高音谱wav文件的路径名

		houzhui = strrchr(pctname, '.');
		if (houzhui == NULL)
		{
			printf("文件名格式错误！！！！");
			exit(-1);
		}
		int roadlen = strlen(pctname) - strlen(houzhui);

		memcpy(roadstr, pctname, roadlen);
		roadstr[roadlen] = '\0';

		strcpy(pcmlname, roadstr);
		strcpy(wavlname, roadstr);
		const char* lpamname = strcat(pcmlname, "l.pcm");
		const char* lwavname = strcat(wavlname, "l.wav");

		strcpy(pcmhname, roadstr);
		strcpy(wavhname, roadstr);
		const char* hpamname = strcat(pcmhname, "h.pcm");
		const char* hwavname = strcat(wavhname, "h.wav");
		int n;
		printf("请输入音频幅度（范围为0-15）：");
		scanf("%d", &n);
		float T_slip = 0.0;
		printf("请设置合成音乐时音符滚降的时长（ms）\n");
		printf("T_slip=");
		scanf("%f", &T_slip);
		//wwl(pcmlname, pctname, wavlname,n,T_slip);

		/*strcpy(pcmhname, roadstr);
		strcpy(wavhname, roadstr);
		strcat(pcmhname, "h.pcm");
		strcat(wavhname, "h.wav");*/
		//wwh(pcmhname, pctname, wavhname, n, T_slip);
		wwl_h(hpamname, lpamname, pctname, hwavname, lwavname, n, T_slip);
	}
	return 0;
}







