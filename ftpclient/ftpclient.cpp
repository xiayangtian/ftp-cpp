#include"ftpclient.h"
#define CONTROLPORT 21
#define DATAPORT 20
#define SERVERADDR "192.168.60.179"
#define FILEDIR "../clientfiles/"
#define FILESIZE 100000
using namespace std;
int main(){
	char ctrData[20];
	char recvResponse[50];
	int recvResponseLen = 0;
	int sendNum;
	char buffer[FILESIZE] = {0};
	CtrMsg ctrMsg;
	TcpClient ftpCtrClient(SERVERADDR,CONTROLPORT);
	TcpClient ftpDataClient(SERVERADDR,DATAPORT);
	ftpCtrClient.TcpClientStart();
	ftpDataClient.TcpClientStart();
	cout<<"Connecting Success!!"<<endl;
	while(1){
		strcpy(ctrData,"");
		cout<<"Input Your Ctr Msg:";
		cin.get(ctrData,20).get();
		if(strlen(ctrData) == 0){
			cin.clear();
			cin.sync();
			cin.ignore(20,'\n');
			continue;
		}
		ctrMsg.InputMsg(ctrData);
		ctrMsg.CtrMsgAnalyze();
		ftpCtrClient.TcpClientSend(ctrData);
		if(ctrMsg.GetMsgType() == QUIT)
			break;
		else if(ctrMsg.GetMsgType() == PUT){
			cout<<"Upload a file to server!!!"<<endl;
			char filePath[50] = {0};
			strcpy(filePath,FILEDIR);
			strcat(filePath,ctrMsg.GetFileName().c_str());
			FILE *fp = fopen(filePath,"rb");
			if(fp == NULL){
				cout<<"Open file error!!!"<<endl;
				break;
			}
			struct stat statbuf;
			stat(filePath,&statbuf);
			int fileSize = statbuf.st_size;
			sendNum = fread(buffer,1,fileSize,fp);
			cout<<"sendNum:"<<sendNum<<endl;
			cout<<"buffer:"<<buffer<<endl;
			ftpDataClient.TcpClientSend(buffer);
			fclose(fp);
		}
		else if(ctrMsg.GetMsgType() == GET){
			cout<<"Download a file from server!!!"<<endl;
			char filepath[50] = {0};
			strcpy(filepath,FILEDIR);
			strcat(filepath,ctrMsg.GetFileName().c_str());
			FILE *fp = fopen(filepath,"wb+");
			if(fp == NULL){
				cout<<"Open file error!!!"<<endl;
				break;
			}
			int recvNum;
			recvNum = ftpDataClient.TcpClientRecv(buffer,FILESIZE);
			cout<<"recvNum:"<<recvNum<<endl;
			cout<<"buffer:"<<buffer<<endl;
			fwrite(buffer,recvNum,1,fp);
			fclose(fp);
		}
		recvResponseLen = ftpCtrClient.TcpClientRecv(recvResponse,50);
		if(recvResponseLen != 0){
			recvResponse[recvResponseLen] = '\0';
			cout<<recvResponse<<"\n"<<endl;
		}
	}
	return 0;
}
