#include <stdio.h>
#include <stdlib.h>
struct node
{
    int from;
    int to;
    int fromcoin;
    int tocoin;
    struct node*next;
};

struct node*stack;
int arr[8][8];
int pawnarr[7][7];
int rookarr[7][7];
int bishoparr[7][7];
int knight[7][7];
int queenarr[7][7];
int kingarr[7][7];
int bstack[16],wstack[16],t1=-1,t2=-1;
int bkingpos=04,wkingpos=74;

void rook();
void queen();
void bishop();
void printemptyrow();
void displaycoinboard1();
void displaydeleted();
void pawnchar();
void printwhite();
void knight1();
void king();
void printline();
void displayrow();
void coinarr();
void displayboard();
void numtochar();
void chessboard();

void push(int from,int to)
{
    struct node*new=(struct node*)malloc(sizeof(struct node));
    new->from=from;
    new->to=to;
    new->fromcoin=arr[from/10][from%10];
    new->tocoin=arr[to/10][to%10];
    if(stack==NULL)
    {
        stack=new;
        stack->next=NULL;
    }
    else
    {
        new->next=stack;
        stack=new;
    }
}

struct node* pop()
{
    if(stack==NULL)
    {
        return NULL;
    }
    else
    {
        struct node*temp=stack;
        stack=stack->next;
        return temp;
    }
}

void delpush(int color,int coin)
{
    if(color==1)
    {
        t1++;
        wstack[t1]=coin;
    }
    else
    {
        t2++;
        bstack[t2]=coin;
    }
}

void delpop(int color)
{
    if(color==1)
    {
        t1--;
    }
    else if(color==2)
    {
        t2--;
    }
}

int straightleftmv(int coin)
{
    if(coin%10!=5 && coin%10!=8)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int pawnmv(int start,int dest)
{
    int coin=arr[start/10][start%10],diff=start/10-dest/10;
    if(coin/10==1 && start%10==dest%10 && diff>0)
    {
        if(arr[dest/10][dest%10]==0 && start/10==6  && (diff==2 || diff==1))
        {
            return 1;
        }
        else if(diff==1 && arr[dest/10][dest%10]==0)
        {
            return 1;
        }
        return 0;
    }
    else if(coin/10==1 && diff==1 && !(start%10==dest%10))
    {
        if(arr[dest/10][dest%10]/10==2)
        {
            return 1;
        }
        return 0;
    }
    if(coin/10==2 && start%10==dest%10 && diff<0)
    {
        if(arr[dest/10][dest%10]==0 && start/10==1  && (diff==-2 || diff==-1))
        {
            return 1;
        }
        else if(diff==-1 && arr[dest/10][dest%10]==0)
        {
            return 1;
        }
        return 0;
    }
    else if(coin/10==2 && diff==-1 && !(start%10==dest%10))
    {
        if(arr[dest/10][dest%10]/10==1)
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

int validpos(int row,int column)
{
    if(row>=0 && row<8 && column<8 && column>=0)
    {
        return 1;
    }
    return 0;
}

int stepslimit(int start,int destination)
{
    int coin=arr[start/10][start%10];
    if(coin%10==6)
    {
        return pawnmv(start,destination);
    }
    else if(coin%10==3)
    {
        return  (start/10-destination/10)==1 || (start/10-destination/10)==-1 ||
        (start%10-destination%10)==1 || (start%10-destination%10)==-1;
    }
    else
    {
        return 1;
    }
    
}

int slpathcheck(int start,int dest)
{
    if(start > dest)
    {
        int tmp=start;
        start=dest;
        dest=tmp;
    }
    int f=1;
    //row mv
    if(start%10==dest%10)
    {
        int j=start%10;
        for(int i=start/10+1;i<dest/10;i++)
        {
            if(arr[i][j]!=0)
            {
                f=0;
            }
        }
    }
    else if(start/10==dest/10)
    {
        int j=start/10;
        for(int i=start%10+1;i<dest%10;i++)
        {
            if(arr[j][i]!=0)
            {
                f=0;
            }
        }
    }
    return f;
}

int crosspath(int start,int destination)
{
    if(start < destination)
    {
        int t=start;
        start=destination;
        destination=t;
    }
    int f=1,diff=start/10-destination/10;
    if(start/10 > destination/10 && start%10 < destination%10)
    {
        int pos=start;
        for(int i=1;i<diff;i++)
        {
            pos=((pos/10)-1)*10+((pos%10)+1);
            if(arr[pos/10][pos%10]!=0)
            {
                f=0;
                break;
            }
        }
    }
    else if(start/10 > destination/10 && start%10 > destination%10)
    {
        int pos=start;
        for(int i=1;i<diff;i++)
        {
            pos=((pos/10)-1)*10+((pos%10)-1);
            if(arr[pos/10][pos%10]!=0)
            {
                f=0;
                break;
            }
        }
    }
    return f;
}

int iscrossmv(int x,int y)
{
    if(x < y)
    {
        int t=x;
        x=y;
        y=t;
    }
    int pos=x,f=0,diff=x/10-y/10;
    if(x/10>y/10 && x%10 < y%10)
    {
        pos=((pos/10)-diff)*10+((pos%10)+diff);
        if(pos==y)
        {
            f=1;
        }
    }
    else if(x/10>y/10 && x%10 > y%10)
    {
        pos=((pos/10)-diff)*10+((pos%10-diff));
        if(pos==y)
        {
            f=1;
        }
    }
    return f;
}

int crossmv(int coin)
{
   if(coin%10!=5 && coin%10!=7)
    {
        return 1;
    }
    else
    {
        return 0;
    } 
}

int isknight(int coin)
{
    if(coin%10==5)
    {
        return 1;
    }
    else
    {
        return 0;
    } 
}

int isknightmv(int x,int y)
{
    if( (x/10+2)*10+(x%10+1)==y || (x/10+1)*10+(x%10+2)==y ||
        (x/10-2)*10+(x%10+1)==y || (x/10-1)*10+(x%10+2)==y ||
        (x/10-2)*10+(x%10-1)==y || (x/10-1)*10+(x%10-2)==y ||
        (x/10+2)*10+(x%10-1)==y || (x/10+1)*10+(x%10-2)==y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int movedirection(int x,int y)
{
    if(x/10==y/10 || x%10==y%10)
    {
        if(straightleftmv(arr[x/10][x%10]))
        {
            return slpathcheck(x,y);
        }
        else
        {
            return 0;
        }
    }
    else if(iscrossmv(x,y))
    {
        if(crossmv(arr[x/10][x%10]))
        {
            return crosspath(x,y);
        }
        else
        {
            return 0;
        }
    }
    else if(isknightmv(x,y))
    {
        return isknight(arr[x/10][x%10]);
    }
    else
    {
        return 0;
    }
}

int ischeck(int kingclr,int square)
{

    int x=square/10,y=square%10;
    for(int i=square/10-1;i>=0;i--)
    {
        if(arr[i][y]/10==kingclr)
        {
           break;
        }
        else if(arr[i][y]!=0 && stepslimit(i*10+y,square) && 
        straightleftmv(arr[i][y]))
        {
            return 1;
        }
    }
    for(int i=square/10+1;i<=7;i++)
    {
        if(arr[i][y]/10==kingclr)
        {
            break;
        }
        else if(arr[i][y]!=0 && stepslimit(i*10+y,square) &&
        straightleftmv(arr[i][y]))
        {
            return 1;
        }
    }
    for(int i=square%10+1;i<=7;i++)
    {
        if(arr[x][i]/10==kingclr)
        {
            break;
        }
        else if(arr[x][i]!=0 && stepslimit(x*10+i,square) &&
        straightleftmv(arr[x][i]))
        {
            return 1;
        }
    }
    for(int i=square%10-1;i>=0;i--)
    {
        if(arr[x][i]/10==kingclr)
        {
            break;
        }
        else if(arr[x][i]!=0 && stepslimit(x*10+i,square) &&
        straightleftmv(arr[x][i]))
        {
            return 1;
        }
    }

    int min,max;
    if(x<y)
    {
        min=x;
        max=y;
    }
    else
    {
        min=y;
        max=x;
    }
    int pos=square;
    for(int i=0;i<min;i++)
    {
        pos=((pos/10)-1)*10+((pos%10)-1);
        if(arr[pos/10][pos%10]/10==kingclr)
        {
            break;
        }
        else if(arr[pos/10][pos%10]!=0 && crossmv(arr[pos/10][pos%10]) &&
                stepslimit(pos,square))
        {
            return 1;
        }
    }
    //south east mv
    pos=square;
    for(int i=0;i<(7-max);i++)
    {
        pos=((pos/10)+1)*10+((pos%10)+1);
        if(arr[pos/10][pos%10]/10==kingclr)
        {
            break;
        }
        else if(arr[pos/10][pos%10]!=0 && crossmv(arr[pos/10][pos%10]) &&
             stepslimit(pos,square))
        {
            return 1;
        }
    }
    //north east
    int steps;
    if((x+y)<7)
    {
        steps=x;
    }
    else
    {
        steps=7-y;
    }
    pos=square;
    for(int i=0;i<steps;i++)
    {
        pos=((pos/10)-1)*10+((pos%10)+1);
        if(arr[pos/10][pos%10]/10==kingclr)
        {       
            break;
        }
        else if(arr[pos/10][pos%10]!=0  && crossmv(arr[pos/10][pos%10]) &&
             stepslimit(pos,square))
        {
            return 1;
        }
    }
    //south west
    if((x+y)<7)
    {
        steps=y;
    }
    else
    {
        steps=7-x;
    }
    pos=square;
    for(int i=0;i<steps;i++)
    {
        pos=((pos/10)+1)*10+((pos%10)-1);
        if(arr[pos/10][pos%10]/10==kingclr)
        {
            break;
        }
        else if(arr[pos/10][pos%10]!=0 && crossmv(arr[pos/10][pos%10]) &&
             stepslimit(pos,square))
        {
            return 1;
        }
    }
    int opcolor;
    if(kingclr==1)
    {
        opcolor=2;
    }
    else
    {
        opcolor=1;
    }
    if( arr[(square/10+2)][(square%10+1)]==opcolor*10+5 ||
        arr[(square/10+1)][(square%10+2)]==opcolor*10+5 ||
        arr[(square/10-2)][(square%10+1)]==opcolor*10+5 || 
        arr[(square/10-1)][(square%10+2)]==opcolor*10+5 ||
        arr[(square/10-2)][(square%10-1)]==opcolor*10+5 || 
        arr[(square/10-1)][(square%10-2)]==opcolor*10+5 ||
        arr[(square/10+2)][(square%10-1)]==opcolor*10+5 || 
        arr[(square/10+1)][(square%10-2)]==opcolor*10+5)
    {
        return 1;
    }
    return 0;
}

int move(int x,int y)
{
    if(x==y)
    {
        return 0;
    }
    else if(movedirection(x,y) && stepslimit(x,y))
    {
        if(arr[y/10][y%10]/10!=arr[x/10][x%10]/10)
        {
            push(x,y);
            if(arr[y/10][y%10]!=0)
            {
                delpush(arr[y/10][y%10]/10,arr[y/10][y%10]);
            }
            arr[y/10][y%10]=arr[x/10][x%10];
            arr[x/10][x%10]=0;
            return 1;
        }
    }
    return 0;
}

int undo()
{
    if(stack!=NULL)
    {
        struct node*temp=pop();
        if(arr[temp->to/10][temp->to%10]!=0)
        {
            delpop(temp->tocoin/10);
        }
        arr[temp->from/10][temp->from%10]=temp->fromcoin;
        arr[temp->to/10][temp->to%10]=temp->tocoin;
        if(temp->fromcoin==13)
        {
            wkingpos=temp->from;
        }  
        else if(temp->fromcoin==23)
        {
            bkingpos=temp->from;
        }   
        free(temp);
        displayboard();
        return 1;
    }
    else
    {
        return 0;
    }
}

void replacepawn(int pos)
{
    int rep;
    while(1)
    {
        printf("4.queen,5,knight,7.rook,8.bishop ");
        scanf("%d",&rep);
        if(rep==4 || rep==5 || rep==7 || rep==8)
        {
            arr[pos/10][pos%10]=(arr[pos/10][pos%10]/10)*10+rep;
            break;
        }
    }
}

void construct()
{
    chessboard();
    queen();
    king();
    knight1();
    pawnchar();
    rook();
    bishop();
}

int main()
{
    construct();
    displayboard();
    int ch=1,white=1;
    int from,to;
    while(ch)
    {
        if(white==1)
        {
            printf("whites move ");
            scanf("%d %d",&from,&to);
            if(!(validpos(from/10,from%10) && validpos(to/10,to%10)))
            {
                continue;
            }
            if(from==3 && to==3)
            {
                if(undo())
                {
                    white=0;
                    continue;
                }
            }
            if(arr[from/10][from%10]%10==3 && ischeck(arr[from/10][from%10]/10,to))
            {
                continue;
            }
            if(arr[to/10][to%10]/10==1 || !move(from,to))
            {
                printf("wrong move\n");
                continue;
            }
            else
            {
                if(arr[to/10][to%10]==13)
                {
                    wkingpos=to;
                }
                if(ischeck(1,wkingpos))
                {
                    undo();
                    continue;
                }
                else if(arr[to/10][to%10]==16 && to/10==0)
                {
                    replacepawn(to);
                }
            }
            white=0;
        }
        else
        {
            printf("black move ");
            scanf("%d %d",&from,&to);
            if(!(validpos(from/10,from%10) && validpos(to/10,to%10)))
            {
                continue;
            }
            if(from==3 && to==3)
            {
                if(undo())
                {
                    white=1;
                    continue;
                }

            }
            if(arr[from/10][from%10]%10==3 && ischeck(arr[from/10][from%10]/10,to))
            {
                continue;
            }
            else if(arr[to/10][to%10]/10==2 || !move(from,to))
            {
                printf("wrong move\n");
                continue;
            }
            else
            {
                if(arr[to/10][to%10]==23)
                {
                    bkingpos=to;
                }
                if(ischeck(2,bkingpos))
                {
                    undo();
                    continue;
                }
                else if(arr[to/10][to%10]==26 && to/10==7)
                {
                    replacepawn(to);
                }
            }
            white=1;
        }
        displayboard();
    }
    return 0;
}
void printemptyrow()
{
    for(int i=0;i<=12;i++)
    {
        printf(" ");
    }
}
void printwhite()
{
    for(int i=0;i<=12;i++)
    {
        printf(":");
    }
}

void printline()
{
    for(int i=0;i<=7;i++)
    {
        printf("______________");
    }
    printf("\n");
}

void displayrow(int arr[][7],int row,int color)
{

    for(int j=0;j<7;j++)
    {
        if(color==1)
        {
            if(arr[row][j]==0)
            {
                printf(" ");
            }
            else
            {
                printf("0");
            }  
        }
        else
        {
            if(arr[row][j]==0)
            {
                printf(" ");
            }
            else
            {
                printf("*");
            }  
        }
    }
}

void coinarr(int x,int row,int color)
{
    if(x%10==6)
    {
       displayrow(pawnarr,row,color);
    }
    else if(x%10==7)
    {
        displayrow(rookarr,row,color);  
    }
    else if(x%10==8)
    {
        displayrow(bishoparr,row,color);  
    }
    else if(x%10==5)
    {
        displayrow(knight,row,color);  
    }
    else if(x%10==3)
    {
        displayrow(kingarr,row,color);  
    }
    else if(x%10==4)
    {
        displayrow(queenarr,row,color);  
    }
}

void displayboard()
{
    displaydeleted(bstack,t2);
    printf("\t  ");
    for(int i=0;i<8;i++)
    {
        printf("     %d        ",i);
    }
    printf("\n");
    for(int i=0;i<=7;i++)
    {
        printf("\t");
        printline();
        for(int k=0;k<=6;k++)
        {
            if(k==3)
            {
                printf("%d ",i);
            }+

            printf("\t|");        
            for(int j=0;j<=7;j++)
            {
                if(arr[i][j]!=0)
                {
                    printf("   ");
                    coinarr(arr[i][j],k,arr[i][j]/10);
                    printf("   |");
                }
                else if(j%2==0 && i%2==0 || 
                j%2!=0 && i%2!=0)
                {
                    printwhite();
                    printf("|");

                }
                else
                {
                    printemptyrow();
                    printf("|");
                }
            }
            printf("\n");
        }
    }
    printf("\t");
    printline();
    printf("\n");
    displaydeleted(wstack,t1);
}

void numtochar(int num)
{
    if(num==1)
    {
        printf("w");
    }
    if(num==2)
    {
        printf("b");
    }
    if(num==3)
    {
        printf("king");
    }
    if(num==4)
    {
        printf("queen");
    }
    if(num==5)
    {
        printf("knight");
    }
    if(num==6)
    {
        printf("pawn");
    }
    if(num==7)
    {
        printf("rook");
    }
    if(num==8)
    {
        printf("bishop");
    }
}

void displaydeleted(int *arr,int t)
{
    printf("\t");
    for(int i=0;i<=t;i++)
    {
        numtochar(arr[i]/10);
        numtochar(arr[i]%10);
        printf(" ");
    }
    printf("\n");
}

void chessboard()
{
   arr[0][0]=27,arr[0][7]=27,arr[0][1]=25,arr[0][6]=25,arr[0][2]=28,arr[0][5]=28,arr[0][3]=24,arr[0][4]=23;
    arr[7][0]=17,arr[7][7]=17,arr[7][1]=15,arr[7][6]=15,arr[7][2]=18,arr[7][5]=18,arr[7][3]=14,arr[7][4]=13;
    for(int i=0;i<=7;i++)
    {
        for(int j=0;j<=7;j++)
        {
            if(i==1)
            {
                arr[i][j]=26;
            }
            else if(i==6)
            {
                arr[i][j]=16;
            }
        }
    }

}

void pawnchar()
{
pawnarr[0][0]=pawnarr[0][1]=pawnarr[0][2]=pawnarr[0][3]=pawnarr[0][4]=pawnarr[0][5]=pawnarr[0][6]=0;
pawnarr[1][0]=pawnarr[1][1]=0,pawnarr[1][2]=1,pawnarr[1][3]=0,pawnarr[1][4]=1,pawnarr[1][5]=pawnarr[1][6]=0;
pawnarr[2][0]=0,pawnarr[2][1]=1,pawnarr[2][2]=0,pawnarr[2][3]=1,pawnarr[2][4]=0,pawnarr[2][5]=1,pawnarr[2][6]=0;
pawnarr[3][0]=0,pawnarr[3][1]=0,pawnarr[3][2]=1,pawnarr[3][3]=0,pawnarr[3][4]=1,pawnarr[3][5]=0,pawnarr[3][6]=0;
pawnarr[4][0]=0,pawnarr[4][1]=0,pawnarr[4][2]=1,pawnarr[4][3]=1,pawnarr[4][4]=1,pawnarr[4][5]=0,pawnarr[4][6]=0;
pawnarr[5][0]=0,pawnarr[5][1]=1,pawnarr[5][2]=1,pawnarr[5][3]=1,pawnarr[5][4]=1,pawnarr[5][5]=1,pawnarr[5][6]=0;
pawnarr[6][0]=1,pawnarr[6][1]=1,pawnarr[6][2]=1,pawnarr[6][3]=1,pawnarr[6][4]=1,pawnarr[6][5]=1,pawnarr[6][6]=1;
}

void bishop()
{
bishoparr[0][0]=0,bishoparr[0][1]=0,bishoparr[0][2]=0,bishoparr[0][3]=1,bishoparr[0][4]=bishoparr[0][5]=bishoparr[0][6]=0;
bishoparr[1][0]=0,bishoparr[1][1]=1,bishoparr[1][2]=1,bishoparr[1][3]=0,bishoparr[1][4]=1,bishoparr[1][5]=1,bishoparr[1][6]=0;
bishoparr[2][0]=0,bishoparr[2][1]=0,bishoparr[2][2]=1,bishoparr[2][3]=1,bishoparr[2][4]=1,bishoparr[2][5]=0,bishoparr[2][6]=0;
bishoparr[3][0]=0,bishoparr[3][1]=0,bishoparr[3][2]=1,bishoparr[3][3]=1,bishoparr[3][4]=1,bishoparr[3][5]=0,bishoparr[3][6]=0;
bishoparr[4][0]=0,bishoparr[4][1]=0,bishoparr[4][2]=1,bishoparr[4][3]=1,bishoparr[4][4]=1,bishoparr[4][5]=0,bishoparr[4][6]=0;
bishoparr[5][0]=0,bishoparr[5][1]=1,bishoparr[5][2]=1,bishoparr[5][3]=1,bishoparr[5][4]=1,bishoparr[5][5]=1,bishoparr[5][6]=0;
bishoparr[6][0]=1,bishoparr[6][1]=1,bishoparr[6][2]=1,bishoparr[6][3]=1,bishoparr[6][4]=1,bishoparr[6][5]=1,bishoparr[6][6]=1;
}

void knight1()
{
knight[0][0]=0,knight[0][1]=0,knight[0][2]=1,knight[0][3]=1,knight[0][4]=knight[0][5]=1,knight[0][6]=0;
knight[1][0]=1,knight[1][1]=1,knight[1][2]=1,knight[1][3]=1,knight[1][4]=0,knight[1][5]=1,knight[1][6]=1;
knight[2][0]=1,knight[2][1]=1,knight[2][2]=1,knight[2][3]=1,knight[2][4]=1,knight[2][5]=1,knight[2][6]=1;
knight[3][0]=0,knight[3][1]=0,knight[3][2]=0,knight[3][3]=0,knight[3][4]=1,knight[3][5]=1,knight[3][6]=1;
knight[4][0]=0,knight[4][1]=0,knight[4][2]=0,knight[4][3]=1,knight[4][4]=1,knight[4][5]=1,knight[4][6]=0;
knight[5][0]=0,knight[5][1]=0,knight[5][2]=1,knight[5][3]=1,knight[5][4]=1,knight[5][5]=1,knight[5][6]=0;
knight[6][0]=1,knight[6][1]=1,knight[6][2]=1,knight[6][3]=1,knight[6][4]=1,knight[6][5]=1,knight[6][6]=1; 
}

void queen()
{
queenarr[0][0]=1,queenarr[0][1]=0,queenarr[0][2]=0,queenarr[0][3]=1,queenarr[0][4]=queenarr[0][5]=0,queenarr[0][6]=1;
queenarr[1][0]=0,queenarr[1][1]=1,queenarr[1][2]=0,queenarr[1][3]=1,queenarr[1][4]=0,queenarr[1][5]=1,queenarr[1][6]=0;
queenarr[2][0]=1,queenarr[2][1]=1,queenarr[2][2]=1,queenarr[2][3]=1,queenarr[2][4]=1,queenarr[2][5]=1,queenarr[2][6]=1;
queenarr[3][0]=0,queenarr[3][1]=0,queenarr[3][2]=1,queenarr[3][3]=1,queenarr[3][4]=1,queenarr[3][5]=0,queenarr[3][6]=0;
queenarr[4][0]=0,queenarr[4][1]=0,queenarr[4][2]=1,queenarr[4][3]=1,queenarr[4][4]=1,queenarr[4][5]=0,queenarr[4][6]=0;
queenarr[5][0]=0,queenarr[5][1]=1,queenarr[5][2]=1,queenarr[5][3]=1,queenarr[5][4]=1,queenarr[5][5]=1,queenarr[5][6]=0;
queenarr[6][0]=1,queenarr[6][1]=1,queenarr[6][2]=1,queenarr[6][3]=1,queenarr[6][4]=1,queenarr[6][5]=1,queenarr[6][6]=1;
}

void king()
{
kingarr[0][0]=kingarr[0][1]=0,kingarr[0][2]=0,kingarr[0][3]=1,kingarr[0][4]=kingarr[0][5]=kingarr[0][6]=0;
kingarr[1][0]=0,kingarr[1][1]=1,kingarr[1][2]=1,kingarr[1][3]=1,kingarr[1][4]=1,kingarr[1][5]=1,kingarr[1][6]=0;
kingarr[2][0]=0,kingarr[2][1]=0,kingarr[2][2]=0,kingarr[2][3]=1,kingarr[2][4]=0,kingarr[2][5]=0,kingarr[2][6]=0;
kingarr[3][0]=1,kingarr[3][1]=1,kingarr[3][2]=1,kingarr[3][3]=1,kingarr[3][4]=1,kingarr[3][5]=1,kingarr[3][6]=1;
kingarr[4][0]=0,kingarr[4][1]=0,kingarr[4][2]=1,kingarr[4][3]=1,kingarr[4][4]=1,kingarr[4][5]=0,kingarr[4][6]=0;
kingarr[5][0]=0,kingarr[5][1]=1,kingarr[5][2]=1,kingarr[5][3]=1,kingarr[5][4]=1,kingarr[5][5]=1,kingarr[5][6]=0;
kingarr[6][0]=1,kingarr[6][1]=1,kingarr[6][2]=1,kingarr[6][3]=1,kingarr[6][4]=1,kingarr[6][5]=1,kingarr[6][6]=1;
}

void rook()
{
    for(int i=0;i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(i==0 || i==6)
            {
                rookarr[i][j]=1;
            }
            else
            {
                if(j==0 || j==6)
                {
                    rookarr[i][j]=0;
                }
                else
                {
                    rookarr[i][j]=1;
                }
            }
        }
        printf("\n");
    }
}

