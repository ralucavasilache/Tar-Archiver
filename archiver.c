#include<stdio.h>

#include<string.h>

#include<stdlib.h>

#include<time.h>

#include<math.h>

union record {
  char charptr[512];

  struct header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[2];
    char linkname[100];
    char magic[8];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
  }
  header;
};
//functie care ridica numarul b la puterea a;
int pow_(int b, int a) {
  int result = 1, i;
  for (i = 0; i < a; i++) {
    result *= b;
  }
  return result;
}
//transformare numar decimal in octal;
int convertDecimalToOctal(int decimalNumber) {
  int octalNumber = 0, i = 1;
  while (decimalNumber != 0) {
    octalNumber += (decimalNumber % 8) * i;
    decimalNumber /= 8;
    i *= 10;
  }
  return octalNumber;
}
//transformare numar octal in decimal;
long long convertOctalToDecimal(int octalNumber)
{
    int decimalNumber = 0, i = 0;
    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow_(8,i);
        ++i;
        octalNumber/=10;
    }
    i = 1;
    return decimalNumber;
}
//transformare sir de caractere in numar decimal;
long stringtodec(char * str) {
  long x = 0, i;
  for (i = strlen(str) - 1; i >= 0; i--) {

    x += ((int) str[i] - 48) * pow_(10, strlen(str) - 1 - i);

  }
  return x;
}
//memorare data calendaristica in variabile de tip int;
void datetoint(char * str, int * year, int * month, int * day) {
  int pw = 0;
  int i;
  * year = 0;
  * month = 0;
  * day = 0;

  for (i = 10; i >= 8; i--) {
    if (str[i] >= '0' && str[i] <= '9') {
      * day += ((int) str[i] - 48) * pow_(10, pw);
      pw++;

    }
  }
  pw = 0;
  for (i = 6; i >= 4; i--) {
    if (str[i] >= '0' && str[i] <= '9') {
      * month += ((int) str[i] - 48) * pow_(10, pw);
      pw++;
    }
  }
  pw = 0;
  for (i = 3; i >= 0; i--) {
    if (str[i] >= '0' && str[i] <= '9') {

      * year += ((int) str[i] - 48) * pow_(10, pw);
      pw++;
    }
  }

}
//memorare ora in variabile de tip int;
void timetoint(char * str, int * hour_, int * minute_, double * second_) {

  int pw = 0;

  * hour_ = 0;
  * minute_ = 0;
  * second_ = 0;
  double seconddb = 0;
  int i;

  for (i = 18; i >= 9; i--) {
    if (str[i] >= '0' && str[i] <= '9') {

      seconddb += ((int) str[i] - 48) / pow_(10, pw);
      pw++;

    }
  }

  pw = 0;
  for (i = 8; i >= 6; i--) {
    if (str[i] >= '0' && str[i] <= '9') {

      * second_ += ((int) str[i] - 48) * pow_(10, pw);
      pw++;

    }
  }

  pw = 0;
  for (i = 5; i >= 3; i--) {
    if (str[i] >= '0' && str[i] <= '9') {
      * minute_ += ((int) str[i] - 48) * pow_(10, pw);
      pw++;
    }
  }

  pw = 0;
  for (i = 2; i >= 0; i--) {
    if (str[i] >= '0' && str[i] <= '9') {
      * hour_ += ((int) str[i] - 48) * pow_(10, pw);
      pw++;
    }
  }

  * second_ += seconddb;
}
//cautare urmatorul multiplu de 512;
int next512(int n)
{
    if((float)n/512 != (int)n/512)
    {
        return (((int)n/512) + 1) * 512;
    }
    else
    {
        return n;
    }
}

void CREATE(char archivename[100], char directoryname[100]) {
  struct tm time_;
  union record arch;
  FILE * files = fopen("files.txt", "r");
  FILE * archFile = fopen(archivename, "w");

  directoryname[strlen(directoryname) - 1] = '\0';

  char fileline[200] = "\0", usr_usrmap[20] = "\0";
  char ui[20] = "\0", gi[20] = "\0", buff[256] = "\0", sizebuff[256] = "\0";
  char * token;
  int count;
  int sizedec, sizeoct;
  int yy1, mm1, dd1;
  int hour, minute;
  int sz;
  unsigned i, j;
  long u, g;
  long csm;
  double second;
  int a=0, b=0, c=0, r=0, w=0, x=0;


  

  while (fgets(fileline, sizeof(fileline), files)) {

    for (i = 0; i < 512; i++) {
      arch.charptr[i] = '\0';
    }

    strcpy(arch.header.mode, "0000000\0");
    strcpy(arch.header.uid, "0000000\0");
    strcpy(arch.header.gid, "0000000\0");
    strcpy(arch.header.size, "00000000000\0");
    strcpy(arch.header.chksum, "        ");
    strcpy(arch.header.typeflag, "0\0");
    strcpy(arch.header.magic, "GNUtar \0");
    strcpy(arch.header.devmajor, "\0\0\0\0\0\0\0\0");
    strcpy(arch.header.devminor, "\0\0\0\0\0\0\0\0");

    token = strtok(fileline, " ");
    count = 1;
    //calculare permisiuni;
    //a, b, c variabile care memoreaza numarul de aparitii
    // ale r, w, x;
    for(i = 0; i < strlen(token); i++) {
    if(token[i] == 'r') r++;
    if(token[i] == 'w') w++;
    if(token[i] == 'x') x++;
    }

    if(r==1) a=4;
    else if(r==2) {
      a=4; 
      b=4;
    } else if(r==3) {
      a=4; 
      b=4; 
      c=4;
    }
    if(w==1) 
      a+=2;
    else if(w==2) {
      a+=2;
      b+=2;
    }
    else if(w==3) {
      a+=2;
      b+=2;
      c+=2;
    }
    if(x==1) 
      a+=1;
    else if(x==2) {
      a+=1;
      b+=1;
    }
    else if(x==3) {
      a+=1;
      b+=1;
      c+=1;
    }

     arch.header.mode[5] = (char)(a + 48);
     arch.header.mode[6] = (char)(b + 48);
     arch.header.mode[7] = (char)(c + 48);
     arch.header.mode[8] = '\0';

    token = strtok(NULL, " ");
    while (token != NULL) {
      count++;
      //completare uname;
      if (count == 3) {
        strcpy(arch.header.uname, token);
        FILE * usrmap = fopen("usermap.txt", "r");
        //cautarea liniei corespunzatoare user-ului in usermap;
        //extragere uid si gid;
        while (strcmp(arch.header.uname, usr_usrmap) != 0) {
          fgets(buff, 250, usrmap);
          sscanf(buff, "%[^:]:%*[^:]:%[^:]:%[^:]:%*[^:]:%*[^:]:%*[^:]s\n", usr_usrmap, ui, gi);
        }

        u = stringtodec(ui);
        g = stringtodec(gi);

        sprintf(buff, "%lo", u);
        strcpy(arch.header.uid + (8 - strlen(buff) - 1), buff);
        arch.header.uid[7] = '\0';
        sprintf(buff, "%lo", g);
        strcpy(arch.header.gid + (8 - strlen(buff) - 1), buff);
        arch.header.gid[7] = '\0';
        fclose(usrmap);
        //completare gname;
      } else if (count == 4) {
        strcpy(arch.header.gname, token);
        //convertire size in octal si completarea campului;
      } else if (count == 5) {
      
        sizedec = stringtodec(token);
        sizeoct = convertDecimalToOctal(sizedec);
        sprintf(sizebuff, "%d", sizeoct);
        strcpy(arch.header.size + (12 - strlen(sizebuff) - 1), sizebuff);
        arch.header.size[11] = '\0';
        //extragere data;
      } else if (count == 6) {

        datetoint(token, & yy1, & mm1, & dd1);
        time_.tm_year = yy1;
        time_.tm_mon = mm1;
        time_.tm_mday = dd1;
        //extragere timp;
      } else if (count == 7) {

        timetoint(token, & hour, & minute, & second);
        time_.tm_hour = hour;
        time_.tm_min = minute;
        time_.tm_sec = second;
      }
      //calculare mtime;
      if (count == 8) {

        time_.tm_year -= 1900;
        time_.tm_mon -= 1;
        sprintf(arch.header.mtime, "%lo", (long) mktime( & time_));
      }
      //completare name;
      if (count == 9) {

        strcpy(arch.header.name, token);
        arch.header.name[strlen(arch.header.name) - 1] = '\0';
        strcpy(arch.header.linkname, arch.header.name);
      }

      token = strtok(NULL, " ");

    }
    //completarea datelor in charptr;
    strcpy(arch.charptr, arch.header.name);
    strcpy(arch.charptr + 100, arch.header.mode);
    strcpy(arch.charptr + 108, arch.header.uid);
    strcpy(arch.charptr + 116, arch.header.gid);
    strcpy(arch.charptr + 124, arch.header.size);
    strcpy(arch.charptr + 136, arch.header.mtime);
    strncpy(arch.charptr + 148, arch.header.chksum, 8);
    strcpy(arch.charptr + 156, arch.header.typeflag);
    strcpy(arch.charptr + 157, arch.header.linkname);
    strcpy(arch.charptr + 257, arch.header.magic);
    strcpy(arch.charptr + 265, arch.header.uname);
    strcpy(arch.charptr + 297, arch.header.gname);
    strcpy(arch.charptr + 329, arch.header.devmajor);
    strcpy(arch.charptr + 337, arch.header.devminor);
    
    //calculare chksum;
    csm = 0;
    for (j = 0; j < 512; j++) {
      csm += (int) arch.charptr[j];
    }

    char buff[8];
    sprintf(buff, "%lo", csm);
    strcpy(arch.header.chksum + (6 - strlen(buff)), buff);
   
    for (j = 0; j < 6 - strlen(buff); j++)
      arch.header.chksum[j] = '0';
    
    strncpy(arch.charptr + 148, arch.header.chksum, 8);
    arch.charptr[155] = ' ';

    for (i = 0; i < 512; i++) 
      fprintf(archFile, "%c", arch.charptr[i]);
    //concatenare nume fisier la nume director;
    char datafilename[100] = "\0";

    strcat(datafilename, directoryname);
    strcat(datafilename, arch.header.name);

    FILE * datafile = fopen(datafilename, "r");
    sz = 0;
    //extragerea datelor din fisier si introducerea lor
    //in arhiva;
    while (fgets(buff, 2, datafile) != NULL) {
      fprintf(archFile, "%c", buff[0]);
      sz++;
    }

    while ((float) sz / 512 != sz / 512) {
      char n = '\0';
      fprintf(archFile, "%c", n);
      sz++;
    }
    fclose(datafile);

    for (int i = 0; i < 512; i++) {
      arch.charptr[i] = '\0';
    }
  }
  fclose(archFile);
  printf("> Done!\n");
}

void LIST(char archivename[100]) {
  archivename[strlen(archivename) - 1] = '\0';
  FILE * archive = fopen(archivename, "r");

  char buff[512] = "\0";
  char name[100] = "\0";
  int datasize = 0;
  int offset = 0;

  if (archive == NULL) {
    printf("> File not found!\n");
  }
  //extragere nume din arhiva si afisarea pe ecran;
  while (archive != NULL) {
    fgets(name, 100, archive);
    if (strcmp(name, "\0") == 0) {
      break;
    }
    printf("> %s\n", name);
    strcpy(name, "\0");
    //mutarea cursorului pe pozitia campului size;
    fseek(archive, offset + 124, SEEK_SET);
    fgets(buff, 12, archive);
    sprintf(buff, "%ld", stringtodec(buff));

    datasize = stringtodec(buff);
    datasize = convertOctalToDecimal(datasize);
    offset += 512;
    datasize = next512(datasize);
    offset += datasize;
    //mutarea cursorului pe pozitia corespunzatoare header-ului
    //urmatorului fisier;
    fseek(archive, offset, SEEK_SET);
  }
  if(archive != NULL)
  {
    fclose(archive);    
  }
}

void EXTRACT(char filename[100], char archivename[100]) {
  archivename[strlen(archivename) - 1] = '\0';
  FILE * archive = fopen(archivename, "r");

  char buff[512];
  char name[100];
  int datasize = 0;
  int offset = 0;
  int filefound = 0;
  int datasize_ = 0;

  while (archive != NULL) {
    fgets(name, 100, archive);
    
    if (strcmp(name, "\0") == 0) {
      break;
    }
    //cautare nume fisier;
    if (strcmp(name, filename) == 0) {
      filefound = 1;
    }
    //mutarea cursorului pe pozitia campului size;
    fseek(archive, offset + 124, SEEK_SET);
    fgets(buff, 12, archive);
    sprintf(buff, "%ld", stringtodec(buff));
    
    datasize = stringtodec(buff);
    datasize = convertOctalToDecimal(datasize);
    offset += 512;
    datasize_ = datasize;
    datasize = next512(datasize);
    //mutarea cursorului pe pozitia datelor;
    fseek(archive, offset, SEEK_SET);
    char data[datasize];
    strcpy(data, "\0");
    //citire date;
    fread(data, 1, datasize, archive);
    //creare fisier extras;
    //scriere date;
    if (filefound) {
      char extractfilename[120] = "\0";
      strcat(extractfilename, "extracted_");
      strcat(extractfilename, name);
      FILE * extractfile = fopen(extractfilename, "w");
      fwrite(data, 1, datasize_, extractfile);
      fclose(extractfile);
      printf("> File extracted!\n");
      break;
    }
    strcpy(name, "\0");
    //mutarea cursorului pe pozitia corespunzatoare header-ului
    //urmatorului fisier;   
    fseek(archive, offset + datasize, SEEK_SET);
    offset += datasize;
  }

  if (!filefound) {
    printf("> File not found!\n");
  }

  fclose(archive);
}

int main() {

  char command[50] = "\0";
  int commandnr = 0;
  char * commandtoken;

  while (strcmp("exit\n", command) != 0) {
    fgets(command, 50, stdin);
    commandtoken = strtok(command, " ");
    
    if (strcmp("exit\n", commandtoken) == 0)
      return 0;
    //verificarea comenzii create si a parametrilor dati;
    if (strcmp("create", commandtoken) == 0) {
      
      commandtoken = strtok(NULL, " ");
      char archName[100] = "\0", archDir[100] = "\0";
      
      while (commandtoken != NULL) {
        commandnr++;
        if (commandnr > 2)
          break;
        //completare nume arhiva si nume director de arhivat;
        if (strlen(archName) == 0) {
          strcpy(archName, commandtoken);
          commandtoken = strtok(NULL, " ");
        } else if (strlen(archName) > 0 && strlen(archDir) == 0) {
          strcpy(archDir, commandtoken);
          commandtoken = strtok(NULL, " ");
        }
      }

      if (commandnr != 2) {
        printf("> Wrong command!\n");
      } else {
        CREATE(archName, archDir);
      }
      commandtoken = NULL;
      commandnr = 0;

    }
    //verificarea functiei list si a parametrilor dati;
    else if(strcmp("list", commandtoken) == 0)
    {
      commandtoken = strtok(NULL, " ");
      char archName[100] = "\0";

      while (commandtoken != NULL) {
        commandnr++;
        if (commandnr > 1) {
          break;
        }
          strcpy(archName, commandtoken);
          commandtoken = strtok(NULL, " ");
      }

      if (commandnr != 1) {
        printf("> Wrong command!\n");
      } else {
        LIST(archName);
      }
      commandtoken = NULL;
      commandnr = 0;
    }
    //verificarea comenzii extract si a parametrilor dati;
    else if (strcmp("extract", commandtoken) == 0) {
      commandtoken = strtok(NULL, " ");
      char archName[100] = "\0", fileName[100] = "\0";
      
      while (commandtoken != NULL) {
        commandnr++;
        if (commandnr > 2)
          break;
        if (strlen(fileName) == 0) {
          strcpy(fileName, commandtoken);
          commandtoken = strtok(NULL, " ");
        } else if (strlen(fileName) > 0 && strlen(archName) == 0) {
          strcpy(archName, commandtoken);
          commandtoken = strtok(NULL, " ");
        }
      }

      if (commandnr != 2) {
        printf("> Wrong command!\n");
      } else {
        EXTRACT(fileName, archName);
      }
      commandtoken = NULL;
      commandnr = 0;

    } 
    else
      printf("> Wrong command!\n");
  }
  return 0;
}