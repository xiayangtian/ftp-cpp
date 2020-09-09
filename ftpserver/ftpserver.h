#include<iostream>
#include<string>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>


#define CTRMSGLEN 50
#define FILENAMELEN 20
using namespace std;
class TcpServer{
	private:
		int listenFd;
		int serverFd;
		struct sockaddr_in serverAddr;
		struct sockaddr_in clientAddr;
		int clientAddrLen;
	public:
		TcpServer(int port = 6666);
		~TcpServer();
		void TcpServerStart();
		int GetListenFd();
		int GetServerFd();
		void TcpServerSend(char *sendBuf);
		int TcpServerRecv(char *recvBuf,int recvSize);
};
TcpServer::TcpServer(int port){
	listenFd = 0;
	serverFd = 0;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cout<<"port:"<<port<<endl;
	listenFd = socket(AF_INET,SOCK_STREAM,0);
	bind(listenFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	listen(listenFd,10);
}

TcpServer::~TcpServer(){
	if(listenFd != 0)
		close(listenFd);
	if(serverFd != 0)
		close(serverFd);
}
int TcpServer::GetListenFd(){
	return listenFd;
}
int TcpServer::GetServerFd(){
	return serverFd;
}
void TcpServer::TcpServerStart(){
	serverFd = accept(listenFd,(struct sockaddr *)&clientAddr,(socklen_t *)&clientAddrLen);
}
void TcpServer::TcpServerSend(char *sendBuf){
	send(serverFd,sendBuf,strlen(sendBuf),0);
}
int TcpServer::TcpServerRecv(char *recvBuf,int recvSize){
	int recvNum;
	recvNum = recv(serverFd,recvBuf,recvSize,0);
	return recvNum;
}

enum ftpType{
	LS=0,QUIT,PUT,GET,OTHERS
};

class CtrMsg{
	private:
		ftpType msgType;
		string msg;
		string fileName;
	public:
		CtrMsg();
		~CtrMsg();
		void CtrMsgAnalyze();
		void InputMsg(char *ftpMsg);
		ftpType GetMsgType();
		string GetFileName();
};
CtrMsg::CtrMsg(){
	msgType = OTHERS;
	msg.erase(0,msg.length());
	fileName.erase(0,fileName.length());
}
CtrMsg::~CtrMsg(void){
	cout<<"msg:"<<msg<<endl;
	cout<<"msgType:"<<msgType<<endl;
}
void CtrMsg::InputMsg(char *ftpMsg){
	msg.assign(ftpMsg);
}
ftpType CtrMsg::GetMsgType(){
	return msgType;
}
string CtrMsg::GetFileName(){
	return fileName;
}
void CtrMsg::CtrMsgAnalyze(){
	string::iterator iteMsg;
	string fileNameTmp;
	for(iteMsg = msg.begin();iteMsg != msg.end();iteMsg++){
		if(*iteMsg != ' ')
			break;

	}
	switch(*iteMsg){
		case 'l':
			if(*(iteMsg+1)=='s'){
				msgType = LS;
				break;
			}
		case 'q':
			if(*(iteMsg+1) == 'u'&& *(iteMsg+2) == 'i'&& *(iteMsg+3) == 't'){
				msgType = QUIT;
				break;
			}
		case 'p':
			if(*(iteMsg+1) == 'u'&& *(iteMsg+2) == 't'){
				msgType = PUT;
				fileNameTmp = msg.substr(iteMsg-msg.begin()+3);
				fileNameTmp.erase(0,fileNameTmp.find_first_not_of(" "));
				fileNameTmp.erase(fileNameTmp.find_last_not_of(" ") + 1);
				fileName.assign(fileNameTmp);
				break;
			}
		case 'g':
			if(*(iteMsg+1) == 'e'&& *(iteMsg+2) == 't'){
				msgType = GET;
				fileNameTmp = msg.substr(iteMsg-msg.begin()+3);
				fileNameTmp.erase(0,fileNameTmp.find_first_not_of(" "));
				fileNameTmp.erase(fileNameTmp.find_last_not_of(" ") + 1);
				fileName.assign(fileNameTmp);
				break;
			}
		default:
			msgType = OTHERS;
	}		



}

