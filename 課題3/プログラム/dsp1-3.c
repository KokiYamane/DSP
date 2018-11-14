//H30 DSP4 4J41山根

#include <stdio.h>
#include <math.h>

#define DATASIZE 86

void fileOpen(double data[DATASIZE],char *filename)
{
	//ファイルオープン
	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("can't open tha file.");
		exit(1);
	}
	
	//ファイル読み込み
	for(int i=0;i<DATASIZE;i++){
		fscanf(fp,"%lf",&data[i]);
	}
	
	fclose(fp);
}

double average(double data[DATASIZE])
{
	int sum=0;
	double ans=0;
	for(int i=0;i<DATASIZE;i++){
		sum+=data[i];
		
	}
	ans=sum/DATASIZE;
	return ans;
}

void normalization(double data[DATASIZE])
{
	double norm=0;
	for(int i=0;i<DATASIZE;i++){
		norm+=pow(data[i],2);
	}
	norm=sqrt(norm);
	for(int i=0;i<DATASIZE;i++){
		data[i]/=norm;
	}
}

double innerProduct(double vector1[DATASIZE],double vector2[DATASIZE])
{
	double ans=0;
	for(int i=0;i<DATASIZE;i++){
		ans+=vector1[i]*vector2[i];
	}
	return ans;
}

int main()
{
	printf("H30 s15198 4J41 山根\n\n");
	
	double data[4][DATASIZE];
	
	fileOpen(data[0],"rdata1.txt");
	fileOpen(data[1],"rdata2.txt");
	fileOpen(data[2],"rdata3.txt");
	fileOpen(data[3],"rdata4.txt");
	
	for(int i=0;i<DATASIZE;i++){
		data[0][i]-=average(data[0]);
		data[1][i]-=average(data[1]);
		data[2][i]-=average(data[2]);
		data[3][i]-=average(data[3]);
	}

	normalization(data[0]);
	normalization(data[1]);
	normalization(data[2]);
	normalization(data[3]);
	
	printf("H16男性・女性   = %f\n",innerProduct(data[0],data[1]));
	printf("H16男性・総人口 = %f\n",innerProduct(data[0],data[2]));
	printf("H16男性・S24    = %f\n",innerProduct(data[0],data[3]));
}
