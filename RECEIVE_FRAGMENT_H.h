#ifndef RECEIVE_FRAGMENT_H
#define RECEIVE_FRAGMENT_H

/* This class performs number of functions on received bytes */
class ReceiveFragment{

  public:

    ReceiveFragment();
    ReceiveFragment(const ReceiveFragment&) {}; //declare copy constructor public

    void setFileName(char file[]);
    void getFileName(char []);

    void createFile(char file[]);
    void writeToTheFile(char block[],int,int);

  private:

    int file_desc;                    // file descriptor
    char file_name[FILE_NAME_LENGTH];
};

/* Constructor initializing class variables */
ReceiveFragment :: ReceiveFragment(){

  this->file_desc=-1;
  strcpy(file_name,"");
}

/* This function sets filename for this fragment*/
void ReceiveFragment :: setFileName(char file[]){
  strcpy(this->file_name,file);
}

/* This function returns filename in file attribute for this fragment */
void ReceiveFragment :: getFileName(char file[]){
  strcpy(file,this->file_name);
}

/* This function creates file fragment with filename*/
void ReceiveFragment :: createFile(char file[]){

  int fd=creat(file,S_IRWXU|S_IRWXG|S_IRWXO);
  if(fd<0)
    cout<<"Cannot create file"<<endl;
  close(fd);

  /* setting file_desc attribute */
  this->file_desc=open(file,O_RDWR);
  if(this->file_desc<0)
    cout<<"Error cannot open file"<<endl;
}

/* This function writes bytes no of data from block to file with file_desc at offset bytes */
void ReceiveFragment:: writeToTheFile(char block[],int bytes, int offset){

  int fd=this->file_desc;
  pwrite(fd,block,bytes,offset);
}

#endif
