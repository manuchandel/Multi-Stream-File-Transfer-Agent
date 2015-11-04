#ifndef ASSEMBLE_H
#define ASSEMBLE_H

/* This class assembles the file fragments present in the temporary directory into one single file */
class AssembleFile{

  public:

    AssembleFile();
    void setFiles(int);
    void createFile(char file[]);
    void writeToTheFile();

  private:

    int file_desc;
    int no_of_files;
};

/* Constructor for this class */
AssembleFile :: AssembleFile(){}

/* This function sets the no_of_file attribute which represents no of fragment files */
void AssembleFile :: setFiles(int n){
  this->no_of_files=n;
}

/* This function creates final output file with given name and read write execute permissions */
void AssembleFile :: createFile(char file[]){

  int fd=creat(file,S_IRWXU|S_IRWXG|S_IRWXO);
  if(fd<0)
    cout<<"Cannot create file"<<endl;
  close(fd);

  /* setting file_desc attribute for newly created file*/
  this->file_desc=open(file,O_RDWR);
  if(this->file_desc<0)
    cout<<"Error cannot open file"<<endl;
}

/* This function reads from file fragments and writes to one output file */
void AssembleFile :: writeToTheFile(){

  int fd=this->file_desc;

  int offset=0;         // for final output file
  int local_offset=0;   // for each fragment file
  int bytes_read=0;

  char file[FILE_NAME_LENGTH];  // fragment file name
  char buffer[MAX_BYTES];       // buffer to be read in
  strcpy(file,TEMP_FILE);

  for(int i=0;i<this->no_of_files;i++){

    file[strlen(TEMP_DIR)+2]=i+'0';       //  file name of next fragment
    int fd1=open(file,O_RDONLY);  // opening fragment with readonly mode

    local_offset=0;
    bytes_read=1;

    while(bytes_read>0){

      bytes_read= pread(fd1,buffer,MAX_BYTES,local_offset); //read from file fragment
      pwrite(fd,buffer,bytes_read,offset);                  // write into file fragment

      offset+=bytes_read;
      local_offset+=bytes_read;
    }
    close(fd1);   //closing this file fragment
  }
  close(fd);      //closing final output file
}

#endif
