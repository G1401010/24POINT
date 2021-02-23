#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <process.h>
#include<Windows.h>
#define TOTAL 1362//总题数
//题号从0开始
int Xofboard=0,Yofboard=0;//游戏界面位置
int anim=0;//视觉暂留时间
int stop;//指示100秒时间到了没，为0代表还没到，为1代表到了
int donenum;//做出的题数
int undonum;//未做出的题数,用于最后显示错题解法
int undone[800];//记录未做出的题目
char key[12];//记录键位信息，key[0-3]为ABCD四数；key[4-7]为加减乘除；key[8]为ERASE键；key[9]为SKIP键；key[10]为左归位；key[11]为右归位
int solvment[8];//s[0]:何种解法，为0为不加括号解（judg4）；为1为加括号解(judg4_special)；s[1-4]:题给四数；s[5-7]:1-3次进行了何种运算，详见judg函数
int opemes[3];//记录三次运算信息，与s[5-7]不同之处在于减和除，opemes没运算顺序；而s[5-7]有运算顺序；二者用exso_to_op()转换
char ok;//开始标志，为Y就开始。用于welcome
int ope(int *num,int *den,int input,int c)//用于四则运算；c为选择运算符及选择两数减除运算先后关系；a=num/den及b=input为参加运算的数字
{
    int middle;
    switch(c)
    {
    case 0:
        *num=*num+*den*input;//a+b
        break;
    case 1:
        *num=*num-*den*input;//a-b
        break;
    case 2:
        *num=*den*input-*num;//b-a
        break;
    case 3:
        *num*=input;//a*b
        break;
    case 4:
        *den*=input;//a/b
        break;
    case 5:
        if(*num==0)//b/a
            return 0;
        else
        {
            middle=*num;
            *num=input**den;
            *den=middle;
        }
        break;
    }
    return 1;
}


int ope_special(int *num,int *den,int num1,int den1,int c)//当a与b皆为分数num/den时的四则运算（a为num/den;b为num2/den2)，c用于选择不同的运算关系
{
    int middle;
    switch(c)
    {
         case 0:
        *num=*num*den1+*den*num1;//a+b
        *den*=den1;
        break;
    case 1:
        *num=*num*den1-*den*num1;//a-b
        *den*=den1;
        break;
    case 2:
        *num=*den*num1-*num*den1;//b-a
        *den*=den1;
        break;
    case 3:
        *num*=num1;//a*b
        *den*=den1;
        break;
    case 4:
        if(num1==0)//a/b
            return 0;
        else
        {
            *num*=den1;
            *den*=num1;
        }
        break;
    case 5:
        if(*num==0)//b/a
            return 0;
        else
        {
            middle=*num;
            *num=num1**den;
            *den=den1*middle;
        }
        break;
    }
    return 1;
}


int judg2(int num,int den,int input)//判断a=num/den与b=input是否可能通过四则运算算出24,用于judge4a
{
    int i,numm,denn;
    numm=num;
    denn=den;
    for(i=0;i<6;i++)
    {
        ope(&num,&den,input,i);
        if(num%den==0&&num/den==24)
        {
            num=numm;
            den=denn;
            return i;
        }
        else
        {
            num=numm;
            den=denn;
        }
    }
    return 6;
}


int judg2_special(int num,int den,int num1,int den1)//判断a=num/den与b=num1/den1是否可能通过四则运算算出24,用于judge4_special
{
    int i,numm,denn;
    numm=num;
    denn=den;
    for(i=0;i<6;i++)
    {
        ope_special(&num,&den,num1,den1,i);
        if(num%den==0&&num/den==24)
        {
            num=numm;
            den=denn;
            return i;
        }
        else
        {
            num=numm;
            den=denn;
        }
    }
    return 6;
}


int judg4(int a,int b,int c,int d)//判断a,b,c,d是否能通过不加括号的四则运算算出24
{
    int e,f,g,h,i,j,k,l,n,o;
    int m[4];
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//四取二进行四则运算
            {
                m[0]=a;
                m[1]=b;
                m[2]=c;
                m[3]=d;
                int num=m[e],den=1;
                ope(&num,&den,m[f],g);
                del(&m,e);
                del(&m,f-1);
                k=m[0];
                l=m[1];
                n=num;
                o=den;
                for(h=0;h<2;h++)//二取一与num/den进行四则运算
                {
                    for(i=0;i<6;i++)
                    {
                        m[0]=k;
                        m[1]=l;
                        num=n;
                        den=o;
                        ope(&num,&den,m[h],i);
                        del(&m,h);
                        j=judg2(num,den,m[0]);//最后判断
                        if(j==6)
                            continue;
                        else
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}


int judg4_special(int a,int b,int c,int d)//判断a,b,c,d是否能通过加括号的四则运算算出24
{
    int m[4];
    int e,f,g,h,i,j,k,l,n,o;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//四取二进行四则运算
            {
                m[0]=a;
                m[1]=b;
                m[2]=c;
                m[3]=d;
                int num=m[e],den=1;
                ope(&num,&den,m[f],g);
                del(&m,e);
                del(&m,f-1);
                i=m[0];
                j=m[1];
                for(h=0;h<6;h++)
                {
                    int num1=m[0],den1=1;
                    ope(&num1,&den1,m[1],h);
                    i=judg2_special(num,den,num1,den1);
                    if(i==6)
                        continue;
                    else
                        return 1;
                }
            }
        }
    }
    return 0;
}


void del(int m[],int i)//用于judg4和judg4_special函数,删去数组中已经参与过计算的数
{
    int a;
    for(a=i;a<4;a++)
    {
        m[a]=m[a+1];
    }
}


int judg(int a,int b,int c,int d)//判断a,b,c,d是否能通过四则运算算出24
{
    if(judg4(a,b,c,d)==0&&judg4_special(a,b,c,d)==0)
        return 0;
    else
        return 1;
}


void creat(int m[])//产生所有能算出24点的4数组合，共1362题
{
    int a,b,c,d,i=0;
    for(a=1;a<14;a++)
    {
        for(b=a;b<14;b++)
        {
            for(c=b;c<14;c++)
            {
                for(d=c;d<14;d++)
                {
                    if(judg(a,b,c,d)==0)
                        continue;
                    else
                    {
                        m[4*i+0]=a;
                        m[4*i+1]=b;
                        m[4*i+2]=c;
                        m[4*i+3]=d;
                        i++;
                    }
                }
            }
        }
    }
}


void crerand(int m[])//创建200个随机数，存于a[200]中，这两百个随机数即为要考察的200题的题号,这两百道题的800个数存于m[800]中,即存于m[800]里的题是一定有解的
{
    int i,a[200],b[TOTAL*4];
    creat(&b);
    srand((unsigned)time(NULL));
    for(i=0;i<200;i++)
    {
        a[i]=rand()%1362;
    }
    for(i=0;i<200;i++)
    {
        m[4*i+0]=b[4*a[i]+0];
        m[4*i+1]=b[4*a[i]+1];
        m[4*i+2]=b[4*a[i]+2];
        m[4*i+3]=b[4*a[i]+3];
    }
}


void crerand_notsure(int m[])//创建两百道题，800个数存于m[800]中（不一定有解）
{
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<800;i++)
    {
        m[i]=rand()%13+1;
    }
}


void coudon(int time)//倒计时显示程序，一定要放在边框创建之后，要利用线程创建函数_beginthread(coudon,0,100)实现
{
    int i;
    int n;
    //gotoxy(Xofboard+40,Yofboard+4);
    //printf("%d",time);
    for(i=time;i>-1;i--)
    {
        for(n=0;n<5;n++)
        {
             gotoxy(Xofboard+29,Yofboard+4);
             printf("TIME LEFT: %d S",i);
             Sleep(200);
        }
       gotoxy(Xofboard+41,Yofboard+4);
       printf("     ");
    }
    stop=1;
    Finish();
}


void errormes(int i)//当做错时显示"ERROR ! Your method cannot get 24."或"ERROR ! 0 cannot be divisor."，持续1秒(P.S.之所以取1001毫秒是为了不干扰倒计时),要利用线程创建函数 _beginthread(errormes,0,0)或_beginthread(errormes,0,1)实现
{
    int n;
    for(n=0;n<30;n++)
    {
        gotoxy(Xofboard+8,Yofboard+8);
        if(i==0)
            printf("ERROR ! Your method cannot get 24.");
        else
            printf("ERROR ! 0 cannot be divisor.");
       Sleep(100);
    }
    gotoxy(Xofboard+8,Yofboard+8);
    printf("                                  ");
}


void skipdly()
{
    int i;
    system("cls");
     for(i=0;i<25;i++)
    {
        gotoxy(Xofboard,Yofboard+i);
        printf("■");
        gotoxy(Xofboard+48,Yofboard+i);
        printf("■");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(Xofboard+i,Yofboard);
        printf("■");
        gotoxy(Xofboard+i,Yofboard+24);
        printf("■");
    }
    gotoxy(Xofboard+22,Yofboard+9);
    printf("■■■");
    gotoxy(Xofboard+4+22,Yofboard+1+9);
    printf("■");
    gotoxy(Xofboard+22,Yofboard+2+9);
    printf("■■■");
    gotoxy(Xofboard+4+22,Yofboard+3+9);
    printf("■");
    gotoxy(Xofboard+22,Yofboard+4+9);
    printf("■■■");
    Sleep(1000);
    gotoxy(Xofboard+4+22,Yofboard+3+9);
    printf("  ");
    gotoxy(Xofboard+22,Yofboard+3+9);
    printf("■");
    Sleep(1000);
    system("cls");
    for(i=0;i<25;i++)
    {
        gotoxy(Xofboard,Yofboard+i);
        printf("■");
        gotoxy(Xofboard+48,Yofboard+i);
        printf("■");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(Xofboard+i,Yofboard);
        printf("■");
        gotoxy(Xofboard+i,Yofboard+24);
        printf("■");
    }
    for(i=0;i<5;i++)
    {
        gotoxy(Xofboard+2+22,Yofboard+i+9);
        printf("■");
    }
    Sleep(1000);
    system("cls");
}


void showkey()
{
     gotoxy(Xofboard+54,Yofboard+1);
     if(key[0]==' ')
        printf("A数键位:空格");
     else
        printf("A数键位:%c",key[0]);
    gotoxy(Xofboard+54,Yofboard+3);
    if(key[1]==' ')
        printf("B数键位:空格");
     else
        printf("B数键位:%c",key[1]);
     gotoxy(Xofboard+54,Yofboard+5);
     if(key[2]==' ')
        printf("C数键位:空格");
     else
        printf("C数键位:%c",key[2]);
     gotoxy(Xofboard+54,Yofboard+7);
    if(key[3]==' ')
        printf("D数键位:空格");
    else
        printf("D数键位:%c",key[3]);
     gotoxy(Xofboard+54,Yofboard+9);
    if(key[4]==' ')
        printf("加号键位:空格");
     else
        printf("加号键位:%c",key[4]);
     gotoxy(Xofboard+54,Yofboard+11);
    if(key[5]==' ')
        printf("减号键位:空格");
     else
       printf("减号键位:%c",key[5]);
     gotoxy(Xofboard+54,Yofboard+13);
    if(key[6]==' ')
        printf("乘号键位:空格");
     else
        printf("乘号键位:%c",key[6]);
     gotoxy(Xofboard+54,Yofboard+15);
    if(key[7]==' ')
        printf("除号键位:空格");
     else
        printf("除号键位:%c",key[7]);
     gotoxy(Xofboard+54,Yofboard+17);
     if(key[8]==' ')
        printf("擦除键位:空格");
     else
        printf("擦除键位:%c",key[8]);
     gotoxy(Xofboard+54,Yofboard+19);
    if(key[9]==' ')
        printf("跳过键位:空格");
     else
        printf("跳过键位:%c",key[9]);
     gotoxy(Xofboard+54,Yofboard+21);
     if(key[10]==' ')
        printf("左归位键:空格");
     else
        printf("左归位键:%c",key[10]);
     gotoxy(Xofboard+54,Yofboard+23);
     if(key[11]==' ')
        printf("右归位键:空格");
     else
        printf("右归位键:%c",key[11]);
}


void gotoxy(int x, int y)//移动光标至（x,y），具体原理我也不知，这是从网上抄的代码
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}


void gotoprint(int x, int y,int num,int den,int c)//用于在（x,y）处打印/删除边框、打印数字、打印加减乘除号；c用来选择不同操作
{
	gotoxy(x, y);
	switch(c)
	{
	    case 0://打印边框
	        printf(".");
	        break;
	    case 1://删除边框
            printf("  ");
            break;
        case 2://打印整数
            if(num<10)
                gotoxy(x+2, y);
            else
                if(num<1000)
                gotoxy(x+1,y);
            printf("%d",num);
            break;
        case 3://打印分数
            if(num<10)
                gotoxy(x+2, y);
            else
                if(num<1000)
                gotoxy(x+1,y);
            printf("%d",num);
            gotoxy(x, y+1);
            printf("-----");
            if(den<10)
                gotoxy(x+2, y+2);
            else
                if(den<1000)
                gotoxy(x+1,y+2);
                else
                gotoxy(x, y+2);
            printf("%d",den);
            break;
        case 4://打印加号
            printf("+");
            break;
        case 5://打印减号
            printf("-");
            break;
        case 6://打印乘号
            printf("*");
            break;
        case 7://打印除号
            printf("/");
            break;
	}
}


void crebor3(int x, int y,int c)//在（x,y）处显示带边框的运算符，c为4到7，对应选择加减乘除
{
    int i;
    for(i=0;i<3;i++)
    {
        gotoprint(x+2*i, y+0,0,0,0);
    }
    gotoprint(x+0, y+1,0,0,0);
    gotoprint(x+2,y+1,0,0,c);
    gotoprint(x+4, y+1,0,0,0);
    for(i=0;i<3;i++)
    {
        gotoprint(x+2*i, y+2,0,0,0);
    }
}


void crebor3x4(int x,int y)//在（x,y）处显示四个带边框的运算符+-*/，间隔为2
{
    int i;
    for(i=1;i<2*4;i+=2)
    {
        crebor3(x+4*i,y,4+((i-1)/2));
    }
}


void crebor5(int x,int y,int num,int den,int c)//在（x,y）处显示带5*5边框的整数或分数，c用来选择打印整数还是分数，0为整数，1为分数,2为仅打印边框
{
    int i;
    if(c==0)
        gotoprint(x+2, y+2,num,den,2);
    else if(c==1)
        gotoprint(x+2, y+1,num,den,3);
    else if(c==2)
        gotoprint(x+1, y+1,num,den,1);
    else
        printf("ERROR ! c given to crebor5 is beyond designed number.");
    for(i=0;i<5;i++)
    {
        gotoprint(x+2*i, y+0,0,0,0);
    }
    for(i=1;i<4;i++)
    {
        gotoprint(x+0, y+i,0,0,0);
    }
    for(i=1;i<4;i++)
    {
        gotoprint(x+8, y+i,0,0,0);
    }
    for(i=0;i<5;i++)
    {
        gotoprint(x+2*i, y+4,0,0,0);
    }
}


void crebor5x4(int x,int y,int numa,int dena,int numb,int denb,int numc,int denc,int numd,int dend,int a,int b,int c,int d)//在（x,y）处显示4个带5*5边框的整数或分数A,B,C,D，a,b,c,d用来选择ABCD打印整数、分数还是不打印：0为整数；1为分数；2为仅打印边框
{
    int i,num[4],den[4],swich[4];
    num[0]=numa;
    num[1]=numb;
    num[2]=numc;
    num[3]=numd;
    den[0]=dena;
    den[1]=denb;
    den[2]=denc;
    den[3]=dend;
    swich[0]=a;
    swich[1]=b;
    swich[2]=c;
    swich[3]=d;
    for(i=1;i<2*4;i+=2)
    {
        crebor5(x+6*i,y,num[(i-1)/2],den[(i-1)/2],swich[(i-1)/2]);
    }
}


void crebor535(int x,int y,int num1,int den1,int num2,int den2,int a,int b,int c)//在（x,y）处显示2个带5*5边框的整数或分数A,B,，它们夹着一个带3*3边框的运算符，a,b用来选择AB整数还是分数：0为整数、1为分数、2为空框；c取值0，1，2，3，-3用于选择打印+-*/,还是空框
{
    crebor5(x,y,num1,den1,a);
    crebor3(x+12, y+1,c+4);
    crebor5(x+20,y,num2,den2,b);
}


void crebordwn1(int x,int y,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//创建两数运算时界面下半部分方框，upnuma(b)/updena(b)为参加运算两数，numa(bcd)/dena(bcd)为底部题目框中的数字
{
    x=x-2;
    int i;
    for(i=0;i<9;i++)
    {
        gotoprint(x+2,y+i,0,0,0);
        gotoprint(x+6,y+i,0,0,0);
        gotoprint(x+42,y+i,0,0,0);
        gotoprint(x+46,y+i,0,0,0);
    }
    gotoprint(x+4,y,0,0,0);
    gotoprint(x+44,y,0,0,0);
    gotoprint(x+4,y+i-1,0,0,0);
    gotoprint(x+44,y+i-1,0,0,0);
    gotoxy(x+4,y+2);
    printf("E");
    gotoxy(x+4,y+3);
    printf("R");
    gotoxy(x+4,y+4);
    printf("A");
    gotoxy(x+4,y+5);
    printf("S");
    gotoxy(x+4,y+6);
    printf("E");
    gotoxy(x+44,y+2);
    printf("S");
    gotoxy(x+44,y+3);
    printf("K");
    gotoxy(x+44,y+4);
    printf("I");
    gotoxy(x+44,y+5);
    printf("P");
    crebor535(x+10,y,upnuma,updena,upnumb,updenb,ua,ub,uc);
    crebor3x4(x+6,y+6);
    crebor5x4(x-4,y+10,numa,dena,numb,denb,numc,denc,numd,dend,a,b,c,d);
}


void crebordwn2(int x,int y,int upnum,int upden,int up,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//创建运算出结果时界面下半部分方框，upnum/upden为计算结果，numa(bcd)/dena(bcd)为底部题目框中的数字
{
    x=x-2;
    int i;
    for(i=0;i<9;i++)
    {
        gotoprint(x+2,y+i,0,0,0);
        gotoprint(x+6,y+i,0,0,0);
        gotoprint(x+42,y+i,0,0,0);
        gotoprint(x+46,y+i,0,0,0);
    }
    gotoprint(x+4,y,0,0,0);
    gotoprint(x+44,y,0,0,0);
    gotoprint(x+4,y+i-1,0,0,0);
    gotoprint(x+44,y+i-1,0,0,0);
    gotoxy(x+4,y+2);
    printf("E");
    gotoxy(x+4,y+3);
    printf("R");
    gotoxy(x+4,y+4);
    printf("A");
    gotoxy(x+4,y+5);
    printf("S");
    gotoxy(x+4,y+6);
    printf("E");
    gotoxy(x+44,y+2);
    printf("S");
    gotoxy(x+44,y+3);
    printf("K");
    gotoxy(x+44,y+4);
    printf("I");
    gotoxy(x+44,y+5);
    printf("P");
    crebor5(x+20,y,upnum,upden,up);
    crebor3x4(x+6,y+6);
    crebor5x4(x-4,y+10,numa,dena,numb,denb,numc,denc,numd,dend,a,b,c,d);
}


void crebor1(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//创建带倒计时和分数的界面整体，score为得分，time为时间，swit=0时显示计算结果界面，swit=1时显示两数计算界面，upnuma(b)/updena(b)为参加运算两数，numa(bcd)/dena(bcd)为底部题目框中的数字
{
    int i;
    for(i=1;i<6;i++)
    {
        gotoprint(x,y+i,0,0,0);
        gotoprint(x+20,y+i,0,0,0);
        gotoprint(x+24,y+i,0,0,0);
        gotoprint(x+44,y+i,0,0,0);
    }
    for(i=0;i<22;i+=2)
    {
        gotoprint(x+i,y,0,0,0);
        gotoprint(x+i+24,y,0,0,0);
        gotoprint(x+i,y+6,0,0,0);
        gotoprint(x+i+24,y+6,0,0,0);
    }
    gotoxy(x+3,y+3);
    printf("YOUR SCORE: %d",score);
    if(swit==0)
        crebordwn2(x,y+8,upnuma,updena,ua,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
        else if(swit==1)
        crebordwn1(x,y+8,upnuma,updena,ua,upnumb,updenb,ub,uc,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
    else
        printf("Swit given to crebor is wrong !");
}


void crebor(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//创建带倒计时和分数的带总边框的界面整体，score为得分，time为时间，swit=0时显示计算结果界面，swit=1时显示两数计算界面，upnuma(b)/updena(b)为参加运算两数，numa(bcd)/dena(bcd)为底部题目框中的数字,用于mainbody
{
    showkey();
    crebor1(x+2,y+1,score,swit,upnuma,updena,ua,upnumb,updenb,ub,uc,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
    int i;
    for(i=0;i<25;i++)
    {
        gotoxy(x,y+i);
        printf("■");
        gotoxy(x+48,y+i);
        printf("■");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(x+i,y);
        printf("■");
        gotoxy(x+i,y+24);
        printf("■");
    }
}


void crebor_forwel(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//创建带倒计时和分数的带总边框的界面整体，score为得分，time为时间，swit=0时显示计算结果界面，swit=1时显示两数计算界面，upnuma(b)/updena(b)为参加运算两数，numa(bcd)/dena(bcd)为底部题目框中的数字,用于welcome
{
    crebor1(x+2,y+1,score,swit,upnuma,updena,ua,upnumb,updenb,ub,uc,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
    int i;
    for(i=0;i<25;i++)
    {
        gotoxy(x,y+i);
        printf("■");
        gotoxy(x+48,y+i);
        printf("■");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(x+i,y);
        printf("■");
        gotoxy(x+i,y+24);
        printf("■");
    }
}


void clearbor(int x,int y,int ul,int um,int ur,int da,int db,int dc,int dd)//把方框内容清空，1为清空
{
    if(ul==1)
    {
        gotoxy(x+12,y+10);
        printf("      ");
        gotoxy(x+12,y+11);
        printf("      ");
        gotoxy(x+12,y+12);
        printf("      ");
    }
    if(ur==1)
    {
        gotoxy(x+32,y+10);
        printf("      ");
        gotoxy(x+32,y+11);
        printf("      ");
        gotoxy(x+32,y+12);
        printf("     ");
    }
    if(um==1)
    {
        gotoxy(x+24,y+11);
        printf(" ");
    }
    if(da==1)
    {
        gotoxy(x+4,y+20);
        printf("      ");
        gotoxy(x+4,y+21);
        printf("      ");
        gotoxy(x+4,y+22);
        printf("     ");
    }
    if(db==1)
    {
        gotoxy(x+16,y+20);
        printf("      ");
        gotoxy(x+16,y+21);
        printf("      ");
        gotoxy(x+16,y+22);
        printf("     ");
    }
    if(dc==1)
    {
        gotoxy(x+28,y+20);
        printf("      ");
        gotoxy(x+28,y+21);
        printf("      ");
        gotoxy(x+28,y+22);
        printf("      ");
    }
    if(dd==1)
    {
        gotoxy(x+40,y+20);
        printf("      ");
        gotoxy(x+40,y+21);
        printf("      ");
        gotoxy(x+40,y+22);
        printf("      ");
    }
}


void give(int give[],int given[])//把give[]的值一一赋给given[]
{
    int i;
    for(i=0;i<3;i++)
    {
        given[i]=give[i];
    }
}


int findvoid(int a,int b,int c,int d)//找出最靠左的空位，若全空，返回-1
{
    int i;
    if(a==2&&b==2&&c==2&&d==2)
        return -1;
    if(a==2)
        return 0;
    else if(b==2)
        return 1;
    else if(c==2)
        return 2;
    else
        return 3;
}


int redu(int a[])//用于约分a[0]和a[1],若约得尽，将a[2]置为0,否则置1
{
    int i;
    int a0=0,a1=0;
    if(a[0]==0)
    {
        a[2]=0;
        return 1;
    }
    if(a[0]<0&&a[1]<0)
    {
        a[0]=-a[0];
        a[1]=-a[1];
    }
    else
    {
        if(a[0]<0)
        {
           a0=1;
           a[0]=-a[0];
        }
        if(a[1]<0)
        {
           a1=1;
           a[1]=-a[1];
        }
    }
    int smallest;
    if(a[0]<a[1])
        smallest=a[0];
    else
        smallest=a[1];
    for(i=smallest;i>0;i--)
    {
        if(a[0]%i==0&&a[1]%i==0)
        {
             a[0]=a[0]/i;
            a[1]=a[1]/i;
            if(a[1]==1)
                a[2]=0;
            else
                a[2]=1;
            if(a0==1||a1==1)
                a[0]=-a[0];
            return 1;
        }
    }
    if(a0==1)
        a[0]=-a[0];
    if(a1==1)
        a[1]=-a[1];
    a[2]=1;
    return 0;
}


void mainBody()
{
    int m[800];
    int i;
    int start=0;//指示计时开始了没，0为没开始，1为开始
    int score;//分数
    int skip;//用于指示是否跳过，0没跳，1跳
    int eras;//用于指示是否擦除，同上
    int a[3],b[3],c[3],d[3];//题目的四个数。以a[]为例：a[0]为num,a[1]为den,a[2]=0，表示a为整数；a[2]=1,表示a为分数；a[2]=2表示该位置空着
    int le[3],ri[3],mi;//指示参加运算的两个数以及运算符是否就位,用法同上,特殊的mi用法：mi=0,1,2,3,代表+-*/；mi=-3代表空框
    int succeed;//指示最后有没有算出24，0为算错；1为算对；为2代表还没算出来
    char read;//读取键盘上的按键的值
    crerand(m);
    Xofboard=0;
    Yofboard=0;
    stop=0;
   for(i=0,score=0,undonum=0;i<200,stop==0;i++)
   {
       undonum++;
       donenum=score;
       skip=0;
       eras=0;
       succeed=2;
       a[0]=m[4*i+0];b[0]=m[4*i+1];c[0]=m[4*i+2];d[0]=m[4*i+3];
       undone[(undonum-1)*4]=a[0];
       undone[(undonum-1)*4+1]=b[0];
       undone[(undonum-1)*4+2]=c[0];
       undone[(undonum-1)*4+3]=d[0];
       a[1]=1;b[1]=1;c[1]=1;d[1]=1;
       a[2]=0;b[2]=0;c[2]=0;d[2]=0;
       le[2]=2;mi=-3;ri[2]=2;//空框
       system("cls");
       if(i==0&&start==0)
       {
           _beginthread(coudon,0,100);
           //_beginthread(showkey,0,NULL);
           Sleep(1);//线程初始化时间
           start=1;
       }
       crebor(Xofboard,Yofboard,score,1,0,0,2,0,0,2,-3,a[0],1,0,b[0],1,0,c[0],1,0,d[0],1,0);//显示题目
       while(stop==0)
       {
           read=getch();
           if(read==key[0])//选A
           {
                if(le[2]==2&&a[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(a,le);
                   a[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&a[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(a,ri);
                   a[2]=2;
               }
           }
            else if(read==key[1])//选B
           {
                if(le[2]==2&&b[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(b,le);
                   b[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&b[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(b,ri);
                   b[2]=2;
               }
           }
            else if(read==key[2])//选C
           {
                  if(le[2]==2&&c[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(c,le);
                   c[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&c[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(c,ri);
                   c[2]=2;
               }
           }
          else if(read==key[3])//选D
           {
                  if(le[2]==2&&d[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(d,le);
                   d[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&d[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(d,ri);
                   d[2]=2;
               }
           }
            else if(read==key[4])//选+
           {
               mi=0;
           }
           else if(read==key[5])//选-
           {
               mi=1;
           }
            else if(read==key[6])//选*
           {
               mi=2;
           }
           else if(read==key[7])//选/
           {
               mi=3;
           }
            else if(read==key[8])//erase
           {
               eras=1;
           }
            else if(read==key[9])//skip
           {
               skip=1;
           }
           else if(read==key[10]&&le[2]!=2)//左归位
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a有空位
                        give(le,a);
                        le[2]=2;
                        break;
                    case 1://b有空位
                        give(le,b);
                        le[2]=2;
                        break;
                    case 2://c有空位
                        give(le,c);
                        le[2]=2;
                        break;
                    case 3://d有空位
                        give(le,d);
                        le[2]=2;
                        break;
                }
           }
           else if(read==key[11]&&ri[2]!=2)//右归位
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a有空位
                        give(ri,a);
                        ri[2]=2;
                        break;
                    case 1://b有空位
                        give(ri,b);
                        ri[2]=2;
                        break;
                    case 2://c有空位
                        give(ri,c);
                        ri[2]=2;
                        break;
                    case 3://d有空位
                        give(ri,d);
                        ri[2]=2;
                        break;
                }
           }
           else//按其它键就单纯刷新
           {
               system("cls");
               crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);
           }
           if((skip+eras+stop)!=0)
            break;
           system("cls");
           crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//先显示运算界面
           if(le[2]!=2&&ri[2]!=2&&mi!=-3)//若两运算数和运算符均就位，开始运算
           {

               switch(mi)
               {
               case 0://加
                  le[0]=le[0]*ri[1]+ri[0]*le[1];
                  le[1]=le[1]*ri[1];//结果存于le中
                  redu(le);
                  break;
               case 1://减
                  le[0]=le[0]*ri[1]-ri[0]*le[1];
                  le[1]=le[1]*ri[1];//结果存于le中
                  redu(le);
                  break;
               case 2://乘
                  le[0]=le[0]*ri[0];
                  le[1]=le[1]*ri[1];
                  redu(le);
                  break;
               case 3://除
                   if(ri[0]==0)
                   {
                       //_beginthread(errormes,0,1);
                       switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a有空位
                                give(le,a);
                                le[2]=2;
                                break;
                            case 1://b有空位
                                give(le,b);
                                le[2]=2;
                                break;
                            case 2://c有空位
                                give(le,c);
                                le[2]=2;
                                break;
                            case 3://d有空位
                                give(le,d);
                                le[2]=2;
                                break;
                        }
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a有空位
                                give(ri,a);
                                ri[2]=2;
                                break;
                            case 1://b有空位
                                give(ri,b);
                                ri[2]=2;
                                break;
                            case 2://c有空位
                                give(ri,c);
                                ri[2]=2;
                                break;
                            case 3://d有空位
                                give(ri,d);
                                ri[2]=2;
                                break;
                        }
                   }
                   else
                   {
                       le[0]=le[0]*ri[1];
                       le[1]=le[1]*ri[0];
                       redu(le);
                   }
                   break;
            }
            mi=-3;
            system("cls");
            crebor(Xofboard,Yofboard,score,0,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//显示计算结果
            if(stop==1)
                break;
            Sleep(anim);
            switch(findvoid(a[2],b[2],c[2],d[2]))//根据计算结果存储数据
            {
                case -1://四数已全部计算完毕
                    if(le[2]==0&&le[0]==24)
                    {
                        succeed=1;
                        undonum=undonum-1;
                    }
                    else
                        succeed=0;
                    break;
                case 0://a有空位
                    give(le,a);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 1://b有空位
                    give(le,b);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 2://c有空位
                    give(le,c);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 3://d有空位
                    give(le,d);
                    le[2]=2;
                    ri[2]=2;
                    break;
            }
            system("cls");
            crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//显示存储结果
           }
           if(succeed!=2||stop==1)
            break;
      }
      if(stop==1)
      {
          system("cls");
          break;
      }
      if(succeed==0)
      {
          i=i-1;
          //_beginthread(errormes,0,0);
      }
      if(succeed==1)
      {
          score=score+1;
          donenum=score;
      }
      if(skip==1)
        skipdly();
      if(eras==1)
      {
          i--;
          undonum--;
      }
   }
   system("cls");
}


void can_or_not(int m[],int can[])
{
    int i;
    for(i=0;i<200;i++)
    {
        can[i]=judg(m[4*i],m[4*i+1],m[4*i+2],m[4*i+3]);
    }
}


void mainBody_notsure()
{
    int m[800];
    int can[200];//指示这两百题那些有解，那些没解;can[n]=0代表第n题没解；can[n]=1代表第n题有解
    int i;
    int start=0;//指示计时开始了没，0为没开始，1为开始
    int score;//分数
    int skip;//用于指示是否跳过，0没跳，1跳
    int eras;//用于指示是否擦除，同上
    int a[3],b[3],c[3],d[3];//题目的四个数。以a[]为例：a[0]为num,a[1]为den,a[2]=0，表示a为整数；a[2]=1,表示a为分数；a[2]=2表示该位置空着
    int le[3],ri[3],mi;//指示参加运算的两个数以及运算符是否就位,用法同上,特殊的mi用法：mi=0,1,2,3,代表+-*/；mi=-3代表空框
    int succeed;//指示最后有没有算出24，0为算错；1为算对；为2代表还没算出来
    char read;//读取键盘上的按键的值
    crerand_notsure(m);
    can_or_not(m,can);
    Xofboard=0;
    Yofboard=0;
    stop=0;
   for(i=0,score=0,undonum=0;i<200,stop==0;i++)
   {
       undonum++;
       donenum=score;
       skip=0;
       eras=0;
       succeed=2;
       a[0]=m[4*i+0];b[0]=m[4*i+1];c[0]=m[4*i+2];d[0]=m[4*i+3];
       undone[(undonum-1)*4]=a[0];
       undone[(undonum-1)*4+1]=b[0];
       undone[(undonum-1)*4+2]=c[0];
       undone[(undonum-1)*4+3]=d[0];
       a[1]=1;b[1]=1;c[1]=1;d[1]=1;
       a[2]=0;b[2]=0;c[2]=0;d[2]=0;
       le[2]=2;mi=-3;ri[2]=2;//空框
       system("cls");
       if(i==0&&start==0)
       {
           _beginthread(coudon,0,100);
           //_beginthread(showkey,0,NULL);
           Sleep(1);//线程初始化时间
           start=1;
       }
       crebor(Xofboard,Yofboard,score,1,0,0,2,0,0,2,-3,a[0],1,0,b[0],1,0,c[0],1,0,d[0],1,0);//显示题目
       while(stop==0)
       {
           read=getch();
           if(read==key[0])//选A
           {
                if(le[2]==2&&a[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(a,le);
                   a[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&a[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(a,ri);
                   a[2]=2;
               }
           }
            else if(read==key[1])//选B
           {
                if(le[2]==2&&b[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(b,le);
                   b[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&b[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(b,ri);
                   b[2]=2;
               }
           }
            else if(read==key[2])//选C
           {
                  if(le[2]==2&&c[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(c,le);
                   c[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&c[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(c,ri);
                   c[2]=2;
               }
           }
          else if(read==key[3])//选D
           {
                  if(le[2]==2&&d[2]!=2)//左数未就位，选择的数赋给左位
               {
                   give(d,le);
                   d[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&d[2]!=2)//左数就位，选择的数赋给给右位
               {
                   give(d,ri);
                   d[2]=2;
               }
           }
            else if(read==key[4])//选+
           {
               mi=0;
           }
           else if(read==key[5])//选-
           {
               mi=1;
           }
            else if(read==key[6])//选*
           {
               mi=2;
           }
           else if(read==key[7])//选/
           {
               mi=3;
           }
            else if(read==key[8])//erase
           {
               eras=1;
           }
            else if(read==key[9])//skip
           {
               skip=1;
           }
           else if(read==key[10]&&le[2]!=2)//左归位
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a有空位
                        give(le,a);
                        le[2]=2;
                        break;
                    case 1://b有空位
                        give(le,b);
                        le[2]=2;
                        break;
                    case 2://c有空位
                        give(le,c);
                        le[2]=2;
                        break;
                    case 3://d有空位
                        give(le,d);
                        le[2]=2;
                        break;
                }
           }
           else if(read==key[11]&&ri[2]!=2)//右归位
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a有空位
                        give(ri,a);
                        ri[2]=2;
                        break;
                    case 1://b有空位
                        give(ri,b);
                        ri[2]=2;
                        break;
                    case 2://c有空位
                        give(ri,c);
                        ri[2]=2;
                        break;
                    case 3://d有空位
                        give(ri,d);
                        ri[2]=2;
                        break;
                }
           }
           else//按其它键就单纯刷新
           {
               system("cls");
               crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);
           }
           if((skip+eras+stop)!=0)
            break;
           system("cls");
           crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//先显示运算界面
           if(le[2]!=2&&ri[2]!=2&&mi!=-3)//若两运算数和运算符均就位，开始运算
           {

               switch(mi)
               {
               case 0://加
                  le[0]=le[0]*ri[1]+ri[0]*le[1];
                  le[1]=le[1]*ri[1];//结果存于le中
                  redu(le);
                  break;
               case 1://减
                  le[0]=le[0]*ri[1]-ri[0]*le[1];
                  le[1]=le[1]*ri[1];//结果存于le中
                  redu(le);
                  break;
               case 2://乘
                  le[0]=le[0]*ri[0];
                  le[1]=le[1]*ri[1];
                  redu(le);
                  break;
               case 3://除
                   if(ri[0]==0)
                   {
                        //_beginthread(errormes,0,1);
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a有空位
                                give(le,a);
                                le[2]=2;
                                break;
                            case 1://b有空位
                                give(le,b);
                                le[2]=2;
                                break;
                            case 2://c有空位
                                give(le,c);
                                le[2]=2;
                                break;
                            case 3://d有空位
                                give(le,d);
                                le[2]=2;
                                break;
                        }
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a有空位
                                give(ri,a);
                                ri[2]=2;
                                break;
                            case 1://b有空位
                                give(ri,b);
                                ri[2]=2;
                                break;
                            case 2://c有空位
                                give(ri,c);
                                ri[2]=2;
                                break;
                            case 3://d有空位
                                give(ri,d);
                                ri[2]=2;
                                break;
                        }
                   }
                   else
                   {
                       le[0]=le[0]*ri[1];
                       le[1]=le[1]*ri[0];
                       redu(le);
                   }
                   break;
            }
            mi=-3;
            system("cls");
            crebor(Xofboard,Yofboard,score,0,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//显示计算结果
            if(stop==1)
                break;
            Sleep(anim);
            switch(findvoid(a[2],b[2],c[2],d[2]))//根据计算结果存储数据
            {
                case -1://四数已全部计算完毕
                    if(le[2]==0&&le[0]==24)
                    {
                        succeed=1;
                        undonum=undonum-1;
                    }
                    else
                        succeed=0;
                    break;
                case 0://a有空位
                    give(le,a);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 1://b有空位
                    give(le,b);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 2://c有空位
                    give(le,c);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 3://d有空位
                    give(le,d);
                    le[2]=2;
                    ri[2]=2;
                    break;
            }
            system("cls");
            crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//显示存储结果
           }
           if(succeed!=2||stop==1)
            break;
      }
      if(stop==1)
      {
          system("cls");
          break;
      }
      if(succeed==0)
      {
          i=i-1;
          //_beginthread(errormes,0,0);
      }
      if(succeed==1)
      {
          score=score+1;
          donenum=score;
      }
      if(skip==1)
      {
          if(can[i]==1)
            skipdly();
          else
          {
              score=score+1;
              undonum--;
              donenum=score;
          }
      }
      if(eras==1)
      {
          i--;
          undonum--;
      }
   }
   system("cls");
}


void Body(int sw)//sw为0时为扫雷模式；为其余值时为平坦模式
{
    if(sw==0)
        mainBody_notsure();
    else
        mainBody();
}

void Finish()
{
    int i;
	system("cls");
	gotoxy(15, 7);
	printf("/**********************************************/");
	gotoxy(15, 21);
	printf("/**********************************************/");
	gotoxy(26,11);
	printf("GAME   OVER   o(*￣▽￣*)o");
	gotoxy(31,13);
	printf("Your Score is %d", donenum);
	gotoxy(22, 17);
    printf("     按任意键查看未解题的解>>>>>");
	if(donenum<=10)
	{
	    gotoxy(22, 15);
	    printf("  你好菜呀 (   →  ω → ) 哈哈哈哈");
	}
	if(donenum>10&&donenum<=20)
    {
	    gotoxy(22, 15);
	    printf("还不错哦，继续努力o(  ∩ _ ∩  )o");
	}
	if(donenum>20&&donenum<=40)
    {
        gotoxy(22, 15);
	    printf("∑( ⊙ △ ⊙ ||)  你这就有点屌了啊");
    }
    if(donenum>40)
    {
        gotoxy(22, 15);
	    printf("。。。你是神仙吧(╯°Д°)╯︵ ┻━┻");
    }
    getch();
    system("cls");
    gotoxy(15, 7);
    printf("/**********************************************/");
    gotoxy(15, 17+(undonum-1)*2);
    printf("/**********************************************/");
	for(i=0;i<undonum;i++)
    {
        gotoxy(22,11+i*2);
        printf("%d %d %d %d 的解：",undone[4*i],undone[4*i+1],undone[4*i+2],undone[4*i+3]);
	    Prtsolv(40,11+i*2,undone[4*i],undone[4*i+1],undone[4*i+2],undone[4*i+3]);
    }
    gotoxy(30, 11+i*2);
    printf("按任意键继续>>>>>");
    getch();
}


void welcome()
{
	gotoxy(15,7);
	printf("/**********************************************/");
	gotoxy(15, 19);
	printf("/**********************************************/");
	gotoxy(24, 11);
	printf("WELCOME TO THE GAME OF 24 POINT");
	gotoxy(19, 13);
	printf("本游戏中提到的“任意键”，仅指字母或空格");
	gotoxy(20, 15);
	printf("请将输入法切至英文，按任意键继续>>>>>");
	getch();
    while(1)
    {
        system("cls");
        gotoxy(15,7);
        printf("/********************************************************************************/");
        gotoxy(15, 27);
        printf("/********************************************************************************/");
        gotoxy(24, 11);
        printf("                            游戏规则");
        gotoxy(24, 13);
        printf("          解题模式：                       求道模式：");
        gotoxy(24, 15);
        printf("使用显示出来的A、B、C、D四个数        输入A、B、C、D四数");
        gotoxy(24, 17);
        printf("      通过四则运算算出24             系统会自动显示出解答");
        gotoxy(24, 19);
        printf("  算出一题得一分，限时100秒         若无解，系统显示“无解”");
        gotoxy(23, 23);
        printf("      按“q”键进入求道模式；按其余任意键进入解题模式>>>>>");
        char q;
        char qu;
        q=getch();
        if(q=='q'||q=='Q')
        {
            while(1)
            {
                system("cls");
                solvque();
                qu=getch();
                if(qu!='q'&&qu!='Q')
                    break;
            }
        }
        else
            break;
    }
    system("cls");
    gotoxy(15,7);
	printf("/**********************************************/");
	gotoxy(15, 21);
	printf("/**********************************************/");
	gotoxy(30,11);
	printf("解题模式分为两种：");
	gotoxy(16, 13);
	printf("平坦模式：所出的题保证有解，且四数从小到大排列");
	gotoxy(16, 15);
	printf("扫雷模式：不保证题目均有解，且不一定小到大排列");
	gotoxy(30, 17);
	printf("按任意键继续>>>>>");
	getch();
	while(1)
    {
        system("cls");
	    crebor_forwel(0,0,0,1,0,0,2,0,0,2,-3,0,0,2,0,0,2,0,0,2,0,0,2);
	    gotoxy(6,21);
     	printf("A");
	    gotoxy(18,21);
	    printf("B");
	    gotoxy(30,21);
	    printf("C");
	    gotoxy(42,21);
	    printf("D");
	    gotoxy(54,0);
	    printf("键位设置（尽量用小写字母或空格作键位，不要使用重复键位）：");
	    gotoxy(5,18);
	    printf("↓");
	    gotoxy(54,2);
	    printf("请选择A数的键位：");
	    key[0]=getch();
	    gotoxy(71,2);
	    if(key[0]==' ')
            printf("空格");
        else
	        printf("%c",key[0]);
        gotoxy(5,18);
	    printf(" ");
	    gotoxy(17,18);
	    printf("↓");
	    while(1)
        {
            gotoxy(54,4);
	        printf("请选择B数的键位：");
	        key[1]=getch();
            if(judg_same(key,1)==0)
                break;
            else
            {
                gotoxy(54,4);
                printf("                                  ");
                gotoxy(54,4);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,4);
                printf("                    ");
            }
        }
	    gotoxy(71,4);
	    if(key[1]==' ')
            printf("空格");
        else
	        printf("%c",key[1]);
	    gotoxy(17,18);
	    printf(" ");
	    gotoxy(29,18);
	    printf("↓");
	    while(1)
        {
            gotoxy(54,6);
	        printf("请选择C数的键位：");
	        key[2]=getch();
            if(judg_same(key,2)==0)
                break;
            else
            {
                gotoxy(54,6);
                printf("                                  ");
                gotoxy(54,6);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,6);
                printf("                    ");
            }
        }
	    gotoxy(71,6);
	    if(key[2]==' ')
            printf("空格");
        else
	        printf("%c",key[2]);
	    gotoxy(29,18);
	    printf(" ");
	    gotoxy(41,18);
	    printf("↓");
	    while(1)
        {
            gotoxy(54,8);
	        printf("请选择D数的键位：");
            key[3]=getch();
            if(judg_same(key,3)==0)
                break;
            else
            {
                gotoxy(54,8);
                printf("                                  ");
                gotoxy(54,8);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,8);
                printf("                    ");
             }
        }
	    gotoxy(71,8);
	    if(key[3]==' ')
            printf("空格");
        else
	        printf("%c",key[3]);
	    gotoxy(41,18);
	    printf(" ");
	    gotoxy(11,18);
	    printf("↑");
	   while(1)
       {
            gotoxy(76,2);
	        printf("请选择加号的键位：");
	        key[4]=getch();
            if(judg_same(key,4)==0)
                break;
            else
            {
                gotoxy(76,2);
                printf("                                  ");
                gotoxy(76,2);
                printf("键位重复！");
                Sleep(800);
                gotoxy(76,2);
                printf("                    ");
            }
        }
	    gotoxy(94,2);
	    if(key[4]==' ')
            printf("空格");
        else
            printf("%c",key[4]);
        gotoxy(11,18);
	    printf(" ");
	    gotoxy(19,18);
	    printf("↑");
	    while(1)
        {
            gotoxy(76,4);
	        printf("请选择减号的键位：");
	        key[5]=getch();
            if(judg_same(key,5)==0)
                break;
            else
            {
                gotoxy(76,4);
                printf("                                  ");
                gotoxy(76,4);
                printf("键位重复！");
                Sleep(800);
                gotoxy(76,4);
                printf("                    ");
            }
        }
	    gotoxy(94,4);
        if(key[5]==' ')
            printf("空格");
        else
	        printf("%c",key[5]);
	    gotoxy(19,18);
	    printf(" ");
	    gotoxy(27,18);
	    printf("↑");
	    while(1)
        {
            gotoxy(76,6);
	        printf("请选择乘号的键位：");
	        key[6]=getch();
            if(judg_same(key,6)==0)
                break;
            else
            {
                gotoxy(76,6);
                printf("                                  ");
                gotoxy(76,6);
                printf("键位重复！");
                Sleep(800);
                gotoxy(76,6);
                printf("                    ");
           }
        }
	    gotoxy(94,6);
     	if(key[6]==' ')
            printf("空格");
        else
	        printf("%c",key[6]);
	    gotoxy(27,18);
	    printf(" ");
	    gotoxy(35,18);
	    printf("↑");
	    while(1)
        {
            gotoxy(76,8);
	        printf("请选择除号的键位：");
	        key[7]=getch();
            if(judg_same(key,7)==0)
                break;
            else
            {
                gotoxy(76,8);
                printf("                                  ");
                gotoxy(76,8);
                printf("键位重复！");
                Sleep(800);
                gotoxy(76,8);
                printf("                    ");
            }
        }
	    gotoxy(94,8);
	    if(key[7]==' ')
            printf("空格");
        else
	        printf("%c",key[7]);
	    gotoxy(54,10);
	    printf("*ERASE键用途：当发现做错时，可使用该键复位，回到初始状态");
	    gotoxy(35,18);
	    printf(" ");
	    gotoxy(3,18);
	    printf("↑");
	    while(1)
        {
            gotoxy(54,12);
	        printf("请选择ERASE键的键位：");
	        key[8]=getch();
            if(judg_same(key,8)==0)
                break;
            else
            {
                gotoxy(54,12);
                printf("                                  ");
                gotoxy(54,12);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,12);
                printf("                    ");
            }
        }
	    gotoxy(75,12);
	    if(key[8]==' ')
            printf("空格");
        else
	        printf("%c",key[8]);
        gotoxy(54,14);
	    printf("*SKIP键用途：平坦模式下牺牲3秒跳到下一题");
	    gotoxy(54,16);
	    printf("扫雷模式下遇到无解题按该键可直接跳下一题");
	    gotoxy(3,18);
	    printf(" ");
	    gotoxy(43,18);
	    printf("↑");
	    while(1)
        {
            gotoxy(54,18);
	        printf("请选择SKIP键的键位：");
	        key[9]=getch();
            if(judg_same(key,9)==0)
                break;
            else
            {
                gotoxy(54,18);
                printf("                                  ");
                gotoxy(54,18);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,18);
                printf("                    ");
            }
        }
	    gotoxy(74,18);
	    if(key[9]==' ')
            printf("空格");
        else
	        printf("%c",key[9]);
        gotoxy(43,18);
	    printf(" ");
        gotoxy(13,14);
	    printf("↑");
	    while(1)
        {
            gotoxy(54,20);
	        printf("请选择左归位键键位（可将参与运算的左数归位重选）：");
	        key[10]=getch();
            if(judg_same(key,10)==0)
                break;
            else
            {
                gotoxy(54,20);
                printf("                                                   ");
                gotoxy(54,20);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,20);
                printf("                    ");
            }
        }
         gotoxy(104,20);
	    if(key[10]==' ')
            printf("空格");
        else
	        printf("%c",key[10]);
        gotoxy(13,14);
	    printf(" ");
	    gotoxy(33,14);
	    printf("↑");
        while(1)
        {
            gotoxy(54,22);
	        printf("请选择右归位键键位（可将参与运算的右数归位重选）：");
	        key[11]=getch();
            if(judg_same(key,11)==0)
                break;
            else
            {
                gotoxy(54,22);
                printf("                                                   ");
                gotoxy(54,22);
                printf("键位重复！");
                Sleep(800);
                gotoxy(54,22);
                printf("                    ");
            }
        }
         gotoxy(104,22);
	    if(key[11]==' ')
            printf("空格");
        else
	        printf("%c",key[11]);
        gotoxy(33,14);
	    printf(" ");
	    gotoxy(54,24);
        printf("对自己的键位选择是否满意？若不满意，按“n”键重选");
        gotoxy(54,26);
        printf("满意按“s”键进入平坦模式或其他任意键进入扫雷模式");
        ok=getch();
        if(ok!='n'&&ok!='N')
            break;
    }
}


int judg_same(char m[],int n)//判断从m[0]到m[n-1]里有没有跟m[n]重复的元素，若有，返回1；没有返回0
{
    int i;
    for(i=0;i<n;i++)
    {
        if(m[i]==m[n])
            return 1;
    }
    return 0;
}


int judg4_forprint(int a,int b,int c,int d)//判断a,b,c,d是否能通过不加括号的四则运算算出24
{
    int e,f,g,h,i,k,l,n,o;
    int m[4];
    solvment[0]=-1;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//四取二进行四则运算
            {
                m[0]=a;
                m[1]=b;
                m[2]=c;
                m[3]=d;
                solvment[1]=m[e];//记录第一次参与运算的两数
                solvment[2]=m[f];
                solvment[5]=g;//记录第一次运算的运算符
                int num=m[e],den=1;
                ope(&num,&den,m[f],g);
                del(&m,e);
                del(&m,f-1);
                k=m[0];
                l=m[1];
                n=num;
                o=den;
                for(h=0;h<2;h++)//二取一与num/den进行四则运算
                {
                    for(i=0;i<6;i++)
                    {
                        m[0]=k;
                        m[1]=l;
                        num=n;
                        den=o;
                        solvment[6]=i;//第二次运算符
                        solvment[3]=m[h];
                        ope(&num,&den,m[h],i);
                        del(&m,h);
                        solvment[4]=m[0];
                        solvment[7]=judg2(num,den,m[0]);//最后判断
                        if(solvment[7]==6)
                            continue;
                        else
                        {
                            solvment[0]=0;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


int judg4_special_forprint(int a,int b,int c,int d)//判断a,b,c,d是否能通过加括号的四则运算算出24
{
    int m[4];
    int e,f,g,h,i,j,k,l,n,o;
    solvment[0]=-1;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//四取二进行四则运算
            {
                m[0]=a;
                m[1]=b;
                m[2]=c;
                m[3]=d;
                int num=m[e],den=1;
                solvment[1]=m[e];
                solvment[2]=m[f];
                solvment[5]=g;
                ope(&num,&den,m[f],g);
                del(&m,e);
                del(&m,f-1);
                i=m[0];
                j=m[1];
                for(h=0;h<6;h++)
                {
                    int num1=m[0],den1=1;
                    solvment[3]=m[0];
                    solvment[4]=m[1];
                    solvment[6]=h;
                    ope(&num1,&den1,m[1],h);
                    solvment[7]=judg2_special(num,den,num1,den1);
                    if(solvment[7]==6)
                        continue;
                    else
                    {
                        solvment[0]=1;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


void exso_to_op()
{
    int i;
    for(i=5;i<8;i++)
    {
        switch(solvment[i])
        {
            case 0:
                opemes[i-5]=4;
                break;
            case 1:
            case 2:
                opemes[i-5]=5;
                break;
            case 3:
                opemes[i-5]=6;
                break;
            case 4:
            case 5:
                opemes[i-5]=7;
                break;
        }
    }
}


void Prtsolv2(int x,int y)//打印两数(solvment[1&2])运算
{
    gotoxy(x,y);
    printf("(     )");
    gotoprint(x+3,y,0,0,opemes[0]);
    if(solvment[5]==2||solvment[5]==5)
    {
        gotoxy(x+1,y);
        printf("%d",solvment[2]);
        gotoxy(x+4,y);
        printf("%d",solvment[1]);
    }
    else
    {
        gotoxy(x+1,y);
        printf("%d",solvment[1]);
        gotoxy(x+4,y);
        printf("%d",solvment[2]);
    }
}


void Prtsolv2_special(int x,int y)//打印两数(solvment[3&4])运算
{
    gotoxy(x,y);
    printf("(     )");
    gotoprint(x+3,y,0,0,opemes[1]);
    if(solvment[6]==2||solvment[6]==5)
    {
        gotoxy(x+1,y);
        printf("%d",solvment[4]);
        gotoxy(x+4,y);
        printf("%d",solvment[3]);
    }
    else
    {
        gotoxy(x+1,y);
        printf("%d",solvment[3]);
        gotoxy(x+4,y);
        printf("%d",solvment[4]);
    }
}


void Prtsolv3(int x,int y)//打印三数运算
{
    gotoxy(x,y);
    printf("(          )");
    if(solvment[6]==2||solvment[6]==5)
    {
        Prtsolv2(x+4,y);
        gotoxy(x+1,y);
        printf("%d",solvment[3]);
        gotoprint(x+3,y,0,0,opemes[1]);
    }
    else
    {
        Prtsolv2(x+1,y);
        gotoxy(x+9,y);
        printf("%d",solvment[3]);
        gotoprint(x+8,y,0,0,opemes[1]);
    }
}


void Prtsolv4(int x,int y)//打印按顺序的四数运算
{
    if(solvment[7]==2||solvment[7]==5)
    {
        Prtsolv3(x+3,y);
        gotoxy(x,y);
        printf("%d",solvment[4]);
        gotoprint(x+2,y,0,0,opemes[2]);
    }
    else
    {
        Prtsolv3(x,y);
        gotoxy(x+13,y);
        printf("%d",solvment[4]);
        gotoprint(x+12,y,0,0,opemes[2]);
    }
}


void Prtsolv4_special(int x,int y)//打印按顺序的四数运算
{
    gotoprint(x+7,y,0,0,opemes[2]);
    if(solvment[7]==2||solvment[7]==5)
    {
        Prtsolv2_special(x,y);
        Prtsolv2(x+8,y);
    }
    else
    {
        Prtsolv2(x,y);
        Prtsolv2_special(x+8,y);
    }
}


int Prtsolv(int x,int y,int a,int b,int c,int d)//在（x,y）处打印出a,b,c,d的解法
{
    int result;
    result=judg4_forprint(a,b,c,d);
    exso_to_op();
    if(result==1)
    {
        Prtsolv4(x,y);
        return 1;
    }
    else
    {
        result=judg4_special_forprint(a,b,c,d);
        exso_to_op();
        if(result==1)
        {
            Prtsolv4_special(x,y);
            return 1;
        }
        else
        {
            gotoxy(x,y);
            printf("  %d %d %d %d  无解",a,b,c,d);
            return 0;
        }
    }
}


void solvque()//用于welcome里的求道模式
{
    int i;
    char numb[2];//用于存储两位数
    char nu[1];//用于判断个位是否数字
    int flag;//和nu配合
    int a,b,c,d;
    gotoxy(15,7);
	printf("/**********************************************/");
	gotoxy(15, 27);
	printf("/**********************************************/");
	gotoxy(15,11);
	printf("注意事项：四数必须皆为大于等于1且小于等于99的数字");
	while(1)
    {
        gotoxy(15,13);
        printf("                                                                                ");
        gotoxy(15,13);
        printf("                 请输入A数：");
        numb[0]=getch();
        printf("%c",numb[0]);
        numb[1]=getch();
        printf("%c",numb[1]);
        nu[0]=numb[1];
        a=atoi(numb);
        flag=atoi(nu);
       if(a*flag==0)
        {
            gotoxy(15,13);
            printf("                                                                                ");
            gotoxy(15,13);
            printf("你输入的是啥玩意儿（ ←_← || ）求求你看看注意事项啊啊啊");
            Sleep(1500);
        }
        else
        {
            break;
        }
    }
    while(1)
    {
        gotoxy(15,15);
        printf("                                                                                ");
        gotoxy(15,15);
        printf("                 请输入B数：");
        numb[0]=getch();
        printf("%c",numb[0]);
        numb[1]=getch();
        printf("%c",numb[1]);
        nu[0]=numb[1];
        b=atoi(numb);
        flag=atoi(nu);
       if(b*flag==0)
        {
            gotoxy(15,15);
            printf("                                                                                ");
            gotoxy(15,15);
            printf("你输入的是啥玩意儿（ ←_← || ）求求你看看注意事项啊啊啊");
            Sleep(1500);
        }
        else
        {
            break;
        }
    }
   while(1)
    {
        gotoxy(15,17);
        printf("                                                                                ");
        gotoxy(15,17);
        printf("                 请输入C数：");
        numb[0]=getch();
        printf("%c",numb[0]);
        numb[1]=getch();
        printf("%c",numb[1]);
        nu[0]=numb[1];
        c=atoi(numb);
        flag=atoi(nu);
       if(c*flag==0)
        {
            gotoxy(15,17);
            printf("                                                                                ");
            gotoxy(15,17);
            printf("你输入的是啥玩意儿（ ←_← || ）求求你看看注意事项啊啊啊");
            Sleep(1500);
        }
        else
        {
            break;
        }
    }
  while(1)
    {
        gotoxy(15,19);
        printf("                                                                                ");
        gotoxy(15,19);
        printf("                 请输入D数：");
        numb[0]=getch();
        printf("%c",numb[0]);
        numb[1]=getch();
        printf("%c",numb[1]);
        nu[0]=numb[1];
        d=atoi(numb);
        flag=atoi(nu);
       if(d*flag==0)
        {
            gotoxy(15,19);
            printf("                                                                                ");
            gotoxy(15,19);
            printf("你输入的是啥玩意儿（ ←_← || ）求求你看看注意事项啊啊啊");
            Sleep(1500);
        }
        else
        {
            break;
        }
    }
    gotoxy(28,21);
    printf("解：");
    Prtsolv(32,21,a,b,c,d);
    gotoxy(16,23);
    printf("按“q”继续求道模式；按其余任意键返回主菜单>>>>>");
}


void main()
{
      system("color 0B");
      while(1)
      {
          char c;
          system("cls");
          welcome();
          skipdly();
          if(ok=='s'||ok=='S')
             Body(1);
          else
             Body(0);
          Finish();
          system("cls");
          gotoxy(15,7);
          printf("/**********************************************/");
          gotoxy(15, 19);
          printf("/**********************************************/");
          gotoxy(32, 11);
          printf("是否再来一局？");
          gotoxy(32, 13);
          printf("同意按“y”键");
          gotoxy(28, 15);
           printf("按其余任意键退出>>>>>");
           c=getch();
           if(c!='y'&&c!='Y')
               break;
      }
}
