#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <process.h>
#include<Windows.h>
#define TOTAL 1362//������
//��Ŵ�0��ʼ
int Xofboard=0,Yofboard=0;//��Ϸ����λ��
int anim=0;//�Ӿ�����ʱ��
int stop;//ָʾ100��ʱ�䵽��û��Ϊ0����û����Ϊ1������
int donenum;//����������
int undonum;//δ����������,���������ʾ����ⷨ
int undone[800];//��¼δ��������Ŀ
char key[12];//��¼��λ��Ϣ��key[0-3]ΪABCD������key[4-7]Ϊ�Ӽ��˳���key[8]ΪERASE����key[9]ΪSKIP����key[10]Ϊ���λ��key[11]Ϊ�ҹ�λ
int solvment[8];//s[0]:���ֽⷨ��Ϊ0Ϊ�������Ž⣨judg4����Ϊ1Ϊ�����Ž�(judg4_special)��s[1-4]:���������s[5-7]:1-3�ν����˺������㣬���judg����
int opemes[3];//��¼����������Ϣ����s[5-7]��֮ͬ�����ڼ��ͳ���opemesû����˳�򣻶�s[5-7]������˳�򣻶�����exso_to_op()ת��
char ok;//��ʼ��־��ΪY�Ϳ�ʼ������welcome
int ope(int *num,int *den,int input,int c)//�����������㣻cΪѡ���������ѡ���������������Ⱥ��ϵ��a=num/den��b=inputΪ�μ����������
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


int ope_special(int *num,int *den,int num1,int den1,int c)//��a��b��Ϊ����num/denʱ���������㣨aΪnum/den;bΪnum2/den2)��c����ѡ��ͬ�������ϵ
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


int judg2(int num,int den,int input)//�ж�a=num/den��b=input�Ƿ����ͨ�������������24,����judge4a
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


int judg2_special(int num,int den,int num1,int den1)//�ж�a=num/den��b=num1/den1�Ƿ����ͨ�������������24,����judge4_special
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


int judg4(int a,int b,int c,int d)//�ж�a,b,c,d�Ƿ���ͨ���������ŵ������������24
{
    int e,f,g,h,i,j,k,l,n,o;
    int m[4];
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//��ȡ��������������
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
                for(h=0;h<2;h++)//��ȡһ��num/den������������
                {
                    for(i=0;i<6;i++)
                    {
                        m[0]=k;
                        m[1]=l;
                        num=n;
                        den=o;
                        ope(&num,&den,m[h],i);
                        del(&m,h);
                        j=judg2(num,den,m[0]);//����ж�
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


int judg4_special(int a,int b,int c,int d)//�ж�a,b,c,d�Ƿ���ͨ�������ŵ������������24
{
    int m[4];
    int e,f,g,h,i,j,k,l,n,o;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//��ȡ��������������
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


void del(int m[],int i)//����judg4��judg4_special����,ɾȥ�������Ѿ�������������
{
    int a;
    for(a=i;a<4;a++)
    {
        m[a]=m[a+1];
    }
}


int judg(int a,int b,int c,int d)//�ж�a,b,c,d�Ƿ���ͨ�������������24
{
    if(judg4(a,b,c,d)==0&&judg4_special(a,b,c,d)==0)
        return 0;
    else
        return 1;
}


void creat(int m[])//�������������24���4����ϣ���1362��
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


void crerand(int m[])//����200�������������a[200]�У������ٸ��������ΪҪ�����200������,�����ٵ����800��������m[800]��,������m[800]�������һ���н��
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


void crerand_notsure(int m[])//�������ٵ��⣬800��������m[800]�У���һ���н⣩
{
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<800;i++)
    {
        m[i]=rand()%13+1;
    }
}


void coudon(int time)//����ʱ��ʾ����һ��Ҫ���ڱ߿򴴽�֮��Ҫ�����̴߳�������_beginthread(coudon,0,100)ʵ��
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


void errormes(int i)//������ʱ��ʾ"ERROR ! Your method cannot get 24."��"ERROR ! 0 cannot be divisor."������1��(P.S.֮����ȡ1001������Ϊ�˲����ŵ���ʱ),Ҫ�����̴߳������� _beginthread(errormes,0,0)��_beginthread(errormes,0,1)ʵ��
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
        printf("��");
        gotoxy(Xofboard+48,Yofboard+i);
        printf("��");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(Xofboard+i,Yofboard);
        printf("��");
        gotoxy(Xofboard+i,Yofboard+24);
        printf("��");
    }
    gotoxy(Xofboard+22,Yofboard+9);
    printf("������");
    gotoxy(Xofboard+4+22,Yofboard+1+9);
    printf("��");
    gotoxy(Xofboard+22,Yofboard+2+9);
    printf("������");
    gotoxy(Xofboard+4+22,Yofboard+3+9);
    printf("��");
    gotoxy(Xofboard+22,Yofboard+4+9);
    printf("������");
    Sleep(1000);
    gotoxy(Xofboard+4+22,Yofboard+3+9);
    printf("  ");
    gotoxy(Xofboard+22,Yofboard+3+9);
    printf("��");
    Sleep(1000);
    system("cls");
    for(i=0;i<25;i++)
    {
        gotoxy(Xofboard,Yofboard+i);
        printf("��");
        gotoxy(Xofboard+48,Yofboard+i);
        printf("��");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(Xofboard+i,Yofboard);
        printf("��");
        gotoxy(Xofboard+i,Yofboard+24);
        printf("��");
    }
    for(i=0;i<5;i++)
    {
        gotoxy(Xofboard+2+22,Yofboard+i+9);
        printf("��");
    }
    Sleep(1000);
    system("cls");
}


void showkey()
{
     gotoxy(Xofboard+54,Yofboard+1);
     if(key[0]==' ')
        printf("A����λ:�ո�");
     else
        printf("A����λ:%c",key[0]);
    gotoxy(Xofboard+54,Yofboard+3);
    if(key[1]==' ')
        printf("B����λ:�ո�");
     else
        printf("B����λ:%c",key[1]);
     gotoxy(Xofboard+54,Yofboard+5);
     if(key[2]==' ')
        printf("C����λ:�ո�");
     else
        printf("C����λ:%c",key[2]);
     gotoxy(Xofboard+54,Yofboard+7);
    if(key[3]==' ')
        printf("D����λ:�ո�");
    else
        printf("D����λ:%c",key[3]);
     gotoxy(Xofboard+54,Yofboard+9);
    if(key[4]==' ')
        printf("�Ӻż�λ:�ո�");
     else
        printf("�Ӻż�λ:%c",key[4]);
     gotoxy(Xofboard+54,Yofboard+11);
    if(key[5]==' ')
        printf("���ż�λ:�ո�");
     else
       printf("���ż�λ:%c",key[5]);
     gotoxy(Xofboard+54,Yofboard+13);
    if(key[6]==' ')
        printf("�˺ż�λ:�ո�");
     else
        printf("�˺ż�λ:%c",key[6]);
     gotoxy(Xofboard+54,Yofboard+15);
    if(key[7]==' ')
        printf("���ż�λ:�ո�");
     else
        printf("���ż�λ:%c",key[7]);
     gotoxy(Xofboard+54,Yofboard+17);
     if(key[8]==' ')
        printf("������λ:�ո�");
     else
        printf("������λ:%c",key[8]);
     gotoxy(Xofboard+54,Yofboard+19);
    if(key[9]==' ')
        printf("������λ:�ո�");
     else
        printf("������λ:%c",key[9]);
     gotoxy(Xofboard+54,Yofboard+21);
     if(key[10]==' ')
        printf("���λ��:�ո�");
     else
        printf("���λ��:%c",key[10]);
     gotoxy(Xofboard+54,Yofboard+23);
     if(key[11]==' ')
        printf("�ҹ�λ��:�ո�");
     else
        printf("�ҹ�λ��:%c",key[11]);
}


void gotoxy(int x, int y)//�ƶ��������x,y��������ԭ����Ҳ��֪�����Ǵ����ϳ��Ĵ���
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}


void gotoprint(int x, int y,int num,int den,int c)//�����ڣ�x,y������ӡ/ɾ���߿򡢴�ӡ���֡���ӡ�Ӽ��˳��ţ�c����ѡ��ͬ����
{
	gotoxy(x, y);
	switch(c)
	{
	    case 0://��ӡ�߿�
	        printf(".");
	        break;
	    case 1://ɾ���߿�
            printf("  ");
            break;
        case 2://��ӡ����
            if(num<10)
                gotoxy(x+2, y);
            else
                if(num<1000)
                gotoxy(x+1,y);
            printf("%d",num);
            break;
        case 3://��ӡ����
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
        case 4://��ӡ�Ӻ�
            printf("+");
            break;
        case 5://��ӡ����
            printf("-");
            break;
        case 6://��ӡ�˺�
            printf("*");
            break;
        case 7://��ӡ����
            printf("/");
            break;
	}
}


void crebor3(int x, int y,int c)//�ڣ�x,y������ʾ���߿���������cΪ4��7����Ӧѡ��Ӽ��˳�
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


void crebor3x4(int x,int y)//�ڣ�x,y������ʾ�ĸ����߿�������+-*/�����Ϊ2
{
    int i;
    for(i=1;i<2*4;i+=2)
    {
        crebor3(x+4*i,y,4+((i-1)/2));
    }
}


void crebor5(int x,int y,int num,int den,int c)//�ڣ�x,y������ʾ��5*5�߿�������������c����ѡ���ӡ�������Ƿ�����0Ϊ������1Ϊ����,2Ϊ����ӡ�߿�
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


void crebor5x4(int x,int y,int numa,int dena,int numb,int denb,int numc,int denc,int numd,int dend,int a,int b,int c,int d)//�ڣ�x,y������ʾ4����5*5�߿�����������A,B,C,D��a,b,c,d����ѡ��ABCD��ӡ�������������ǲ���ӡ��0Ϊ������1Ϊ������2Ϊ����ӡ�߿�
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


void crebor535(int x,int y,int num1,int den1,int num2,int den2,int a,int b,int c)//�ڣ�x,y������ʾ2����5*5�߿�����������A,B,�����Ǽ���һ����3*3�߿���������a,b����ѡ��AB�������Ƿ�����0Ϊ������1Ϊ������2Ϊ�տ�cȡֵ0��1��2��3��-3����ѡ���ӡ+-*/,���ǿտ�
{
    crebor5(x,y,num1,den1,a);
    crebor3(x+12, y+1,c+4);
    crebor5(x+20,y,num2,den2,b);
}


void crebordwn1(int x,int y,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//������������ʱ�����°벿�ַ���upnuma(b)/updena(b)Ϊ�μ�����������numa(bcd)/dena(bcd)Ϊ�ײ���Ŀ���е�����
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


void crebordwn2(int x,int y,int upnum,int upden,int up,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//������������ʱ�����°벿�ַ���upnum/updenΪ��������numa(bcd)/dena(bcd)Ϊ�ײ���Ŀ���е�����
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


void crebor1(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//����������ʱ�ͷ����Ľ������壬scoreΪ�÷֣�timeΪʱ�䣬swit=0ʱ��ʾ���������棬swit=1ʱ��ʾ����������棬upnuma(b)/updena(b)Ϊ�μ�����������numa(bcd)/dena(bcd)Ϊ�ײ���Ŀ���е�����
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


void crebor(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//����������ʱ�ͷ����Ĵ��ܱ߿�Ľ������壬scoreΪ�÷֣�timeΪʱ�䣬swit=0ʱ��ʾ���������棬swit=1ʱ��ʾ����������棬upnuma(b)/updena(b)Ϊ�μ�����������numa(bcd)/dena(bcd)Ϊ�ײ���Ŀ���е�����,����mainbody
{
    showkey();
    crebor1(x+2,y+1,score,swit,upnuma,updena,ua,upnumb,updenb,ub,uc,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
    int i;
    for(i=0;i<25;i++)
    {
        gotoxy(x,y+i);
        printf("��");
        gotoxy(x+48,y+i);
        printf("��");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(x+i,y);
        printf("��");
        gotoxy(x+i,y+24);
        printf("��");
    }
}


void crebor_forwel(int x,int y,int score,int swit,int upnuma,int updena,int ua,int upnumb,int updenb,int ub,int uc,int numa,int dena,int a,int numb,int denb,int b,int numc,int denc,int c,int numd,int dend,int d)//����������ʱ�ͷ����Ĵ��ܱ߿�Ľ������壬scoreΪ�÷֣�timeΪʱ�䣬swit=0ʱ��ʾ���������棬swit=1ʱ��ʾ����������棬upnuma(b)/updena(b)Ϊ�μ�����������numa(bcd)/dena(bcd)Ϊ�ײ���Ŀ���е�����,����welcome
{
    crebor1(x+2,y+1,score,swit,upnuma,updena,ua,upnumb,updenb,ub,uc,numa,dena,a,numb,denb,b,numc,denc,c,numd,dend,d);
    int i;
    for(i=0;i<25;i++)
    {
        gotoxy(x,y+i);
        printf("��");
        gotoxy(x+48,y+i);
        printf("��");
    }
    for(i=2;i<48;i+=2)
    {
        gotoxy(x+i,y);
        printf("��");
        gotoxy(x+i,y+24);
        printf("��");
    }
}


void clearbor(int x,int y,int ul,int um,int ur,int da,int db,int dc,int dd)//�ѷ���������գ�1Ϊ���
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


void give(int give[],int given[])//��give[]��ֵһһ����given[]
{
    int i;
    for(i=0;i<3;i++)
    {
        given[i]=give[i];
    }
}


int findvoid(int a,int b,int c,int d)//�ҳ����Ŀ�λ����ȫ�գ�����-1
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


int redu(int a[])//����Լ��a[0]��a[1],��Լ�þ�����a[2]��Ϊ0,������1
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
    int start=0;//ָʾ��ʱ��ʼ��û��0Ϊû��ʼ��1Ϊ��ʼ
    int score;//����
    int skip;//����ָʾ�Ƿ�������0û����1��
    int eras;//����ָʾ�Ƿ������ͬ��
    int a[3],b[3],c[3],d[3];//��Ŀ���ĸ�������a[]Ϊ����a[0]Ϊnum,a[1]Ϊden,a[2]=0����ʾaΪ������a[2]=1,��ʾaΪ������a[2]=2��ʾ��λ�ÿ���
    int le[3],ri[3],mi;//ָʾ�μ�������������Լ�������Ƿ��λ,�÷�ͬ��,�����mi�÷���mi=0,1,2,3,����+-*/��mi=-3����տ�
    int succeed;//ָʾ�����û�����24��0Ϊ���1Ϊ��ԣ�Ϊ2����û�����
    char read;//��ȡ�����ϵİ�����ֵ
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
       le[2]=2;mi=-3;ri[2]=2;//�տ�
       system("cls");
       if(i==0&&start==0)
       {
           _beginthread(coudon,0,100);
           //_beginthread(showkey,0,NULL);
           Sleep(1);//�̳߳�ʼ��ʱ��
           start=1;
       }
       crebor(Xofboard,Yofboard,score,1,0,0,2,0,0,2,-3,a[0],1,0,b[0],1,0,c[0],1,0,d[0],1,0);//��ʾ��Ŀ
       while(stop==0)
       {
           read=getch();
           if(read==key[0])//ѡA
           {
                if(le[2]==2&&a[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(a,le);
                   a[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&a[2]!=2)//������λ��ѡ�������������λ
               {
                   give(a,ri);
                   a[2]=2;
               }
           }
            else if(read==key[1])//ѡB
           {
                if(le[2]==2&&b[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(b,le);
                   b[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&b[2]!=2)//������λ��ѡ�������������λ
               {
                   give(b,ri);
                   b[2]=2;
               }
           }
            else if(read==key[2])//ѡC
           {
                  if(le[2]==2&&c[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(c,le);
                   c[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&c[2]!=2)//������λ��ѡ�������������λ
               {
                   give(c,ri);
                   c[2]=2;
               }
           }
          else if(read==key[3])//ѡD
           {
                  if(le[2]==2&&d[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(d,le);
                   d[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&d[2]!=2)//������λ��ѡ�������������λ
               {
                   give(d,ri);
                   d[2]=2;
               }
           }
            else if(read==key[4])//ѡ+
           {
               mi=0;
           }
           else if(read==key[5])//ѡ-
           {
               mi=1;
           }
            else if(read==key[6])//ѡ*
           {
               mi=2;
           }
           else if(read==key[7])//ѡ/
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
           else if(read==key[10]&&le[2]!=2)//���λ
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a�п�λ
                        give(le,a);
                        le[2]=2;
                        break;
                    case 1://b�п�λ
                        give(le,b);
                        le[2]=2;
                        break;
                    case 2://c�п�λ
                        give(le,c);
                        le[2]=2;
                        break;
                    case 3://d�п�λ
                        give(le,d);
                        le[2]=2;
                        break;
                }
           }
           else if(read==key[11]&&ri[2]!=2)//�ҹ�λ
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a�п�λ
                        give(ri,a);
                        ri[2]=2;
                        break;
                    case 1://b�п�λ
                        give(ri,b);
                        ri[2]=2;
                        break;
                    case 2://c�п�λ
                        give(ri,c);
                        ri[2]=2;
                        break;
                    case 3://d�п�λ
                        give(ri,d);
                        ri[2]=2;
                        break;
                }
           }
           else//���������͵���ˢ��
           {
               system("cls");
               crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);
           }
           if((skip+eras+stop)!=0)
            break;
           system("cls");
           crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//����ʾ�������
           if(le[2]!=2&&ri[2]!=2&&mi!=-3)//���������������������λ����ʼ����
           {

               switch(mi)
               {
               case 0://��
                  le[0]=le[0]*ri[1]+ri[0]*le[1];
                  le[1]=le[1]*ri[1];//�������le��
                  redu(le);
                  break;
               case 1://��
                  le[0]=le[0]*ri[1]-ri[0]*le[1];
                  le[1]=le[1]*ri[1];//�������le��
                  redu(le);
                  break;
               case 2://��
                  le[0]=le[0]*ri[0];
                  le[1]=le[1]*ri[1];
                  redu(le);
                  break;
               case 3://��
                   if(ri[0]==0)
                   {
                       //_beginthread(errormes,0,1);
                       switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a�п�λ
                                give(le,a);
                                le[2]=2;
                                break;
                            case 1://b�п�λ
                                give(le,b);
                                le[2]=2;
                                break;
                            case 2://c�п�λ
                                give(le,c);
                                le[2]=2;
                                break;
                            case 3://d�п�λ
                                give(le,d);
                                le[2]=2;
                                break;
                        }
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a�п�λ
                                give(ri,a);
                                ri[2]=2;
                                break;
                            case 1://b�п�λ
                                give(ri,b);
                                ri[2]=2;
                                break;
                            case 2://c�п�λ
                                give(ri,c);
                                ri[2]=2;
                                break;
                            case 3://d�п�λ
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
            crebor(Xofboard,Yofboard,score,0,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//��ʾ������
            if(stop==1)
                break;
            Sleep(anim);
            switch(findvoid(a[2],b[2],c[2],d[2]))//���ݼ������洢����
            {
                case -1://������ȫ���������
                    if(le[2]==0&&le[0]==24)
                    {
                        succeed=1;
                        undonum=undonum-1;
                    }
                    else
                        succeed=0;
                    break;
                case 0://a�п�λ
                    give(le,a);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 1://b�п�λ
                    give(le,b);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 2://c�п�λ
                    give(le,c);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 3://d�п�λ
                    give(le,d);
                    le[2]=2;
                    ri[2]=2;
                    break;
            }
            system("cls");
            crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//��ʾ�洢���
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
    int can[200];//ָʾ����������Щ�н⣬��Щû��;can[n]=0�����n��û�⣻can[n]=1�����n���н�
    int i;
    int start=0;//ָʾ��ʱ��ʼ��û��0Ϊû��ʼ��1Ϊ��ʼ
    int score;//����
    int skip;//����ָʾ�Ƿ�������0û����1��
    int eras;//����ָʾ�Ƿ������ͬ��
    int a[3],b[3],c[3],d[3];//��Ŀ���ĸ�������a[]Ϊ����a[0]Ϊnum,a[1]Ϊden,a[2]=0����ʾaΪ������a[2]=1,��ʾaΪ������a[2]=2��ʾ��λ�ÿ���
    int le[3],ri[3],mi;//ָʾ�μ�������������Լ�������Ƿ��λ,�÷�ͬ��,�����mi�÷���mi=0,1,2,3,����+-*/��mi=-3����տ�
    int succeed;//ָʾ�����û�����24��0Ϊ���1Ϊ��ԣ�Ϊ2����û�����
    char read;//��ȡ�����ϵİ�����ֵ
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
       le[2]=2;mi=-3;ri[2]=2;//�տ�
       system("cls");
       if(i==0&&start==0)
       {
           _beginthread(coudon,0,100);
           //_beginthread(showkey,0,NULL);
           Sleep(1);//�̳߳�ʼ��ʱ��
           start=1;
       }
       crebor(Xofboard,Yofboard,score,1,0,0,2,0,0,2,-3,a[0],1,0,b[0],1,0,c[0],1,0,d[0],1,0);//��ʾ��Ŀ
       while(stop==0)
       {
           read=getch();
           if(read==key[0])//ѡA
           {
                if(le[2]==2&&a[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(a,le);
                   a[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&a[2]!=2)//������λ��ѡ�������������λ
               {
                   give(a,ri);
                   a[2]=2;
               }
           }
            else if(read==key[1])//ѡB
           {
                if(le[2]==2&&b[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(b,le);
                   b[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&b[2]!=2)//������λ��ѡ�������������λ
               {
                   give(b,ri);
                   b[2]=2;
               }
           }
            else if(read==key[2])//ѡC
           {
                  if(le[2]==2&&c[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(c,le);
                   c[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&c[2]!=2)//������λ��ѡ�������������λ
               {
                   give(c,ri);
                   c[2]=2;
               }
           }
          else if(read==key[3])//ѡD
           {
                  if(le[2]==2&&d[2]!=2)//����δ��λ��ѡ�����������λ
               {
                   give(d,le);
                   d[2]=2;
               }
               if(le[2]!=2&&ri[2]==2&&d[2]!=2)//������λ��ѡ�������������λ
               {
                   give(d,ri);
                   d[2]=2;
               }
           }
            else if(read==key[4])//ѡ+
           {
               mi=0;
           }
           else if(read==key[5])//ѡ-
           {
               mi=1;
           }
            else if(read==key[6])//ѡ*
           {
               mi=2;
           }
           else if(read==key[7])//ѡ/
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
           else if(read==key[10]&&le[2]!=2)//���λ
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a�п�λ
                        give(le,a);
                        le[2]=2;
                        break;
                    case 1://b�п�λ
                        give(le,b);
                        le[2]=2;
                        break;
                    case 2://c�п�λ
                        give(le,c);
                        le[2]=2;
                        break;
                    case 3://d�п�λ
                        give(le,d);
                        le[2]=2;
                        break;
                }
           }
           else if(read==key[11]&&ri[2]!=2)//�ҹ�λ
           {
                switch(findvoid(a[2],b[2],c[2],d[2]))
                {
                    case 0://a�п�λ
                        give(ri,a);
                        ri[2]=2;
                        break;
                    case 1://b�п�λ
                        give(ri,b);
                        ri[2]=2;
                        break;
                    case 2://c�п�λ
                        give(ri,c);
                        ri[2]=2;
                        break;
                    case 3://d�п�λ
                        give(ri,d);
                        ri[2]=2;
                        break;
                }
           }
           else//���������͵���ˢ��
           {
               system("cls");
               crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);
           }
           if((skip+eras+stop)!=0)
            break;
           system("cls");
           crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//����ʾ�������
           if(le[2]!=2&&ri[2]!=2&&mi!=-3)//���������������������λ����ʼ����
           {

               switch(mi)
               {
               case 0://��
                  le[0]=le[0]*ri[1]+ri[0]*le[1];
                  le[1]=le[1]*ri[1];//�������le��
                  redu(le);
                  break;
               case 1://��
                  le[0]=le[0]*ri[1]-ri[0]*le[1];
                  le[1]=le[1]*ri[1];//�������le��
                  redu(le);
                  break;
               case 2://��
                  le[0]=le[0]*ri[0];
                  le[1]=le[1]*ri[1];
                  redu(le);
                  break;
               case 3://��
                   if(ri[0]==0)
                   {
                        //_beginthread(errormes,0,1);
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a�п�λ
                                give(le,a);
                                le[2]=2;
                                break;
                            case 1://b�п�λ
                                give(le,b);
                                le[2]=2;
                                break;
                            case 2://c�п�λ
                                give(le,c);
                                le[2]=2;
                                break;
                            case 3://d�п�λ
                                give(le,d);
                                le[2]=2;
                                break;
                        }
                        switch(findvoid(a[2],b[2],c[2],d[2]))
                        {
                            case 0://a�п�λ
                                give(ri,a);
                                ri[2]=2;
                                break;
                            case 1://b�п�λ
                                give(ri,b);
                                ri[2]=2;
                                break;
                            case 2://c�п�λ
                                give(ri,c);
                                ri[2]=2;
                                break;
                            case 3://d�п�λ
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
            crebor(Xofboard,Yofboard,score,0,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//��ʾ������
            if(stop==1)
                break;
            Sleep(anim);
            switch(findvoid(a[2],b[2],c[2],d[2]))//���ݼ������洢����
            {
                case -1://������ȫ���������
                    if(le[2]==0&&le[0]==24)
                    {
                        succeed=1;
                        undonum=undonum-1;
                    }
                    else
                        succeed=0;
                    break;
                case 0://a�п�λ
                    give(le,a);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 1://b�п�λ
                    give(le,b);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 2://c�п�λ
                    give(le,c);
                    le[2]=2;
                    ri[2]=2;
                    break;
                case 3://d�п�λ
                    give(le,d);
                    le[2]=2;
                    ri[2]=2;
                    break;
            }
            system("cls");
            crebor(Xofboard,Yofboard,score,1,le[0],le[1],le[2],ri[0],ri[1],ri[2],mi,a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2],d[0],d[1],d[2]);//��ʾ�洢���
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


void Body(int sw)//swΪ0ʱΪɨ��ģʽ��Ϊ����ֵʱΪƽ̹ģʽ
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
	printf("GAME   OVER   o(*������*)o");
	gotoxy(31,13);
	printf("Your Score is %d", donenum);
	gotoxy(22, 17);
    printf("     ��������鿴δ����Ľ�>>>>>");
	if(donenum<=10)
	{
	    gotoxy(22, 15);
	    printf("  ��ò�ѽ (   ��  �� �� ) ��������");
	}
	if(donenum>10&&donenum<=20)
    {
	    gotoxy(22, 15);
	    printf("������Ŷ������Ŭ��o(  �� _ ��  )o");
	}
	if(donenum>20&&donenum<=40)
    {
        gotoxy(22, 15);
	    printf("��( �� �� �� ||)  ������е���˰�");
    }
    if(donenum>40)
    {
        gotoxy(22, 15);
	    printf("�������������ɰ�(�s�㧥��)�s�� �ߩ���");
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
        printf("%d %d %d %d �Ľ⣺",undone[4*i],undone[4*i+1],undone[4*i+2],undone[4*i+3]);
	    Prtsolv(40,11+i*2,undone[4*i],undone[4*i+1],undone[4*i+2],undone[4*i+3]);
    }
    gotoxy(30, 11+i*2);
    printf("�����������>>>>>");
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
	printf("����Ϸ���ᵽ�ġ������������ָ��ĸ��ո�");
	gotoxy(20, 15);
	printf("�뽫���뷨����Ӣ�ģ������������>>>>>");
	getch();
    while(1)
    {
        system("cls");
        gotoxy(15,7);
        printf("/********************************************************************************/");
        gotoxy(15, 27);
        printf("/********************************************************************************/");
        gotoxy(24, 11);
        printf("                            ��Ϸ����");
        gotoxy(24, 13);
        printf("          ����ģʽ��                       ���ģʽ��");
        gotoxy(24, 15);
        printf("ʹ����ʾ������A��B��C��D�ĸ���        ����A��B��C��D����");
        gotoxy(24, 17);
        printf("      ͨ�������������24             ϵͳ���Զ���ʾ�����");
        gotoxy(24, 19);
        printf("  ���һ���һ�֣���ʱ100��         ���޽⣬ϵͳ��ʾ���޽⡱");
        gotoxy(23, 23);
        printf("      ����q�����������ģʽ��������������������ģʽ>>>>>");
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
	printf("����ģʽ��Ϊ���֣�");
	gotoxy(16, 13);
	printf("ƽ̹ģʽ���������Ᵽ֤�н⣬��������С��������");
	gotoxy(16, 15);
	printf("ɨ��ģʽ������֤��Ŀ���н⣬�Ҳ�һ��С��������");
	gotoxy(30, 17);
	printf("�����������>>>>>");
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
	    printf("��λ���ã�������Сд��ĸ��ո�����λ����Ҫʹ���ظ���λ����");
	    gotoxy(5,18);
	    printf("��");
	    gotoxy(54,2);
	    printf("��ѡ��A���ļ�λ��");
	    key[0]=getch();
	    gotoxy(71,2);
	    if(key[0]==' ')
            printf("�ո�");
        else
	        printf("%c",key[0]);
        gotoxy(5,18);
	    printf(" ");
	    gotoxy(17,18);
	    printf("��");
	    while(1)
        {
            gotoxy(54,4);
	        printf("��ѡ��B���ļ�λ��");
	        key[1]=getch();
            if(judg_same(key,1)==0)
                break;
            else
            {
                gotoxy(54,4);
                printf("                                  ");
                gotoxy(54,4);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,4);
                printf("                    ");
            }
        }
	    gotoxy(71,4);
	    if(key[1]==' ')
            printf("�ո�");
        else
	        printf("%c",key[1]);
	    gotoxy(17,18);
	    printf(" ");
	    gotoxy(29,18);
	    printf("��");
	    while(1)
        {
            gotoxy(54,6);
	        printf("��ѡ��C���ļ�λ��");
	        key[2]=getch();
            if(judg_same(key,2)==0)
                break;
            else
            {
                gotoxy(54,6);
                printf("                                  ");
                gotoxy(54,6);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,6);
                printf("                    ");
            }
        }
	    gotoxy(71,6);
	    if(key[2]==' ')
            printf("�ո�");
        else
	        printf("%c",key[2]);
	    gotoxy(29,18);
	    printf(" ");
	    gotoxy(41,18);
	    printf("��");
	    while(1)
        {
            gotoxy(54,8);
	        printf("��ѡ��D���ļ�λ��");
            key[3]=getch();
            if(judg_same(key,3)==0)
                break;
            else
            {
                gotoxy(54,8);
                printf("                                  ");
                gotoxy(54,8);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,8);
                printf("                    ");
             }
        }
	    gotoxy(71,8);
	    if(key[3]==' ')
            printf("�ո�");
        else
	        printf("%c",key[3]);
	    gotoxy(41,18);
	    printf(" ");
	    gotoxy(11,18);
	    printf("��");
	   while(1)
       {
            gotoxy(76,2);
	        printf("��ѡ��Ӻŵļ�λ��");
	        key[4]=getch();
            if(judg_same(key,4)==0)
                break;
            else
            {
                gotoxy(76,2);
                printf("                                  ");
                gotoxy(76,2);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(76,2);
                printf("                    ");
            }
        }
	    gotoxy(94,2);
	    if(key[4]==' ')
            printf("�ո�");
        else
            printf("%c",key[4]);
        gotoxy(11,18);
	    printf(" ");
	    gotoxy(19,18);
	    printf("��");
	    while(1)
        {
            gotoxy(76,4);
	        printf("��ѡ����ŵļ�λ��");
	        key[5]=getch();
            if(judg_same(key,5)==0)
                break;
            else
            {
                gotoxy(76,4);
                printf("                                  ");
                gotoxy(76,4);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(76,4);
                printf("                    ");
            }
        }
	    gotoxy(94,4);
        if(key[5]==' ')
            printf("�ո�");
        else
	        printf("%c",key[5]);
	    gotoxy(19,18);
	    printf(" ");
	    gotoxy(27,18);
	    printf("��");
	    while(1)
        {
            gotoxy(76,6);
	        printf("��ѡ��˺ŵļ�λ��");
	        key[6]=getch();
            if(judg_same(key,6)==0)
                break;
            else
            {
                gotoxy(76,6);
                printf("                                  ");
                gotoxy(76,6);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(76,6);
                printf("                    ");
           }
        }
	    gotoxy(94,6);
     	if(key[6]==' ')
            printf("�ո�");
        else
	        printf("%c",key[6]);
	    gotoxy(27,18);
	    printf(" ");
	    gotoxy(35,18);
	    printf("��");
	    while(1)
        {
            gotoxy(76,8);
	        printf("��ѡ����ŵļ�λ��");
	        key[7]=getch();
            if(judg_same(key,7)==0)
                break;
            else
            {
                gotoxy(76,8);
                printf("                                  ");
                gotoxy(76,8);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(76,8);
                printf("                    ");
            }
        }
	    gotoxy(94,8);
	    if(key[7]==' ')
            printf("�ո�");
        else
	        printf("%c",key[7]);
	    gotoxy(54,10);
	    printf("*ERASE����;������������ʱ����ʹ�øü���λ���ص���ʼ״̬");
	    gotoxy(35,18);
	    printf(" ");
	    gotoxy(3,18);
	    printf("��");
	    while(1)
        {
            gotoxy(54,12);
	        printf("��ѡ��ERASE���ļ�λ��");
	        key[8]=getch();
            if(judg_same(key,8)==0)
                break;
            else
            {
                gotoxy(54,12);
                printf("                                  ");
                gotoxy(54,12);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,12);
                printf("                    ");
            }
        }
	    gotoxy(75,12);
	    if(key[8]==' ')
            printf("�ո�");
        else
	        printf("%c",key[8]);
        gotoxy(54,14);
	    printf("*SKIP����;��ƽ̹ģʽ������3��������һ��");
	    gotoxy(54,16);
	    printf("ɨ��ģʽ�������޽��ⰴ�ü���ֱ������һ��");
	    gotoxy(3,18);
	    printf(" ");
	    gotoxy(43,18);
	    printf("��");
	    while(1)
        {
            gotoxy(54,18);
	        printf("��ѡ��SKIP���ļ�λ��");
	        key[9]=getch();
            if(judg_same(key,9)==0)
                break;
            else
            {
                gotoxy(54,18);
                printf("                                  ");
                gotoxy(54,18);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,18);
                printf("                    ");
            }
        }
	    gotoxy(74,18);
	    if(key[9]==' ')
            printf("�ո�");
        else
	        printf("%c",key[9]);
        gotoxy(43,18);
	    printf(" ");
        gotoxy(13,14);
	    printf("��");
	    while(1)
        {
            gotoxy(54,20);
	        printf("��ѡ�����λ����λ���ɽ����������������λ��ѡ����");
	        key[10]=getch();
            if(judg_same(key,10)==0)
                break;
            else
            {
                gotoxy(54,20);
                printf("                                                   ");
                gotoxy(54,20);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,20);
                printf("                    ");
            }
        }
         gotoxy(104,20);
	    if(key[10]==' ')
            printf("�ո�");
        else
	        printf("%c",key[10]);
        gotoxy(13,14);
	    printf(" ");
	    gotoxy(33,14);
	    printf("��");
        while(1)
        {
            gotoxy(54,22);
	        printf("��ѡ���ҹ�λ����λ���ɽ����������������λ��ѡ����");
	        key[11]=getch();
            if(judg_same(key,11)==0)
                break;
            else
            {
                gotoxy(54,22);
                printf("                                                   ");
                gotoxy(54,22);
                printf("��λ�ظ���");
                Sleep(800);
                gotoxy(54,22);
                printf("                    ");
            }
        }
         gotoxy(104,22);
	    if(key[11]==' ')
            printf("�ո�");
        else
	        printf("%c",key[11]);
        gotoxy(33,14);
	    printf(" ");
	    gotoxy(54,24);
        printf("���Լ��ļ�λѡ���Ƿ����⣿�������⣬����n������ѡ");
        gotoxy(54,26);
        printf("���ⰴ��s��������ƽ̹ģʽ���������������ɨ��ģʽ");
        ok=getch();
        if(ok!='n'&&ok!='N')
            break;
    }
}


int judg_same(char m[],int n)//�жϴ�m[0]��m[n-1]����û�и�m[n]�ظ���Ԫ�أ����У�����1��û�з���0
{
    int i;
    for(i=0;i<n;i++)
    {
        if(m[i]==m[n])
            return 1;
    }
    return 0;
}


int judg4_forprint(int a,int b,int c,int d)//�ж�a,b,c,d�Ƿ���ͨ���������ŵ������������24
{
    int e,f,g,h,i,k,l,n,o;
    int m[4];
    solvment[0]=-1;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//��ȡ��������������
            {
                m[0]=a;
                m[1]=b;
                m[2]=c;
                m[3]=d;
                solvment[1]=m[e];//��¼��һ�β������������
                solvment[2]=m[f];
                solvment[5]=g;//��¼��һ������������
                int num=m[e],den=1;
                ope(&num,&den,m[f],g);
                del(&m,e);
                del(&m,f-1);
                k=m[0];
                l=m[1];
                n=num;
                o=den;
                for(h=0;h<2;h++)//��ȡһ��num/den������������
                {
                    for(i=0;i<6;i++)
                    {
                        m[0]=k;
                        m[1]=l;
                        num=n;
                        den=o;
                        solvment[6]=i;//�ڶ��������
                        solvment[3]=m[h];
                        ope(&num,&den,m[h],i);
                        del(&m,h);
                        solvment[4]=m[0];
                        solvment[7]=judg2(num,den,m[0]);//����ж�
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


int judg4_special_forprint(int a,int b,int c,int d)//�ж�a,b,c,d�Ƿ���ͨ�������ŵ������������24
{
    int m[4];
    int e,f,g,h,i,j,k,l,n,o;
    solvment[0]=-1;
    for(e=0;e<3;e++)
    {
        for(f=e+1;f<4;f++)
        {
            for(g=0;g<6;g++)//��ȡ��������������
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


void Prtsolv2(int x,int y)//��ӡ����(solvment[1&2])����
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


void Prtsolv2_special(int x,int y)//��ӡ����(solvment[3&4])����
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


void Prtsolv3(int x,int y)//��ӡ��������
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


void Prtsolv4(int x,int y)//��ӡ��˳�����������
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


void Prtsolv4_special(int x,int y)//��ӡ��˳�����������
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


int Prtsolv(int x,int y,int a,int b,int c,int d)//�ڣ�x,y������ӡ��a,b,c,d�Ľⷨ
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
            printf("  %d %d %d %d  �޽�",a,b,c,d);
            return 0;
        }
    }
}


void solvque()//����welcome������ģʽ
{
    int i;
    char numb[2];//���ڴ洢��λ��
    char nu[1];//�����жϸ�λ�Ƿ�����
    int flag;//��nu���
    int a,b,c,d;
    gotoxy(15,7);
	printf("/**********************************************/");
	gotoxy(15, 27);
	printf("/**********************************************/");
	gotoxy(15,11);
	printf("ע��������������Ϊ���ڵ���1��С�ڵ���99������");
	while(1)
    {
        gotoxy(15,13);
        printf("                                                                                ");
        gotoxy(15,13);
        printf("                 ������A����");
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
            printf("���������ɶ������� ��_�� || �������㿴��ע���������");
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
        printf("                 ������B����");
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
            printf("���������ɶ������� ��_�� || �������㿴��ע���������");
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
        printf("                 ������C����");
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
            printf("���������ɶ������� ��_�� || �������㿴��ע���������");
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
        printf("                 ������D����");
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
            printf("���������ɶ������� ��_�� || �������㿴��ע���������");
            Sleep(1500);
        }
        else
        {
            break;
        }
    }
    gotoxy(28,21);
    printf("�⣺");
    Prtsolv(32,21,a,b,c,d);
    gotoxy(16,23);
    printf("����q���������ģʽ��������������������˵�>>>>>");
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
          printf("�Ƿ�����һ�֣�");
          gotoxy(32, 13);
          printf("ͬ�ⰴ��y����");
          gotoxy(28, 15);
           printf("������������˳�>>>>>");
           c=getch();
           if(c!='y'&&c!='Y')
               break;
      }
}
