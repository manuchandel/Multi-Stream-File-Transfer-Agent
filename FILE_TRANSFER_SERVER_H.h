#ifndef FILE_TRANSER_SERVER_H
#define FILE_TRANSER_SERVER_H

/* This class gives service to serversocket  */
class FileTransferServer{
  public:

    FileTransferServer();
    ~FileTransferServer();
    FileTransferServer(const FileTransferServer &) {}; //declare copy constructor public

    friend void connectClient(FileTransferServer*,char filename[]);
    friend void sendFragment(int,DataTransfer *);

  private:

    int server_socket;
    struct sockaddr_in server_addr;

};

/* constructor sets up necesary socket connections */
FileTransferServer :: FileTransferServer(){

  /* initiate server socket with TCP connection */
  this->server_socket=socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket<0)
    cout<<"Socket failed to initialize"<<endl;

  /* set sockaddr_in structure */
  server_addr.sin_port=htons(PORT); //  set port number
  server_addr.sin_family=AF_INET; //ipv4 internet family
  server_addr.sin_addr.s_addr=inet_addr(HOST); // set host address

  if(bind(server_socket,(struct sockaddr *)(&server_addr),sizeof(server_addr))<0)  // bind socket
    cout<<"Cannot bind socket"<<endl;

  if(listen(server_socket,5)<0)  //start listening on port specified
    cout<<"Cannot Listen on this port"<<endl;

}

/* Destructor closes server connection after file transfer process completes */
FileTransferServer :: ~FileTransferServer(){
  close(server_socket);
}

#endif
