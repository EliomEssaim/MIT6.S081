#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

const int BUFSIZE = 512;

char*
namecopy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  *s = 0;
  return os;
}


void getFileName(char* path, char* name_buf){
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  memmove(name_buf, p, strlen(p));
}
void cleanBuf(char* buf, int bufsize){
  memset(buf, 0, bufsize);
}
void DFS(char* path, char* target, char* name_buf,int level){
  char *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s in level %d\n", path, level);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf("%s is a FILE\n", path);
    cleanBuf(name_buf, 10);
    getFileName(path, name_buf);
    if(strcmp(name_buf, target)==0){
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    // printf("%s is a DIR\n", path);
    if(strlen(path) + 1 > BUFSIZE){
      printf("find: path too long\n");
      break;
    }
    p = path+strlen(path);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // printf("de.name %s end in level %d\n",de.name, level);
      if(strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
        continue;
      namecopy(p, de.name);

      DFS(path, target, name_buf, level + 1);
    }
    break;
  }
  close(fd);
};

int
main(int argc, char *argv[])
{
  if(argc != 3){
      printf("Error arugments.");
      exit(1);
  }
  char* path = argv[1];
  char* target = argv[2];
  char name_buf[10];
  char buf[BUFSIZE];
  cleanBuf(buf, BUFSIZE);
  strcpy(buf, path);
  DFS(buf, target, name_buf, 0 );

  exit(0);
}
