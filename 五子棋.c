//这个程序是一个五子棋游戏
//有比较友好的UI
//能够正常进行游戏
//有简单的AI
//没有明显的BUG
//有简单的联机程序（只能在本机上使用，测试用）
//
//请在终端中运行
//如果在其他环境中运行，有些字符会无法对齐，请手动修改
//
//2015年10月22日  国栋

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

int m[16][16],n[16][16],zj[16][16];//m纪录棋盘，n备用
int i,j;//控制某些循环，并且表示纵坐标和横坐标
int w,v;//用来获取其他输入值
int p,q;//表示方向
int mode;//表示游戏模式 1双人 2人机 3联机
int win;//获胜者
int next,ni,nj;//下一个要输入的玩家
int before,bi,bj;//上一个已经输入的玩家和坐标
int ti;//输入的次数
char *t[16];//用来显示信息的字符串指针
int k;//控制循环（判定时的）
int k1,k2;//判定时计数
int flag;//判定时计数
int score;//计算当前的分数
int maxscore,mi,mj;//最大分数和当时的坐标
FILE *fp;
int buff;

int init();//初始化
int shuru();//输入下一步，或调用AI
int shuchu();//输出棋盘
int panding();//判定是否有人获胜
int ai();//人工智能
int code1();//用来表示重复代码的函数
int code2();//加分项
int code3();//减分项
int getscore();//用来评定当前的激励分数
int lianjiinit();//初始化联机模式
int readdata();//将文件中的数据读到n中
int printdata();//将n中的数据保存到文件中
int duqu();//将n中的数据转化到m中
int zhuanhuan();//将m中的数据转化到n中
int cheak();//对比m和n，将不同的坐标作为下一步

int main(){
loop1:
    init();
    system("clear");
    printf("五子棋游戏\n请选择：\n（1）双人游戏\n（2）人机对战\n（3）联机模式\n（4）！！！删除联机数据（如果上次游戏没有正常退出）！！！\n\n***请注意输入格式！***\n");
    scanf("%d",&w);
    if (w==1) {mode=1;}
    else if (w==2){mode=2;}
    else if (w==3){mode=3;lianjiinit();}
    else if (w==4){system("rm data");goto loop1;}
    else {printf("输入错误！已退出\n");return 0;}
    shuchu();
    while (win==0)
    {
        if(shuru())break;
        shuchu();
        win=panding();
    }
    system("rm data");
    if (win==1) {printf("玩家一获胜！\n");}
    if (win==2) {printf("玩家二获胜！\n");}
    return 0;
}
int lianjiinit()
{
    fp=fopen("data", "r");
    if (fp==NULL)
    {
        printf("新建文件\n");
        fp=fopen("data", "w");
        buff=1;
        zhuanhuan();
        printdata();
        fclose(fp);
    }
    else
    {
        fclose(fp);
        readdata();
        if (n[0][0]==1) {buff=2;}
        else {buff=1;}
        duqu();
    }
    return 0;
}
int zhuanhuan()
{
    printf("转换\n");
    for (i=1; i<16; i++)
    {
        for (j=1; j<16; j++)
        {
            if(buff==1){n[i][j]=m[i][j];}
            if (buff==2) {
                if (m[i][j]==1) {n[i][j]=2;}
                if (m[i][j]==2) {n[i][j]=1;}
            }
        }
    }
    return 0;
}
int duqu()
{
    printf("读取\n");
    printf("buff=%d\n",buff);
    for (i=1; i<16; i++)
    {
        for (j=1; j<16; j++)
        {
            if(buff==1){
                m[i][j]=n[i][j];
            }
            if (buff==2) {
                if (n[i][j]==1) {m[i][j]=2;}
                if (n[i][j]==2) {m[i][j]=1;}
            }
        }
    }
    return 0;
}
int readdata(){
//    printf("读文件\n");
    fp=fopen("data", "r");
    for (i=0; i<16; i++)
    {
        for (j=0; j<16; j++)
        {
            fscanf(fp, "%d", &n[i][j]);
//            printf("%d",n[i][j]);
        }
    }
    fclose(fp);
    return 0;
}
int printdata(){
    printf("保存\n");
    fp=fopen("data", "w+");//格式可能有问题，此处要求删掉之前的数据
    n[0][0]=buff;
    for (i=0; i<16; i++)
    {
        for (j=0; j<16; j++)
        {
            fprintf(fp, "%d ", n[i][j]);
        }
    }
    fclose(fp);
    return 0;
}
int code2(p,q){
    k1=0;k2=1;
    if (m[i-p][j-q]==next) {return 0;}
    if ((i-p)<1||(i-p)>16||(j-q)<1||(j-q)>16) {k1++;}
    if (m[i-p][j-q]==before) {k1++;}
    for (k=1; (m[i+p*k][j+q*k]==next||m[i+p*k][j+q*k]==0)&&k<5; k++)
    {if(m[i+p*k][j+q*k]==next){k2++;}else{score-=1;}}
    if ((i+p*k)<1||(i+p*k)>16||(j+q*k)<1||(j+q*k)>16) {k1++;}
    if (m[i+p*k][j+q*k]==before) {k1++;}
    if (k2==2&&k1==1) {score+=1;}
    if (k2==2&&k1==0) {score+=2;}
    if (k2==3&&k1==1) {score+=4;}
    if (k2==3&&k1==0)  {score+=8;}
    if (k2==4&&k1==1) {score+=16;}
    if (k2==4&&k1==0) {score+=1024;}
    if (k2==5) {score+=9999;}
    return 0;
}
int code3(p,q){
    k1=0;k2=1;
    if (m[i-p][j-q]==before) {return 0;}
    if ((i-p)<1||(i-p)>16||(j-q)<1||(j-q)>16) {k1++;}
    if (m[i-p][j-q]==next) {k1++;}
    for (k=1; (m[i+p*k][j+q*k]==before||m[i+p*k][j+q*k]==0)&&k<5; k++)
    {if (m[i+p*k][j+q*k]==before) {k2++;}else{score-=1;}}
    if ((i+p*k)<1||(i+p*k)>16||(j+q*k)<1||(j+q*k)>16) {k1++;}
    if (m[i+p*k][j+q*k]==next) {k1++;}
    if (k2==2&&k1==1) {score-=5;}
    if (k2==2&&k1==0) {score-=9;}
    if (k2==3&&k1==1) {score-=17;}
    if (k2==3&&k1==0) {score-=33;}
    if (k2==4&&k1==1) {score-=5000;}
    if (k2==4&&k1==0) {score-=9999;}
    return 0;
}
int getscore(){
    score=0;
    for (i=1; i<16; i++) {
        for (j=1; j<16; j++) {
            if (m[i][j]==next) {code2(1,1);code2(1,0);code2(1,-1);code2(0,1);}
            if (m[i][j]==before) {code3(1,1);code3(1,0);code3(1,-1);code3(0,1);}
        }
    }
    return 0;
}
int cheak()
{
    for (i=1; i<16; i++) {
        for (j=1; j<16; j++) {
            if (m[i][j]!=n[i][j]&&buff==1) {
                return 1;
            }
            if (buff==2) {
                if (n[i][j]==1) {n[i][j]=2;}
                else if(n[i][j]==2){n[i][j]=1;}
                if (m[i][j]!=n[i][j]) {
                    return 1;
                }
            }
        }
    }
//    printf("错误，没有找到下一步的动作\n");
    sleep(1);
    return 0;
}
int shuru()
{
    if (next==1&&mode==3) {
        printf("请输入下一步！\n");
    }
    if (next==2&&mode==2) {ai();}
    else if (next==2&&mode==3) {
        printf("请等待对方输入！\n");
        while (1) {
            readdata();
            if (n[0][0]!=buff) {
                if(cheak())
                {break;}
            }
            else
            {
//                printf("等待对方走棋中。。。\n");
                sleep(1);
            }
        }
    }
    else
        scanf("%x %x",&i,&j);
    if (i==0&&j==0) {
        return 1;
    }
    if (i>0&&i<16&&j>0&&j<16)
    {
        if (m[i][j]==0)
        {
            bi=i;
            bj=j;
            ti++;
            m[i][j]=next;
            before=next;
            if (next==2) {next=1;}
            else{next++;}
            zhuanhuan();
            printdata();
        }
        else {printf("此处已经有棋子！\n");}
    }
    else{
        printf("超出了范围！\n");
        getchar();
        getchar();
    }
    return 0;
}
int code1(p,q){
    i=bi;
    j=bj;
    flag=0;
    for (k=1; m[i+p*k][j+q*k]==before; k++) {flag++;}
    for (k=1; m[i-p*k][j-q*k]==before; k++) {flag++;}
    if(flag==4){return before;}
    return 0;
}
int panding(){
    if(code1(1,1)){return before;}
    if(code1(1,0)){return before;}
    if(code1(1,-1)){return before;}
    if(code1(0,1)){return before;}
    return 0;
}
int shuchu(){
    system("clear");
    for(i=0;i<16;i++)
    {
        for(j=0;j<16;j++)
        {
            if(i!=0&&j!=0)
            {
                if(i==bi&&j==bj)printf("➡️ ");
                else if(i==bi&&j==bj+1)printf("⬅️ ");
                else printf("  ");
            }
            if(i==0&&j==0)
            {
                printf("  ");
            }
            else if(i==0)
            {
                printf("  %x ",j);
            }
            else if(j==0)
            {
                printf("%x ",i);
            }
            else
            {
                if(m[i][j]==0){printf("⭕️ ");}
                if(m[i][j]==1){printf("🔴 ");}
                if(m[i][j]==2){printf("🔵 ");}
            }
        }
        if(i==6)
        {
            printf("    第%d回合",(ti/2)+1);
        }
        else if(i==7)
        {
            if(next==1)printf("    请 玩家1🔴   进行下一步！");
            if(next==2)printf("    请 玩家2🔵   进行下一步！");
        }
        else if(i==13)
        {
            if(mode==1)printf("     玩家控制");
            if(mode==2)printf("     电脑控制");
            if(mode==3)printf("     联机玩家");
        }
        else printf("    %s",t[i]);
        printf("\n\n");
    }
    return 0;
}
int ai(){
    if(ti<5)
    {
        if(m[8][8]==0){i=8;j=8;}
        else if(m[9][9]==0){i=9;j=9;}
        else if(m[9][8]==0){i=9;j=8;}
        else if(m[8][9]==0){i=8;j=9;}
    }
    else
    {
        maxscore=-100000;
        for (ni=1; ni<16; ni++) {
            for (nj=1; nj<16; nj++) {
                if (m[ni][nj]==0) {
                    m[ni][nj]=next;
                    getscore();
                    if (score>maxscore) {
                        maxscore=score;
                        mi=ni;
                        mj=nj;
                    }
                    m[ni][nj]=0;
                }
            }
        }
        i=mi;
        j=mj;
    }
    return 0;
}
int init(){
    t[0]="使用说明：";
    t[1]="输入坐标进行游戏";
    t[2]="例如： a 3    纵坐标＋空格＋横坐标";
    t[3]="如果出错，请退出并重新打开程序 ";
    t[4]="➡️ ⭕️ ⬅️ 中为上一步下的棋子";
    t[5]=" ";
    t[6]=" ";
    t[7]=" ";
    t[8]=" ";
    t[9]="玩家一🔴 ";
    t[10]=" 玩家控制";
    t[11]=" ";
    t[12]="玩家二🔵 ";
    t[13]=" ";
    t[14]="输入0 0退出";
    t[15]="Create by GD";
    win=0;
    next=1;
    return 0;
}
