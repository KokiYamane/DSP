//H30 DSP1-4 4J41山根

#include <stdio.h>
#include <math.h>

#define DATASIZE 701
#define AUTODATASIZE 71

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
	
	double data[3][DATASIZE]={{0}};
	double R[DATASIZE];
	
	//書き込み用ファイルオープン
	FILE *fp;
	fp=fopen("rdata.txt","w");
	if(fp==NULL){
		printf("can't open tha file.");
		exit(1);
	}
	
/********** 相互相関関数 **********/

	printf("相互相関関数\n");
	fileOpen(data[0],"wdata1.txt");
	fileOpen(data[1],"wdata2.txt");
	
	for(int m=0;m<DATASIZE;m++){
		for(int j=0;j<DATASIZE;j++){
			data[2][j]=data[1][j+m];
		}
		R[m]=innerProduct(data[0],data[2])/DATASIZE;
		fprintf(fp,"%f\n",R[m]);
		if(m%100==0)printf("Rxy(%3d) = %-f\n",m,R[m]);
	}
	printf("\n");
	
/********** 自己相関関数 **********/
	printf("自己相関関数\n");
	fileOpen(data[0],"data3.txt");
	
	for(int m=0;m<AUTODATASIZE;m++){
		for(int j=0;j<AUTODATASIZE-m;j++){
			data[2][j]=data[0][j+m];
		}
		R[m]=innerProduct(data[0],data[2])/AUTODATASIZE;
		fprintf(fp,"%f\n",R[m]);
		if(m%10==0)printf("Rxx(%3d) = %f\n",m,R[m]);
	}

}