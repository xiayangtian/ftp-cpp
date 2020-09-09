#include"ftpserver.h"
#define CONTROLPORT 21
#define DATAPORT 20
#define FILEDIR "../serverfiles/"
#define FILESIZE 10000
using namespace std;
int main(){
	int fdCurrent;
	int err;
	char ctrData[21];
	int ctrDataLen;
	
	CtrMsg ctrMsg;
	
	int epfd,nfds;
	struct epoll_event ev,events[20];
	
	struct dirent **nameList;
	struct stat statBuf;
	int fileNum;
	char fileNameBuff[20];
	int recvNum;
	int sendNum;
	char fileBuffer[FILESIZE] = {0};
	
	epfd = epoll_create(256);
	TcpServer ftpCtrServer(CONTROLPORT);
	TcpServer ftpDataServer(DATAPORT);

	ev.data.fd = ftpCtrServer.GetListenFd();
	ev.events = EPOLLIN|EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,ftpCtrServer.GetListenFd(),&ev);

	

	
	cout<<"Start FTP Control Connection!!!"<<endl;
	while(1){
		nfds = epoll_wait(epfd,events,20,500);
		if(ctrMsg.GetMsgType() == QUIT){
			cout<<"Closing!!!"<<endl;
			break;
		}
		for(int i=0;i<nfds;i++){
			if(events[i].data.fd == ftpCtrServer.GetListenFd()){
				ftpCtrServer.TcpServerStart();
				ftpDataServer.TcpServerStart();
				ev.data.fd = ftpCtrServer.GetServerFd();
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,ftpCtrServer.GetServerFd(),&ev);
			}
			else if(events[i].events&EPOLLIN){
				cout<<"\nRecv a Ctr Msg!!!"<<endl;
				if((fdCurrent = events[i].data.fd)<0)
					continue;
				if((ctrDataLen = read(fdCurrent,ctrData,20))<0){
					cout<<"Recv error!!!"<<endl;
				}
				else{
					ctrData[ctrDataLen] = '\0';
					cout<<"Ctr Msg:"<<ctrData<<endl;
					ctrMsg.InputMsg(ctrData);
					ctrMsg.CtrMsgAnalyze();
					cout<<"Msg type:"<<ctrMsg.GetMsgType()<<endl;
					if(ctrMsg.GetMsgType() == PUT){
						cout<<"Download a file from client"<<endl;
						char filePath[50] = {0};
						strcpy(filePath,FILEDIR);
						strcat(filePath,ctrMsg.GetFileName().c_str());
						cout<<"FilePath:"<<filePath<<endl;
						FILE *fp = fopen(filePath,"wb+");
						if(fp == NULL){
							cout<<"Open file error!!!"<<endl;
							break;
						}

						memset(fileBuffer,0,sizeof(fileBuffer));
						recvNum = ftpDataServer.TcpServerRecv(fileBuffer,FILESIZE);
						cout<<"recvnum:"<<recvNum<<endl;
						cout<<"buffer:"<<fileBuffer<<endl;
						fwrite(fileBuffer,recvNum,1,fp);
						fclose(fp);
					}
					
					ev.data.fd = fdCurrent;
					ev.events = EPOLLOUT|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_MOD,fdCurrent,&ev);
				}
			}
			else if(events[i].events&EPOLLOUT){
				fdCurrent = events[i].data.fd;
				if(ctrMsg.GetMsgType() == GET){
					cout<<"Upload a file to client"<<endl;
					char filePath[50] = {0};
					strcpy(filePath,FILEDIR);
					strcat(filePath,ctrMsg.GetFileName().c_str());
					cout<<"filePath:"<<filePath<<endl;
					FILE *fp = fopen(filePath,"rb+");
					if(fp == NULL){
						cout<<"Open file error!!!"<<endl;
						break;
					}
					struct stat statbuf;
					int fileSize;
					stat(filePath,&statbuf);
					fileSize = statbuf.st_size;
					//memset(fileBuffer,0,sizeof(fileBuffer));
					sendNum = fread(fileBuffer,1,fileSize,fp);
					cout<<"sendnum:"<<sendNum<<endl;
					cout<<"buffer:"<<fileBuffer<<endl;
					ftpDataServer.TcpServerSend(fileBuffer);
					fclose(fp);
				}
				if(ctrMsg.GetMsgType() == QUIT || ctrMsg.GetMsgType() == GET || ctrMsg.GetMsgType() == PUT)
					write(fdCurrent,"OK",3);
				else if(ctrMsg.GetMsgType() == 0){
					
					fileNum = scandir(FILEDIR,&nameList,0,alphasort);
					if(fileNum == 0){
						cout<<"No files!!!"<<endl;	
						break;
					}
					for(int i=0;i<fileNum;i++){
						if(nameList[i]->d_type == 8){
							memset(fileNameBuff,0,sizeof(fileNameBuff));
							cout<<"filename:"<<nameList[i]->d_name<<endl;
							strcpy(fileNameBuff,nameList[i]->d_name);
							strcat(fileNameBuff,"\n");
							write(fdCurrent,fileNameBuff,strlen(fileNameBuff));
						}
					}	
				}
				else{	
					write(fdCurrent,"Error Instruction",18);
				}
				ev.data.fd = fdCurrent;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_MOD,fdCurrent,&ev);
			}
		}
	}	
	
	return 0;	
}
