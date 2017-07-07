//Solving Open Vehicle Routing Problem Using IWD Algorithm
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define time(a,b) matrix[a][b]/iwd[i].vel
#define soil(i,j) soil[i][j]
//Global variable and Function Declaration
float probability(int,int,int);
void route(int);
void iwd_algo();
void mainalgo();
void distribute(int);
void local_update(int,int,int);
int global_update(int,int);
float matrix[150][150];
float x[150];
float y[150];
int city;
int node_count;
float total_distance=0;
float soil[150][150];
int global_visit[150];
int global_node[150];
int global_node_var=1;
int algoconst=0;
int visit;
int total_city;
float global_quality=9999999;
float quality();
float demand[150];
float resetvc;
float vehicle_capacity;
int infi=9999;
//Global variable and Function Declaration End

 struct iwd //Water Drop Structure
{
    float vel;
    float soil;
    int vc[150];
    float count;
    int no_visited_nodes;

};

struct iwd iwd[150];

int main()
{
	clock_t begin=clock();
	int i, j; //Loop Variables
	int z=1;  // Vehicle Count Variable
	int temp2=0;//Variable For Resetting cost Matrix
	int q=999999;// Variable For Quality Decision
	float matrix_aux[150][150]; // Auxiliary Matrix For Resetting Cost Matrix
    int temp;
    double time_spent;
    FILE *fp;
    fp=fopen("Test Data/test.txt","r");
    if(fp==NULL)
    {
        printf("Cannot Open File\n");
        getch();
        exit(1);
    }
    fscanf(fp,"%d %f",&city,&vehicle_capacity);
    total_city=city;
    for(i=0;i<total_city;i++)
    {
        fscanf(fp,"%f %f %f",&x[i],&y[i],&demand[i]);
    }

    printf("Total Cities= %d Vehicle capacity= %f\n",city,vehicle_capacity);
    for(i=1;i<150;i++)
    {
       global_node[i]=9999;
    }
    fclose(fp);
    for(i=0;i<total_city;i++)
    {
        for(j=0;j<total_city;j++)
        {
            if(i==j)
            {
                matrix[i][j]=0;
            }
            else
            {
                matrix[i][j]=sqrt( (x[i]-y[i])*(x[i]-y[i])+(x[j]-y[j])*(x[j]-y[j]));
            }
        }
    }
	node_count=city;
	total_city=city;
		for(i=0;i<total_city;i++)
        {

		for(j = 0; j < total_city; j++)
		{
			matrix_aux[i][j]=matrix[i][j];
		}
        }

    resetvc=vehicle_capacity;

	for(i=0;i<total_city-1;i++) //Initializing Dynamic Parameters For Water Drops.
    {
        iwd[i].vel=100;
        iwd[i].soil=0;
        iwd[i].count=0;
        iwd[i].no_visited_nodes=1;
        for(j=0;j<150;j++)
        {
            iwd[i].vc[j]=infi;
        }
    }
    for(i=0;i<total_city;i++)  //Initializing The Soil Content on Each Path
    {
        for(j=0;j<city;j++)
        {
            soil[i][j]=1000;
        }
    }
	printf("\nOptimal Path(s):\n");
	while(node_count!=1) // Algorithm Begins
    {

        global_quality=9999999;
        distribute(city);  //Allocating Water Drops to Each Nodes(Cities)
        for(i=0;i<city;i++)
            {
                iwd_algo();
                quality();

        for(i=0;i<city-1;i++) //reset water drops
            {
               iwd[i].vel=100;
                iwd[i].soil=0;
                iwd[i].count=0;
                iwd[i].no_visited_nodes=1;
                for(j=0;j<150;j++)
                    {
                        iwd[i].vc[j]=infi;
                    }
            }
            }


        for(i=0;i<=visit;i++) // Printing The Optimal Paths
        {

            printf("%d",global_visit[i]);
            if(i!=visit)
            {
                printf("->");
            }

        }
        printf("\nRoute Distance = %f\n",global_quality);
        total_distance=total_distance+global_quality;
         for(i=1;i<150;i++) //Code For Getting Subsequent Paths For Left Over Cities
        {
            if(global_visit[i]!=9999)
            {
                node_count--;
            }
        }
         for(i=1;i<150;i++)
         {
             if(global_visit[i]==9999)
             {
                 temp=i;
                 break;
             }
         }
         for(i=1;i<temp;i++)
         {
            global_node[global_node_var]=global_visit[i];
            global_node_var++;
         }
        for(i=1;i<150;i++)
        {
            if(global_node[i]==9999)
                {
                temp2=i-1;
                break;
                }
        }



    for(i=0;i<temp2;i++)
        {

        matrix_aux[global_node[i]][global_node[i+1]]=0;
        matrix_aux[global_node[i+1]][global_node[i]]=0;
        }




        for(i=0;i<total_city;i++)
        {
            for(j=0;j<total_city;j++)
            {
               matrix[i][j]=matrix_aux[i][j];
            }

        }

            printf("\n");



        city=node_count;
        visit=0; //Code For Getting Subsequent Paths For Left Over Cities End
        z++;     // Vehicle Count Variable
    }
    printf("\nTotal Vehicles Required = %d \n",z-1);
    printf("\nTotal Distance Covered = %f \n",total_distance);
    clock_t end=clock();
    time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("\nExecution Time = %f\n",time_spent);
    getch();
    return 0;
}

void distribute(int city) //Function For Distributing Water Drops On Nodes
{
    int flag=0;
    int k=1;int i,j;
    int aux[150];
    for(i=1;i<total_city;i++)
    {
        for(j=1;j<(total_city-node_count+1);j++)
            {
                if(i==global_node[j])
                    {
                        flag=1;
                        break;
                    }
                else
                    {
                        continue;
                    }
            }
        if(flag==0)
        {
            aux[k]=i;
            k++;
        }
        flag=0;
    }
    k=1;
    for(i=0;i<city-1;i++)
    {
        iwd[i].vc[0]=0;
        iwd[i].vc[1]=aux[k];
        k++;
    }
}

void iwd_algo() // Algorithm Function For Satisfying Constraints And Using Other Functions Involved
{
    int i;
    int time=0;
    int flag=0;
    for(i=0;i<city-1;i++)
        {
            while(algoconst<city)
                {
                    if(flag==0)
                        {
                            vehicle_capacity=vehicle_capacity-demand[i+1];
                            flag=1;
                        }
                route(i);
                algoconst++;
                }
            flag=0;
            algoconst=0;
            vehicle_capacity=resetvc;
        }
}
void route(int i) // Route Building Function For Water Drops
{
    int cn,nn;
    int t;
    int k=0;
    float temp=0;
    int fav_node;
    float fav=infi;
    for(t=2;t<city;t++)
    {
        if(iwd[i].vc[t]==infi)
            {
                t--;
                break;
            }
    }
    for(k=1;k<total_city;k++)
    {
        temp=probability(i,iwd[i].vc[t],k);
        if(temp<=fav)
            {
                fav=temp;
                fav_node=k;
            }
    }
    if(fav>=infi)
        {

        }
    else
        {
            iwd[i].vc[t+1]=fav_node;
            iwd[i].no_visited_nodes++;
            vehicle_capacity=vehicle_capacity-demand[fav_node];
            cn=iwd[i].vc[t];
            nn=iwd[i].vc[t+1];
            local_update(cn,nn,i);
        }
}
float probability(int i,int a,int b) // Probability Function For Selecting Next Node
{
    float time;float p;float temp=0;int hit=0;
    int var;float soil_curr,del_soil;
    for(var=0;var<city;var++)
        {
            if(b==iwd[i].vc[var])
                {
                    hit=1;
                    break;
                }
        }
    for(var=0;var<total_city;var++)
        {
            if(b==global_node[var])
                {
                    hit=1;
                    break;
                }
        }
    if(a==b||b==0||hit==1||vehicle_capacity<demand[b])
        {

            return infi;
        }
    else
        {
            time=matrix[a][b]/iwd[i].vel;
            del_soil=(1000/(0.01+time));
            for(var=1;var<city;var++)
                {
                    if(var==a)
                        {
                            continue;
                        }
                    else
                        temp=temp+soil(a,var);
                }
            soil_curr=soil(a,b)-del_soil;
            p=soil_curr/temp;
            return p;
        }
}

void local_update(int cn,int nn,int i) //Local Update Function
{
    float del_soil;
    float time;
    time=(matrix[cn][nn]/iwd[i].vel);
    iwd[i].vel=iwd[i].vel+(1000/(0.01+soil[cn][nn]));
    del_soil=(1000/(0.01+time));
    soil[cn][nn]=soil[nn][cn]=soil[cn][nn]-del_soil;
    iwd[i].soil=iwd[i].soil+del_soil;
    iwd[i].count=iwd[i].count+matrix[cn][nn];
}

float quality() // Quality Function To Test Quality of Solution
{
    int i;int x=1;
    int k,j;
    int temp,temp1;
    float tem;
    float q=9999999;
    for(i=0;i<city-1;i++)
    {
        if(iwd[i].no_visited_nodes>visit)
            visit=iwd[i].no_visited_nodes;
    }
    for(i=0;i<city-1;i++)
        {
            if(iwd[i].no_visited_nodes==visit)
                {

                    tem=iwd[i].count+matrix[0][i+1];

                    if(tem<q)
                        {
                            q=tem;

                            temp=i;
                        }

                }
        }


    for(i=0;i<150;i++)
        {

            if(iwd[temp].vc[i]==infi)
                {
                    temp1=i-1;
                    break;
                }
        }
    if(q<global_quality)
        {
            global_quality=q;

            global_update(temp1,temp);
        }

    return q;
}

int global_update(int temp1,int temp) // Global Update Function
{

    int i,j;float soil_new[150][150];
    for(i=1;i<temp1;i++)
        {
            soil_new[i][i+1]=soil[i][i+1];
            soil_new[i+1][i]=soil[i+1][i];
        }

    for(i=0;i<city;i++)
        {
            for(j=0;j<city;j++)
                {
                    soil[i][j]=soil_new[i][j];
                }
        }
    for(i=0;i<150;i++)
        {
            global_visit[i]=iwd[temp].vc[i];
        }

    return 0;
}




