#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

/* The instance of this class is attached with each thread and provides services of IO with files */
class DataTransfer{
  public:

    DataTransfer();
    DataTransfer(const DataTransfer &) {}; //declare copy constructor public


    void setFileName(char file[]);
    void setExtension();
    void setFileDesc();

    void setPartSeq(int);
    void setPartFileSize(int);
    void setInitialOffset(int);

    void getFileName(char []);
    void getExtension(char []);

    int getFileDesc();
    int getPartSeq();
    int getFileSize();
    int getPartFileSize();
    int getInitialOffset();

    int readFragment(char block[],int,int);

  private:

    int part_seq;                         // part sequence number
    int file_desc;                        // file descriptor
    int part_file_size;                   // size of this part
    int initial_offset;                   // initial offset of this fragment
    char file_name[FILE_NAME_LENGTH];     // filename
    char extension[FILE_NAME_LENGTH];     // file extension

};

/* Constructor initializes all attributes*/
DataTransfer :: DataTransfer(){

  this->file_desc=-1;
  this->part_seq=-1;
  this->part_file_size=0;
  this->initial_offset=0;
  strcpy(file_name,"");
  strcpy(extension,"");
}

/* sets file_name attribute */
void DataTransfer :: setFileName(char file[]){
  strcpy(this->file_name,file);
}

/* opens file and sets file_desc attribute */
void DataTransfer :: setFileDesc(){

  this->file_desc=open(this->file_name,O_RDONLY);
  if(this->file_desc<0)
    cout<<"Error cannot open file"<<endl;
}

/* extracts extension from file name given */
void DataTransfer :: setExtension(){

  int n=strlen(this->file_name);
  int i=n-1,j;

  char file[FILE_NAME_LENGTH];

  while(this->file_name[i]!='.' && i>=0) i--;

  for(j=0;j<n-i;j++)
    file[j]=this->file_name[i+j];
  file[j]='\0';

  strcpy(this->extension,file);
}

/* sets sequence number of the part of file*/
void DataTransfer :: setPartSeq(int seq){
  this->part_seq=seq;
}

/* sets file size of this fragment */
void DataTransfer :: setPartFileSize(int size){
    this->part_file_size=size;
}

/* sets initial offset of fragment */
void DataTransfer :: setInitialOffset(int offset){
    this->initial_offset=offset;
}

/* writes filename attribute to file arguement */
void DataTransfer :: getFileName(char file[]){
  strcpy(file,this->file_name);
}

/* returns sequence of part */
int DataTransfer :: getPartSeq(){
  return this->part_seq;
}

/*  writes filename attribute to file arguement */
void DataTransfer :: getExtension(char file[]){
  strcpy(file,this->extension);
}


/* returns total size of the file to be sent */
int DataTransfer :: getFileSize(){

  /* create a stat object */
  struct stat obj;
  if(fstat(this->file_desc,&obj)<0)
    cout<<"Cannot get file size error"<<endl;
  return obj.st_size; // file size
}


/* returns size of fragment which is part of file*/
int DataTransfer :: getPartFileSize(){
  return this->part_file_size; // file size
}

/* returns initial offset */
int DataTransfer :: getInitialOffset(){
  return this->initial_offset; // file size
}


/* reads bytes from file into the block at given offset and return bytes read */
int DataTransfer:: readFragment(char block[],int bytes, int offset){

  int fd=this->file_desc;
  int bytes_read=pread(fd,block,bytes,offset);
  if(bytes_read<0){
    cout<<"Cannot read specified block"<<endl;
    return -1;
  }
  return bytes_read;
}

#endif
