#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include<time.h>
#include<omp.h>
using namespace std;

int distance_calculate_cluster1(int x1, int y1, int x2, int y2)// used to calculate the distance between the centroid of the first clusters and the upcoming co-ordinates set
{
	int distance;
	distance=sqrt(pow((x2-x1),2)+pow((y2-y1),2));
	return distance;
}

int distance_calculate_cluster2(int x1, int y1, int x2, int y2) // used to calculate the distance between the centroid of the second clusters and the upcoming co-ordinates set
{
	int distance1;
	distance1=sqrt(pow((x2-x1),2)+pow((y2-y1),2));
	return distance1;
}



int main ()
{
  int nthread,tid;
  int no_of_clusters=2,select;
  int cluster_dataset[no_of_clusters][no_of_clusters];
  int user_input[1][1];
  string line;
  ifstream myfile ("weather_database2.csv");// used for accessing the dataset that is used for extracting the data upon which the k-means algorithm works upon
  int pos,d,f=0,i=0,k=0,p=0,z=0,v=0;
  string array[8000];
  string array1[8000];
  int temp_min[8000];// used for storing the data of the minimum temperature column which is used for further uses
  int temp_max[8000]; // used for storing the data of the maximum temperature column which is used for further uses
  #pragma omp parallel private(nthread,tid)
  {
  	float start_time1=omp_get_wtime();
  	nthread=omp_get_num_threads();
	tid=omp_get_thread_num();
	if(tid==1)
	{
	  		//cout<<"Enter the Number of Clusters you want to make\n";
	  		//cin>> no_of_clusters;// for now the number of clusters is reduced to 2 in number
	  		//int cluster_dataset[no_of_clusters][no_of_clusters];
	  		cout<<"***************Select a number to dispaly the otput of the clustering data*****************"<<'\n';
	  		cout<<"1. Cluster 1 Data "<<'\n';
	  		cout<<"2. Cluster 2 Data "<<'\n';
	  		cout<<"Enter your choice 1 or 2"<<'\n';
	  		cin>>select;

    }

	if(tid==0)
	{
	  	float start_time=omp_get_wtime();
	  	if (myfile.is_open())
	  	{
	    		while ( getline (myfile,line) )
	    		{
	    	 		pos=line.find(",");
                    string sub = line.substr(pos+1);// used for identifying the ',' delimiter which separates the csv file and used for substringing the section
                    string sub1 = line.substr(f,pos);
                    array[k]=sub;// adding to the temporary array which is to be swapped later to the temp_max array
                    array1[p]=sub1; // adding to the temporary array which is to be swapped later to the temp_min array
                    k++;
                    p++;
                }
                for(int l=0;l<k;l++)
                {
                    stringstream convert(array[l]);//convert char to int
                    convert>>d;
                    temp_max[z]=d;// adding the string->int values to the temp_max array
                    z=z+1;
                }

                for(int j=0;j<p;j++)
                {

                    stringstream convert(array1[j]);//convert char to int
                    convert>>d;
                    temp_min[v]=d; // // adding the string->int values to the temp_min array
                    v=v+1;
                }

                int temp_data [z][2];
                int cluster1  [z][2];
                int cluster2  [z][2];
                for(int i=0;i<z;i++)
                {
                    temp_data[i][0]=temp_min[i];
                    temp_data[i][1]=temp_max[i];
                }
                for(int i=0;i<no_of_clusters;i++)
                {
                    for(int j=0;j<no_of_clusters;j++)
                    {
                        cluster_dataset[i][j]=temp_data[i][j];// used for storing the first and second data in a temprary array which 					is to be swapped later
                    }
                }

	  		cluster1[0][0]=cluster_dataset[0][0];// used for shifting the first and the second data of the entire database to the first and 		second cluster respectively
			cluster1[0][1]=cluster_dataset[0][1];
			cluster2[0][0]=cluster_dataset[1][0];
			cluster2[0][1]=cluster_dataset[1][1];

			int counter=1;
			int counter1=1;
			cout<<"************The Eucledian distance for the next set of co-ordinates from centroid*************"<<'\n';// For the purpose 		of declaring the eucledian distance so that cross-checking might be easier

			for(int i=2;i<z;i++)
			{
				int a= temp_data[i][0];//  next-co-ordinate(temperature_minimum)
				int b= temp_data[i][1];// next co-ordinate(temperature_maximum)
				int c= cluster1[0][0];// cluster 1 co-ordinates // or the original centroid co-ordinates of cluster 1
				int d= cluster1[0][1];// cluster 1 co-ordinates // or the original centroid co-ordinates of cluster 1
				int e= cluster2[0][0];// cluster 2 co-ordinates // or the original centroid co-ordinates of cluster 2
				int g= cluster2[0][1];// cluster 2 co-ordinates // or the original centroid co-ordinates of cluster 2

				int calc_distance= distance_calculate_cluster1(a,b,c,d);// calculate distance from cluster 1 centroid
				int calc_distance1= distance_calculate_cluster2(a,b,e,g);// calculate distance from cluster 2 centroid

				//cout<<calc_distance<<'\t';
				//cout<<calc_distance1;

				if(calc_distance<calc_distance1)
				{
					cluster1[counter][0]=a;// assigning the new co-ordinates to the first cluster
					cluster1[counter][1]=b;
					//cout<<cluster1[counter][0]<<'\t';
					//cout<<cluster1[counter][1];
					counter++;
					cluster1[0][0]=(a+c)/2;//updating the centroid value so that the updated centroid value is used the 						next time
					cluster1[0][1]=(b+d)/2;

				}
				if(calc_distance1<calc_distance)
				{
					cluster2[counter1][0]=a; // assigning the new co-ordinates to the second cluster
					cluster2[counter1][1]=b;
					//cout<<cluster2[counter1][0]<<'\t';
					//cout<<cluster2[counter1][1];
					counter1++;
					cluster2[0][0]=(a+e)/2;// updating the centroid value so that the updated value is used next time
					cluster2[0][1]=(b+g)/2;

				}

				//cout<<'\n';
					cout<<"*****************************Dataset of Cluster 1*************************************"<<'\n';
					for(int i=0;i<counter;i++)
					{
						for(int j=0;j<2;j++)
						{
							cout<<cluster1[i][j]<<'\t';// Displaying the various (temp_min,temp_max) co-ordinates that were 						pushed to cluster 1
						}
						cout<<'\n';
					}
					//printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC)

                    cout<<"******************************Dataset Of Cluster 2************************************"<<'\n';
					for(int i=1;i<counter1;i++)
					{
						for(int j=0;j<2;j++)
						{
							cout<<cluster2[i][j]<<'\t'; // Displaying the various (temp_min,temp_max) co-ordinates that 							were pushed to cluster 2
						}
						cout<<'\n';
					}
					//printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
			}

			cout<<"enter the Minimum temperature you want to add to the cluster"<<'\n';
	  		cin>>user_input[0][0];
	  		cout<<user_input[0][0]<<'\n';
	  		cout<<"enter the Maximum Temperature you want to add to the cluster"<<'\n';
	  		cin>>user_input[0][1];
	  		cout<<user_input[0][1]<<'\n';

			int a=user_input[0][0];
			int b=user_input[0][1];
			int c= cluster1[0][0];// cluster 1 co-ordinates // or the original centroid co-ordinates of cluster 1
            int d= cluster1[0][1];// cluster 1 co-ordinates // or the original centroid co-ordinates of cluster 1
            int e= cluster2[0][0];// cluster 2 co-ordinates // or the original centroid co-ordinates of cluster 2
            int g= cluster2[0][1];

            int calc_distance= distance_calculate_cluster1(a,b,c,d);// calculate distance from cluster 1 centroid
            int calc_distance1= distance_calculate_cluster2(a,b,e,g);

            if(calc_distance<calc_distance1)
            {
					cluster1[counter][0]=a;// assigning the new co-ordinates to the first cluster
					cluster1[counter][1]=b;
					//cout<<cluster1[counter][0]<<'\t';
					//cout<<cluster1[counter][1];
					counter++;
					cluster1[0][0]=(a+c)/2;//updating the centroid value so that the updated centroid value is used the 						next time
					cluster1[0][1]=(b+d)/2;

					cout<<"The input temperature variables"<<a<<"(min temp) and"<<b<<"(max temp) is included into Cluster 1"<<'\n';
                    float end_time = omp_get_wtime();
                    cout<<"The time taken for the execution of the thread is"<<end_time-start_time<<'\n';
            }
            if(calc_distance1<calc_distance)
            {
					cluster2[counter1][0]=a; // assigning the new co-ordinates to the second cluster
					cluster2[counter1][1]=b;
					//cout<<cluster2[counter1][0]<<'\t';
					//cout<<cluster2[counter1][1];
					counter1++;
					cluster2[0][0]=(a+e)/2;// updating the centroid value so that the updated value is used next time
					cluster2[0][1]=(b+g)/2;

					cout<<"The input temperature variables"<<a<<"(min temp) and"<<b<<"(max temp) is included into Cluster 2 "<<'\n';
                    float end_time = omp_get_wtime();
                    cout<<"The time taken for the execution of the thread is"<<end_time-start_time<<'\n';
				}

            myfile.close();

        }
    }
    float end_time1=omp_get_wtime();
    cout<<"The time taken for the execution of the entire parallel region is"<<end_time1-start_time1<<'\n';
  }
  return 0;
}
