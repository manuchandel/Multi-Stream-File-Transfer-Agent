#include <iostream>
#include "HEADERS_AND_CONSTANTS_H.h"
#include "RECEIVE_FRAGMENT_H.h"
#include "ASSEMBLE_H.h"

/* This message reads bytes from download stream */
void readMessage(int client,ReceiveFragment *receiveFragment){

  char block[MAX_BYTES];  //used as buffer
  char file[FILE_NAME_LENGTH];    //used to keep file name

  this_thread::sleep_for(chrono::seconds(SECONDS_TO_WAIT)); //wait 3 seconds before begging to match with network delay

  cout<<"Connected to server"<<endl<<endl;
  /* receive header for this fragment */
  if(recv(client,block,sizeof(struct FragmentHeader),0)!=sizeof(struct FragmentHeader))
    cout<<"Error in receiving Header"<<endl;
  else cout<<"Received header successfully"<<endl<<endl;
  FragmentHeader *f=(struct FragmentHeader *)block;
  receiveFragment->setFileName(f->filename);

  /* create fragment file with names F0.temp, F1.temp, F2.temp ..... where number denotes fragment sequentially */
  strcpy(file,TEMP_FILE);
  file[strlen(TEMP_DIR)+2]=f->part_no+'0';
  receiveFragment->createFile(file);

  int part=f->part_no;      //part no of this fragment
  int size=f->file_size;    //size of this fragment
  int offset=0;
  int bytes=0;

  /* send acknowledgement for received header and indicate server to send data bytes */
  memset(block,0,MAX_BYTES);
  send(client,block,sizeof(struct FragmentHeader),0);
  
  cout<<"Thread : "<<part<<" => Reading Bytes "<<endl<<endl;
  /* read bytes from byte stream */
  while(offset<size){

    bytes=recv(client,block,MAX_BYTES,0);
    receiveFragment->writeToTheFile(block,bytes,offset);
    offset+=bytes;
  }

  cout<<size<<" Bytes received from thread "<<part<<endl;

  /* send tear down packet to server indicating end of transfer */
  memset(block,0,sizeof(struct FragmentHeader));
  send(client,block,sizeof(struct FragmentHeader),0);
  cout<<"Thread "<<part<<" : "<<"Tear down packet sent to server closing connection "<<endl<<endl;

}

/* Initiates client connection to server this method is called by all threads*/
void connectClient(ReceiveFragment *receiveFragment){

    int client_socket=socket(PF_INET,SOCK_STREAM,0);  //create client socket
    if(client_socket<0)
      cout<<"Cannot create client socket"<<endl<<endl;

    /* set all values for sockaddr_in struct */
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;           //ipv4 connection family
    server_addr.sin_port=htons(PORT);         //port of server in network byte order
    server_addr.sin_addr.s_addr=inet_addr(HOST);      //server address to connect with
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));   //padding rest bits to zero

    if(connect(client_socket,(struct sockaddr *)(&server_addr),sizeof(server_addr))<0)  //connect to server
      cout<<"Cannot connect to server"<<endl<<endl;

    readMessage(client_socket,receiveFragment);
    close(client_socket);
}

/* This methods opens multiple connection to clients and starts downloading data from server */
void manageClients(){

  int i;
  char file[FILE_NAME_LENGTH];
  char extension[FILE_NAME_LENGTH];   //extension of file

  mkdir(TEMP_DIR,S_IRWXU|S_IRWXG|S_IRWXO);    //All fragments are in this new directory

  thread myThreads[MAX_STREAMS];
  ReceiveFragment receiveFragment[MAX_STREAMS];

  for(i=0;i<MAX_STREAMS;i++)
    myThreads[i]=thread(connectClient,&receiveFragment[i]);

  for(i=0;i<MAX_STREAMS;i++)
      myThreads[i].join();


  AssembleFile assemblefile;  // assembling file
  receiveFragment[0].getFileName(extension);

  /* New file name */
  strcpy(file,NEW_FILE);
  memcpy(file+strlen(NEW_FILE),extension,strlen(extension));
  file[strlen(NEW_FILE)+strlen(extension)]='\0';

  /* Assemble all files into new file*/
  assemblefile.createFile(file);
  assemblefile.setFiles(MAX_STREAMS);
  assemblefile.writeToTheFile();
}

int main(int argc, char *argv[]){
  manageClients();
}
