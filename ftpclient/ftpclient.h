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
class TcpClient{
	private:
		int clientFd;
		struct sockaddr_in serverAddr;
	public:
		TcpClient(char* cserverAddr = "127.0.0.1",int port = 6666);
		~TcpClient();
		void TcpClientStart();
		void TcpClientSend(char *sendBuf);
		int TcpClientRecv(char *recvBuf,int recvSize);
};
TcpClient::TcpClient(char* cserverAddr,int port){
	clientFd = 0;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(cserverAddr);
	cout<<"port:"<<port<<endl;
	cout<<"serverAddr:"<<cserverAddr<<endl;
}
TcpClient::~TcpClient(){
	if(clientFd != 0)
		close(clientFd);
}
void TcpClient::TcpClientStart(){
	int err;
	clientFd = socket(AF_INET,SOCK_STREAM,0);
	err = connect(clientFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	if(err < 0){
		cout<<"Connect error"<<endl;
		exit(0);
	}
}
void TcpClient::TcpClientSend(char *sendBuf){
	send(clientFd,sendBuf,strlen(sendBuf),0);

}
int TcpClient::TcpClientRecv(char *recvBuf,int recvSize){
	int recvNum;
	recvNum = recv(clientFd,recvBuf,recvSize,0);
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
	//cout<<"Create a CtrMsg Variate"<<endl;
	msgType = OTHERS;
	msg.erase(0,msg.length());
	fileName.erase(0,fileName.length());
}
CtrMsg::~CtrMsg(void){
	//cout<<"Destroy a CtrMsg Variate"<<endl;
	cout<<"msg:"<<msg<<endl;
	cout<<"msgType:"<<msgType<<endl;
	//cout<<"fileName:"<<fileName<<endl;
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

