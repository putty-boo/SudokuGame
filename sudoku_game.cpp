#include "sudoku_game.h"
#include "ui_sudoku_game.h"
#include "mysudoku.h"
#include "won.h"
#include "youlose.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h> //random (rand()%9+1)
#include <time.h>
#include <ctype.h>
#include <string.h>

using namespace std;

extern int yrov; //обращение к глобальной переменной
int a[9][9], z[9][9];  //массив a для сгенерированного судоку, массив z для судоку с нулями

void generation(int n,int a[][9]){ //генерация сетки судоку
    int j,i,c,k,a2[9][9];
    srand(time(NULL));
    for (j=0;j<n;j++) //генерация первой строки
    {
        a[0][j]=rand()%9+1;   //рандомно задается первая строка в матрице
        for (k=0;k<j&&(a[0][j]!=a[0][k]||k==j);k++);
            if (k<j)
                j=j-1;
    }
    for (i=0, j=0;j<3;j++) //заполнение первых трех квадратов 3 на 3
    {
        a[i+1][j+3]=a[i][j];
        a[i+2][j+6]=a[i][j];
        a[i+1][j+6]=a[i][j+3];
        a[i+2][j]=a[i][j+3];
        a[i+1][j]=a[i][j+6];
        a[i+2][j+3]=a[i][j+6];
    }
    for (j=0;j<3;j++) //заполнение остальных квадратов 3 на 3
    {
        for (i=0;i<3;i++)
        {
            a[i+3][j*3+1]=a[i][j*3];
            a[i+6][j*3+2]=a[i][j*3];
            a[i+3][j*3+2]=a[i][j*3+1];
            a[i+6][j*3+0]=a[i][j*3+1];
            a[i+3][j*3+0]=a[i][j*3+2];
            a[i+6][j*3+1]=a[i][j*3+2];
        }
    }
    for (i=0;i<3;i++) //цикл для перестановки нескольких строк
    {
        for (j=0;j<n;j++)
        {
            c=a[i*3][j];
            a[i*3][j]=a[i*3+2-i][j];
            a[i*3+2-i][j]=c;
        }
    }
    for (j=0;j<3;j++) //цикл для перестановки нескольких столбцов
    {
        for (i=0;i<n;i++)
        {
            c=a[i][j*3];
            a[i][j*3]=a[i][j*3+2-j];
            a[i][j*3+2-j]=c;
        }
    }
    for (j=0;j<n;j++)  //транспонирование матрицы (строки становятся столбцами и наоборот)
    {
        for (i=0;i<n;i++)
        {
            a2[i][j]=a[j][i];
        }
    }
    for (j=0;j<n;j++)
    {
        for (i=0;i<n;i++)
        {
            a[i][j]=a2[i][j];
        }
    }
}

void peresstroki(int n, int *ffl, int w2[][9], int b[][9]){ /*проверка по строкам, столбцам и квадратам 3 на 3,
которая ищет и удаляет значения подсказок из списка кандидатов*/
    int i,j,u=0,k2=0,r,f=0,k,q,f1=0,x2[150],y2[150],xx[150],yy[150],i1,u2=0;
    char strpr2[80][81],strpr1[80][81],strpr3[80][81];
    char strpr21[80][81];
    char *s,*s1,*s2;
    *ffl=0, f=0;    //ffl=0 - не было удаления, f - считает количество пустых ячеек (с кандидатами)
    for (i=0;i<n;i++) //преобразовывает все нули в строку "123456789" и запоминает координаты
    {
        for (j=0;j<n;j++)
        {
            if (b[i][j]==0)  //поиск и подсчет всех нулевых ячеек матрицы
            {
                sprintf(strpr1[f], "%d", w2[i][j]);     //переводит в строку соответствующую ей ячейку с кандидатами из матрицы w2[][]
                xx[f]=i;    //сохраняет координаты
                yy[f]=j;
                f++;
            }

        }
    }
    for (r=0;r<f;r++) //проверка по строкам. Цикл идет по строкам с кандидатами (по пустым ячейкам)
    {
        k2=0;   //для определения длины строки кандидатов
        for (j=0;j<n;j++)   //выделяет строку,в которой ищет подсказки
        {
            if (b[xx[r]][j]!=0)  //если нашлась подсказка в соответствующей строке xx[r]
            {
                sprintf(strpr2[u], "%d",b[xx[r]][j]);   //переводит ее в строку
                s=strpr1[r];                            //указатель на начало строки кандидатов
                while(*s)
                {
                    if(*s==*(strpr2[u])&&strlen(strpr1[r])!=1)  //если текущий кандидат равен подсказке и длина строки кандидатов не равна 1
                    {                                           //удаление кандидата,который совпал с подсказской
                        s1=s+1;
                        s2=s+strlen(s)+1;
                        strncpy(s,s1,s2-s1);
                        k2=strlen(strpr1[r]);        //определяет длину строки кандидатов
                        *ffl=1;       //было удаление
                        s++;    //переходим к следующему кандидату
                    }
                    else  s++;
                }
            }
        }
        w2[xx[r]][yy[r]]=atoi(strpr1[r]);   //строка оставшихся кандидатов переводится обратно в цифру и записывается в w2
        if (k2==1)                          //если длина строки кандидатов равна 1, то этот кандидат становится подсказской
        {
            b[xx[r]][yy[r]]=w2[xx[r]][yy[r]];   //переписывается в матрицу с подсказками (b[][])
            j=-1;                               //цикл возвращается назад, и проверка по строкам происходит заново с учетом новой подсказки
        }
    }
    for (r=0;r<f;r++) // аналогичная проверка по столбцам
    {
        k2=0;
        for (i=0;i<n;i++)
        {
            if (b[i][yy[r]]!=0)
            {
                sprintf(strpr2[u], "%d",b[i][yy[r]]);
                s=strpr1[r];
                while(*s)
                {
                    if(*s==*(strpr2[u])&&strlen(strpr1[r])!=1)
                    {
                        s1=s+1;
                        s2=s+strlen(s)+1;
                        strncpy(s,s1,s2-s1);
                        k2=strlen(strpr1[r]);
                        *ffl=1;
                        s++;
                    }
                    else  s++;
                }
            }
        }
        w2[xx[r]][yy[r]]=atoi(strpr1[r]);
        if (k2==1)
        {
            b[xx[r]][yy[r]]=w2[xx[r]][yy[r]];
            i=-1;
        }
    }
    // проверка по квадратам 3 на 3
    f1=0;   //количество пустых ячеек в квадрате
    for (q=0;q<3;q++) // разбивка матрицы на квадраты 3 на 3
    {
        for (k=0;k<3;k++)
        {
            for (i=0+3*q;i<3+q*3;i++)
            {
                for (j=0+3*k;j<3+3*k;j++)
                {
                    if (b[i][j]==0)
                    {
                        sprintf(strpr3[f1], "%d",w2[i][j]);
                        x2[f1]=i;
                        y2[f1]=j;
                        f1=f1+1;   //считает количество всех пустых ячеек в текущем квадрате 3 на 3
                    }
                    else w2[i][j]=b[i][j];   //подсказка переписывается в w2
                }
            }
            for (i=0+q*3;i<3+q*3;i++)   //цикл в текущем квадрате по подсказкам
                for (j=0+3*k;j<3+3*k;j++)
                {
                    if (b[i][j]!=0)  //если нашлась подсказка
                    {
                        sprintf(strpr21[u2], "%d",b[i][j]);  //переводим ее в строку
                        for (i1=0;i1<f1;i1++)   //цикл по всем нулевым ячейкам с кандидатами в текущем квадрате
                        {
                            k2=0;
                            s=strpr3[i1];
                            while(*s)
                            {
                                if(*s==*(strpr21[u2])&&strlen(strpr3[i1])!=1)
                                {
                                    s1=s+1;
                                    s2=s+strlen(s)+1;
                                    strncpy(s,s1,s2-s1);
                                    k2=strlen(strpr3[i1]);
                                    *ffl=1;
                                    s++;
                                }
                                else  s++;
                            }
                            w2[x2[i1]][y2[i1]]=atoi(strpr3[i1]);
                            if (k2==1)    //если длина строки кандидатов равна 1, то этот кандидат становится подсказской
                            {
                                b[x2[i1]][y2[i1]]=w2[x2[i1]][y2[i1]];
                                i1=-1;   // //цикл возвращается назад, и проверка по квадратам происходит заново с учетом новой подсказки
                                j=0+3*k;
                                i=0+3*q;
                            }
                        }
                    }
                }
            f1=0;
        }
    }
}

void odinochki(int n, int *ffl, int w2[][9], int b[][9]){  /*ищет уникальных (единственных) кандидатов,
сравнивая каждого кандидата текущей ячейки со всеми остальными кандидатами в других ячейках,
находящихся в соответствующих  строках, столбцах, квадратах 3 на 3 */
    int i,j,k,q,c,p[90],r2,rk,f2=0,f=0,xs1[150],ys1[150],xxk[90],yyk[90],u4=0,sr[90];
    char strpr4PR[80][81],strpr42pr[80][81],strpr4PRs[80][81],strprKV[80][81],*s,*s1;
    // поиск "ОДИНОЧЕК"
    //по столбцам
    *ffl=0, f=0;  //*ffl=0 - не нашлась одиночка, f - считает количество пустых ячеек (с кандидатами)
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (b[i][j]==0)  //находит пустую ячейку
            {
                xs1[f]=i;    //сохраняет координаты
                ys1[f]=j;
                sprintf(strpr4PR[f], "%d", w2[i][j]);    //переводит в строку соответствующую ей ячейку с кандидатами из матрицы w2[][]
                s=strpr4PR[f];                           //ставит указатель на начало строки
                while (*s)
                {
                    p[f]=0;              // p[f] - считает повторы
                    for (c=0;c<n;c++)    //выделяет соответствующий столбец для сравнения
                    {
                        if (b[c][ys1[f]]==0&&c!=i)    //находит пустую ячейку в соответствующем столбце, кроме текущей b[i][j]
                        {
                            sprintf(strpr42pr[u4], "%d", w2[c][ys1[f]]);   //переводит в строку соответствующую ей ячейку с кандидатами из матрицы w2[][]
                            s1=strpr42pr[u4];
                            while (*s1)
                            {
                                 if(*s1==*s)          //проверка на повтор текущего кандидата с кандидатами из других ячеек столбца
                                 {
                                     p[f]=p[f]+1;
                                     s1++;
                                 }
                                 else s1++;
                            }
                        }
                    }
                    if (p[f]==0) //если повторов не было
                    {
                        *(s+1)='\0';    //обрезает всех оставшихся кандитатов
                        w2[xs1[f]][ys1[f]]=atoi(s);  //присваивает w2 оставшегося одного кандидата в виде числа
                        b[xs1[f]][ys1[f]]= w2[xs1[f]][ys1[f]];  //присваивает уникального кандидата в матрицу с подсказками,(одиночка теперь подсказка)
                        peresstroki(n,&*ffl,w2,b);      //удаление значений подсказок из списка кандидатов
                        while (*ffl==1)                 //удаление значений подсказок из списка кандидатов, до тех пор пока ffl не будет равен 0
                            peresstroki(n,&*ffl,w2,b);
                        *ffl=1;    //было удаление
                        s++;
                    }
                    else s++;
                }
                f=f+1;  //считает пустые ячейки
            }
        }
    }
    // аналогично по строкам
    f=0, p[f]=0;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (b[i][j]==0)
            {
                xs1[f]=i;
                ys1[f]=j;
                sprintf(strpr4PRs[f], "%d", w2[i][j]);
                s=strpr4PRs[f];
                while (*s)
                {
                    p[f]=0;
                    for (c=0;c<n;c++)
                    {
                        if (b[xs1[f]][c]==0&&c!=j)
                        {
                            sprintf(strpr42pr[u4], "%d", w2[xs1[f]][c]);
                            s1=strpr42pr[u4];
                            while (*s1)
                            {
                                if(*s1==*s)
                                {
                                    p[f]=p[f]+1;
                                    s1++;
                                 }
                                 else s1++;
                            }
                        }
                    }
                    if (p[f]==0)
                    {
                        *(s+1)='\0';
                        w2[xs1[f]][ys1[f]]=atoi(s);
                        b[xs1[f]][ys1[f]]= w2[xs1[f]][ys1[f]];
                        *ffl=0;
                        peresstroki(n,&*ffl,w2,b);
                        while (*ffl==1)
                            peresstroki(n,&*ffl,w2,b);
                        *ffl=1;
                        s++;
                    }
                    else s++;
                }
                f=f+1;
            }
        }
    }
    // проверка по квадратам
    for (q=0;q<3;q++)    //деление матрицы на квадраты 3 на 3
    {
        for (k=0;k<3;k++)
        {
            f2=0;
            for (i=0+3*q;i<3+q*3;i++)
            {
                for (j=0+3*k;j<3+3*k;j++)
                {
                    if (b[i][j]==0)  //находит пустую ячейку в соответствующем квадрате
                    {
                        sprintf(strprKV[f2], "%d",w2[i][j]);  //переписывает в строку кандидатов, соответствующих текущей пустой ячейке
                        xxk[f2]=i;    //запоминает координаты текущей пустой ячейки в соответствующем квадрате
                        yyk[f2]=j;
                        f2=f2+1;   //считает количество всех пустых ячеек в текущем квадрате 3 на 3
                    }

                }
            }
            for (r2=0;r2<f2;r2++)  //фиксирует текущую строку из кандидатов для проверки ее с другими ячейками кандидатов
            {
                s=strprKV[r2];
                while (*s)
                {
                    sr[r2]=0;  //считает повторы
                    for (rk=0;rk<f2;rk++)  //открывает новый цикл для сравнения текущей строки кандидатов с другими ячейками кандидатов квадрата
                    {
                        if (rk!=r2)   //если не равен сам себе
                        {
                            s1=strprKV[rk];
                            while (*s1)
                            {
                                if (*s1==*s)   //если нашел повтор
                                {
                                    sr[r2]=sr[r2]+1;
                                    s1++;
                                }
                                else s1++;
                            }
                        }
                    }
                    if (sr[r2]==0)   //если повторов не было
                    {
                        *(s+1)='\0';   //обрезает всех оставшихся кандитатов
                        w2[xxk[r2]][yyk[r2]]=atoi(s);     //присваивает w2 оставшегося одного кандидата в виде числа
                        b[xxk[r2]][yyk[r2]]= w2[xxk[r2]][yyk[r2]]; //присваивает уникального кандидата в матрицу с подсказками,(одиночка теперь подсказка)
                        *ffl=0;  //не было удаления
                        peresstroki(n,&*ffl,w2,b);
                        while (*ffl==1)  //удаление значений подсказок из списка кандидатов, до тех пор пока ffl не будет равен 0
                            peresstroki(n,&*ffl,w2,b);
                        *ffl=1;
                        s++;
                    }
                    else s++;
                }
            }
        }
    }
}

// ПОИСК  ОТКРЫТЫХ ПАР
void para(int n,int *ffl,int w[][9],int b[][9]){    /*если в столбце,строке, квадрате 3 на 3 имеются две ячейки с одинаковыми парами кандидатов,
то эти кандидаты могут быть только у этих ячеек, т.е. эти кандидаты можно удалить из списка кандидатов в других ячейках этого столбца, строки, квадрата */
    int i,j,f,f2,c,cc,k,q,r2,rk,u3=0,xx1[90],yy1[90]; char strpr4PR[80][81],strpr42pr[80][81],strpr62pr[80][81];
    char *s,*s1,*s2;
    //поиск пары по столбцам
    f=0;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (b[i][j]==0)  //находит пустую ячейку
            {
                xx1[f]=i;   //запоминает координаты
                yy1[f]=j;
                sprintf(strpr4PR[f], "%d", w[i][j]);
                s=strpr4PR[f];
                if (strlen(strpr4PR[f])==2)  //если длина строки кандидатов равна 2, т.к. ищем только пару
                {
                    for (cc=0;cc<n;cc++)   //выделяет соответствующий столбец
                    {
                        if (b[cc][yy1[f]]==0&&cc!=i)   //находит пустую ячейку, не учитывая сам себя
                        {
                            sprintf(strpr42pr[u3], "%d", w[cc][yy1[f]]);  //переписывает в строку
                            if (strcmp(strpr4PR[f],strpr42pr[u3])==0)   //сравниваем две строки из кандидитов на идентичность
                            {
                                for (c=0;c<n;c++)
                                {
                                    if (b[c][yy1[f]]==0)  //поиск нулевого элемента в столбце
                                    {
                                        sprintf(strpr62pr[u3], "%d",w[c][yy1[f]]);
                                        s1=strpr4PR[f];
                                        while (*s1)
                                        {
                                            s=strpr62pr[u3];
                                            while(*s) //удаление значений пары из списка кандидат
                                            {
                                                if (*s==*s1&&strlen(strpr62pr[u3])!=1&&strcmp(strpr4PR[f],strpr62pr[u3])!=0)  /*сравнивает значение пары с кандидатами в других ячейках,
                                                    длина строки кандидатов не равна 1, иначе это подсказка и пара не сравнивается с идентичной ей парой*/
                                                {
                                                    s2=s+strlen(s)+1;
                                                    strncpy(s,s+1,s2-s-1);
                                                    w[c][yy1[f]]=atoi(strpr62pr[u3]);
                                                    if (strlen(strpr62pr[u3])==1) /*если остался один кандидат, то он становится подсказкой,
                                                    снова делаем проверки (с учётом этой подсказки)*/
                                                    {
                                                        b[c][yy1[f]]=w[c][yy1[f]];
                                                        peresstroki(n,&*ffl,w,b);
                                                        while (*ffl==1)
                                                            peresstroki(n,&*ffl,w,b);
                                                        odinochki(n,&*ffl,w,b);
                                                        while (*ffl==1)

                                                        odinochki(n,&*ffl,w,b);
                                                    }
                                                    s++;
                                                }
                                                else  s++;
                                            }
                                            s1++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                f=f+1; //переход на следующую пустую ячейку в поисках пары
            }
        }
    }
    // поиск открытых пар по строкам
    f=0;
    u3=0;
    *ffl=0;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (b[i][j]==0)
            {
                xx1[f]=i;
                yy1[f]=j;
                sprintf(strpr4PR[f], "%d", w[i][j]);
                s=strpr4PR[f];
                if (strlen(strpr4PR[f])==2)
                {
                    for (cc=0;cc<n;cc++)
                    {
                        if (b[xx1[f]][cc]==0&&cc!=j)
                        {
                            sprintf(strpr42pr[u3], "%d", w[xx1[f]][cc]);
                            if (strcmp(strpr4PR[f],strpr42pr[u3])==0)
                            {
                                for (c=0;c<n;c++)
                                {
                                    if (b[xx1[f]][c]==0&&c!=j&&c!=cc)
                                    {
                                        sprintf(strpr62pr[u3], "%d",w[xx1[f]][c]);
                                        s1=strpr4PR[f];   // пара, с которой будет сравнение
                                        while (*s1)
                                        {
                                            s=strpr62pr[u3];  //нулевые ячейки (с кандидатами) в строке, кроме найденных пар
                                            while(*s)
                                            {
                                                if (*s==*s1&&strlen(strpr62pr[u3])!=1&&strcmp(strpr4PR[f],strpr62pr[u3])!=0)
                                                {
                                                    s2=s+strlen(s)+1;
                                                    strncpy(s,s+1,s2-s-1);
                                                    w[xx1[f]][c]=atoi(strpr62pr[u3]);
                                                    if (strlen(strpr62pr[u3])==1)
                                                    {
                                                        b[xx1[f]][c]=w[xx1[f]][c];
                                                        *ffl=0;
                                                        peresstroki(n,&*ffl,w,b);
                                                        while (*ffl==1)
                                                            peresstroki(n,&*ffl,w,b);
                                                        odinochki(n,&*ffl,w,b);
                                                        while (*ffl==1)
                                                            odinochki(n,&*ffl,w,b);
                                                    }
                                                    s++;
                                                }
                                                else  s++;
                                            }
                                            s1++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                f=f+1;
            }
        }
    }
    // поиск открытых пар по квадратам 3 на 3
    f2=0;
    u3=0;
    for (q=0;q<3;q++)
    {
        for (k=0;k<3;k++)
        {
            f2=0;
            for (i=0+3*q;i<3+q*3;i++)
            {
                for (j=0+3*k;j<3+3*k;j++)
                {
                    if (b[i][j]==0)
                    {
                        sprintf(strpr4PR[f2], "%d",w[i][j]);
                        xx1[f2]=i;
                        yy1[f2]=j;
                        f2=f2+1;
                    }
                }
            }
            for (r2=0;r2<f2;r2++)
            {
                if (strlen(strpr4PR[r2])==2)
                {
                    for (rk=0;rk<f2;rk++)
                    {
                        if (rk!=r2)
                        {
                            if (strcmp(strpr4PR[r2],strpr4PR[rk])==0)
                            {
                                for (c=0;c<f2;c++)
                                {
                                    if (c!=r2&&c!=rk)
                                    {
                                        s1=strpr4PR[r2];
                                        while (*s1)
                                        {
                                            s=strpr4PR[c];
                                            while(*s)
                                            {
                                                if (*s==*s1&&strlen(strpr4PR[c])!=1&&strcmp(strpr4PR[r2],strpr4PR[c])!=0)
                                                {
                                                    s2=s+strlen(s)+1;
                                                    strncpy(s,s+1,s2-s-1);
                                                    w[xx1[c]][yy1[c]]=atoi(strpr4PR[c]);
                                                    if (strlen(strpr4PR[c])==1)
                                                    {
                                                        b[xx1[c]][yy1[c]]=w[xx1[c]][yy1[c]];
                                                        *ffl=0;
                                                        peresstroki(n,&*ffl,w,b);
                                                        while (*ffl==1)
                                                            peresstroki(n,&*ffl,w,b);
                                                        odinochki(n,&*ffl,w,b);
                                                        while (*ffl==1)
                                                            odinochki(n,&*ffl,w,b);
                                                    }
                                                    s++;
                                                }
                                                else  s++;
                                            }
                                            s1++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void main2(int yrov, int a[][9], int z[][9])
{
    int x[150],y[150],i,j,cj,ck,n=9,k1,m,w[9][9],b[9][9],f=0,ffl,pr;
    srand(time(NULL)); //без этой функции числа, выдаваемые ф-цией rand(), выдаются "случайно" только один раз
    pr=0; //количество неудачных решений
    generation(n,a);// генерирование сетки для судоку
    for (i=0;i<n;i++) //копируем матрицу a в z
    {
        for (j=0;j<n;j++)
            z[i][j]=a[i][j];
    }
    //УРОВНИ
    //кандидат - предполагаемое значение для пустой ячейки
    //подсказка - ячейка с единственным кандидатом
    if (yrov==1)
    {
        ck=rand()%7+35;  //  41-46 подсказок
    }
    if (yrov==2)
    {
        ck=rand()%7+41;  // 34-40 подсказки
    }
    if (yrov==3)
    {
        ck=rand()%8+48;    // 25-33 подсказки
    }
    for (cj=0;cj<ck;cj++) //цикл удаления ячеек
    {
        x[cj]=rand()%9+0; //рандомно выбирается ячейка
        y[cj]=rand()%9+0;
        while (z[x[cj]][y[cj]]==0) //цикл, не позволяющий брать уже удалённую ячейку
        {
            x[cj]=rand()%9+0;
            y[cj]=rand()%9+0;
        }
        z[x[cj]][y[cj]]=0; //удаление ячейки
        for (f=0,i=0;i<n;i++) //создаём список кандидатов для пустой ячейки (123456789)
        {
            for (j=0;j<n;j++)
            {
                if (z[i][j]==0)
                {
                    w[i][j]=123456789;  //w -массив, где вместо 0 записаны 123456789 (кандидаты)
                    b[i][j]=0;   //массив b дублирует массив z
                }
                else
                {
                    w[i][j]=z[i][j];  //подсказки переписываем
                    b[i][j]=z[i][j];
                }
            }
        }
        peresstroki(n,&ffl,w,b); //проверка по строкам, столбцам, квадратам 3 на 3
        while (ffl==1) //пока можно удалять кандидатов из списка, удаляем
            peresstroki(n,&ffl,w,b);
        m=0;   //решилась ли судоку
        for (i=0;i<n;i++)
        {
            for (j=0;j<n;j++)
                if (b[i][j]==0)
                    m=1; //не получилось единственное решение, т.е. несколько кандидатов

        }
        ffl=0, k1=0;
        if (m==1)
        {
            odinochki(n,&ffl,w,b);  /*ищет уникальных (единственных) кандидатов,
сравнивая каждого кандидата текущей ячейки со  всеми остальными кандидатами в других ячейках,
 находящихся в соответствующих  строках,столбцах, квадратах 3 на 3.*/
            while (ffl==1)
            {
                odinochki(n,&ffl,w,b);
            }
        }
        m=0;    //если судоку не решилась после проверок в peresstroki и odinochki то проверяется в para
        for (i=0;i<n;i++)
        {
            for (j=0;j<n;j++)
                if (b[i][j]==0)
                    m=1;
        }
        para(n,&ffl,w,b);
        m=0;
        for (i=0;i<n;i++)
        {
            for (j=0;j<n;j++)
                if (b[i][j]==0)
                    m=1;
        }
        if (m==1) //если не нашлось решение
        {
            z[x[cj]][y[cj]]=a[x[cj]][y[cj]]; //возвращает значение ячейки на место
            cj=cj-1;    //уменьшает цикл, чтобы удалить уже другую ячейку и проверить решение
            pr=pr+1;              //количество неудачных решений
            if (pr>20)
            {
                z[x[cj]][y[cj]]=a[x[cj]][y[cj]]; //возвращает значение ячейки на место
                cj=cj-1;    //уменьшает цикл, чтобы удалить уже другую ячейку и проверить решение
                ck=ck-1;   //увеличиваем количество подсказок
                pr=0;
            }
        }
    }  // Cудоку для решения создано
}

Sudoku_game::Sudoku_game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sudoku_game)
{
    main2(yrov, a, z);
    ui->setupUi(this);
    centerOnDesktop();
    ui->tableWidget->setRowCount(9); //создаётся таблица 9 на 9
    ui->tableWidget->setColumnCount(9);
    for(int row=0; row < ui->tableWidget->rowCount() ; row++)
        for (int column=0; column < ui->tableWidget->columnCount(); column++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(); //выделяется память под ячейку
            ui->tableWidget->setItem(row, column, item);     //данной ячейке ставятся в соотетствие строка и столбец
            if (z[row][column]!=0) /*если в массиве z данный элемент является не нулем,
            то программа записывает значение элемента в соответствующую ячейку таблицы*/
            {
                item->setText(QString("%1").arg(z[row][column]));
                ui->tableWidget->item(row,column)->setFlags(Qt::NoItemFlags); //запрет на редактирование ячейки пользователем
            }
            if (!(((row<3 || row>5) && column>2 && column<6) || (row>2 && row<6 && (column<3 || column>5))))
                //цветом выделяем определённые квадраты, чтобы было видно квадраты 3 на 3
                ui->tableWidget->item(row,column)->setBackground(QBrush(QColor(247,175,2)));
             //устанавливаем для каждой цифры положение внутри ячейки (по центру по вертикали и горизонтали)
            ui->tableWidget->item(row,column)->setTextAlignment(0x004 | 0x080);
            ui->tableWidget->item(row,column)->setTextColor(Qt::black); //устанавливаем цвет текста в ячейке
        }
        connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changed(QTableWidgetItem*)));
        /*отслеживание изменения ячейки, чтобы запустить сигнал проверки */
}

Sudoku_game::~Sudoku_game()
{
    delete ui;
}

void Sudoku_game::on_pushButton_clicked() //возврат к меню
{
    MySudoku * w = new MySudoku(this);
    w->show();
    hide();
}

void Sudoku_game::on_pushButton_2_clicked() //запускает ф-цию подсказки
{
    help(a);
}

void Sudoku_game::count(int a[9][9]) //проверка на заполненность судоку
{
    int kol=0,k=0, row, column,i;
    for(row=0;row<9;row++) //цикл считает количество заполненных цифрами ячеек
        for(column=0; column<9; column++){
            i=ui->tableWidget->item(row,column)->text().toInt();
            if(i>0 && i<10) k+=1; //k-счётчик заполненных ячеек
        }
    if(k==81){ //если судоку заполнено, проверяет то, как оно заполнено
        for(row=0; row<9; row++)
            for(column=0; column<9; column++){
                i=ui->tableWidget->item(row,column)->text().toInt();
                if(a[row][column]==i) kol+=1; //kol - счётчик правильно заполненных ячеек
            }
        if(kol==81){ //если все правильно заполнено, выводит окно выигрыша, скрывая окно игры
            won * w = new won(this);
            w->show();
            hide();
        }
        if(kol<81 && kol!=0){ //если не все ячейки
            YouLose * w = new YouLose(this);
            w->show();
            hide();
        }
    }
}

void Sudoku_game::changed(QTableWidgetItem *item) //надёжность ввода
{
    if((item->text().toInt()<=9 && item->text().toInt()>0))
        count(a);
    else
        item->setText(" ");
}

void Sudoku_game::help(int a[9][9]) //ф-ция подсказки
{
    int row, column, i;
    for(row=0;row<9;row++)
        for(column=0;column<9;column++){
            i=ui->tableWidget->item(row,column)->text().toInt();
            if(a[row][column]!=i && i>0 && i<10)
                ui->tableWidget->item(row,column)->setTextColor(Qt::red); //если пользователь поставил направильную цифру, программа выделит ее красной
            else
                ui->tableWidget->item(row,column)->setTextColor(Qt::black);
        }
}

void Sudoku_game::centerOnDesktop()
{
    quint16 screenWidth, width, screenHeight, height, x, y;
    QSize windowSize;

    screenWidth = QApplication::desktop()->width();
    screenHeight = QApplication::desktop()->height();
    windowSize = size();
    width = windowSize.width();
    height = windowSize.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 100;
    move(x, y);
}
