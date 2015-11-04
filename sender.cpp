#include <iostream>
#include "HEADERS_AND_CONSTANTS_H.h"
#include "DATA_TRANSFER_H.h"
#include "FILE_TRANSFER_SERVER_H.h"
/* This function sends fragment of file and is called by each thread */
void sendFragment(int client,DataTransfer *dataTransfer){

  /* get offset and size of this fragment*/
  int offset=dataTransfer->getInitialOffset();
  int size=dataTransfer->getPartFileSize();

  char block[MAX_BYTES];
  struct FragmentHeader fragmentheader;

  /* set fragment header values */
  fragmentheader.file_size=size;
  fragmentheader.part_no=dataTransfer->getPartSeq();
  dataTransfer->getExtension(fragmentheader.filename);  //set filename to be sent as extension

  cout<<"Thread no :"<<dataTransfer->getPartSeq()<<"=> Connected to client sending file Header..."<<endl<<endl;
  /* send fragment header describing fragment structure and wait for acknowledgement*/
  send(client,&fragmentheader,sizeof(fragmentheader),0);
  recv(client,&fragmentheader,sizeof(fragmentheader),0);

  cout<<"Thread no :"<<dataTransfer->getPartSeq()<<"=> Received acknowledgement sending data .."<<endl<<endl;
  int local_offset=0;
  int readbytes=1;

  /* read bytes from file and send to client */
  while(local_offset<size){

    readbytes=dataTransfer->readFragment(block,min(MAX_BYTES,size-local_offset),offset);
    send(client,block,readbytes,0);

    offset+=readbytes;
    local_offset+=readbytes;

  }

  cout<<"Thread no :"<<dataTransfer->getPartSeq()<<"=> Sent data waiting for tear down packet .."<<endl<<endl;
  /* receive tear down message indicating complete download */
  recv(client,block,sizeof(struct FragmentHeader),0);
  close(client);
}


/* This method accepts client connections and initiate file transfer process for each thread in parallel */
void connectClient(FileTransferServer *server,char file[]){

    int i=0;

    DataTransfer dataTransfer[MAX_STREAMS]; // different objects for each thread

    thread myThreads[MAX_STREAMS];  //separate threads for all STREAMS

    for(i=0;i<MAX_STREAMS;i++){             // set file attributes
      dataTransfer[i].setFileName(file);
      dataTransfer[i].setExtension();
      dataTransfer[i].setFileDesc();
    }

    int size=dataTransfer[0].getFileSize(); //size of total bytes

    /* divide file size into equal parts in fragments to be sent */
    int part_size=ceil((double)size/MAX_STREAMS);
    int initial_offset=0;

    /* distributing file size and initial offset for each stream */
    for(i=0;i<MAX_STREAMS;i++){
      dataTransfer[i].setPartFileSize(min(size-initial_offset,part_size));
      dataTransfer[i].setInitialOffset(initial_offset);
      dataTransfer[i].setPartSeq(i);
      initial_offset+=part_size;
    }

    i=0;
    while(i<MAX_STREAMS){

      /* accept client connection */
      struct sockaddr_in client_addr;
      socklen_t add_size=sizeof(client_addr);
      int client=accept(server->server_socket,(struct sockaddr *)(&client_addr),&add_size);

      /* call threads in parallel for execution*/
      myThreads[i]=thread(sendFragment,client,&dataTransfer[i]);
      i++;
    }

    /* wait for all threads to finish */
    for(i=0;i<MAX_STREAMS;i++)
      myThreads[i].join();

}

/* argv[1] is the relative or absolute file name */
int main(int argc,char *argv[]){

  FileTransferServer server;
  connectClient(&server,argv[1]);
}
