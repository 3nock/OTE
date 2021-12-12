// Wordlist Creator v1.1
// Coded by @thelinuxchoice
// Github: github.com/thelinuxchoice/wlcreator
// Sorry the shit code, it was made on "Legend Mode" using permute algorithm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


const char* charset = "abcdefghijklmnopqrstuvwxyz";
const char* charset0 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* charset1 = "0123456789";
const char* charset2 = "!@#$%*£_";
const char* charset3 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* charset4= "abcdefghijklmnopqrstuvwxyz0123456789";
const char* charset5 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* charset6 = "abcdefghijklmnopqrstuvwxyz!@#$%*£_";
const char* charset7 = "abcdefghijklmnopqrstuvwxyz!@#$%*£_0123456789";
const char* charset8 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%*£_";
const char* charset9 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* charset10 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* charset11 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%*£_";
const char* charset12 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%*£_01234567";
const char* charset13 = "0123456789!@#$%*£_";
const char* charset14 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%*£_0123456789";



char buffer[50];
char chr[3], cap[3], num[3], spc[3], apatt[3];
int patt = 0;
char makepatt[32];
void permute(int level) {
  const char* charset_ptr = charset;
  if(level == -1){


    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute(level - 1);

   }
  }
}


void permute0(int level) {
  const char* charset_ptr = charset0;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute0(level - 1);
   }
  }
}


void permute1(int level) {
  const char* charset_ptr = charset1;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute1(level - 1);
   }
  }
}


void permute2(int level) {
  const char* charset_ptr = charset2;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute2(level - 1);
   }
  }
}


void permute3(int level) {
  const char* charset_ptr = charset3;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute3(level - 1);
   }
  }
}


void permute4(int level) {
  const char* charset_ptr = charset4;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute4(level - 1);
   }
  }
}


void permute5(int level) {
  const char* charset_ptr = charset5;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute5(level - 1);
   }
  }
}


void permute6(int level) {
  const char* charset_ptr = charset6;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute6(level - 1);
   }
  }
}


void permute7(int level) {
  const char* charset_ptr = charset7;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute7(level - 1);
   }
  }
}


void permute8(int level) {
  const char* charset_ptr = charset8;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute8(level - 1);
   }
  }
}


void permute9(int level) {
  const char* charset_ptr = charset9;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute9(level - 1);
   }
  }
}


void permute10(int level) {
  const char* charset_ptr = charset10;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute10(level - 1);
   }
  }
}


void permute11(int level) {
  const char* charset_ptr = charset11;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute11(level - 1);
   }
  }
}


void permute12(int level) {
  const char* charset_ptr = charset12;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute12(level - 1);
   }
  }
}


void permute13(int level) {
  const char* charset_ptr = charset13;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute13(level - 1);
   }
  }
}


void permute14(int level) {
  const char* charset_ptr = charset14;
  if(level == -1){
    puts(buffer);
  }else {
   while(buffer[level]=*charset_ptr++) {
    permute14(level - 1);
   }
  }
}

int main(int argc, char **argv)
{

printf(" _    _ _ _____                _              \n");
printf("| |  | | /  __ \\              | |             \n");
printf("| |  | | | /  \\/_ __ ___  __ _| |_ ___  _ __  \n");
printf("| |/\\| | | |   | '__/ _ \\/ _` | __/ _ \\| '__| \n");
printf("\\  /\\  / | \\__/\\ | |  __/ (_| | || (_) | |    \n");
printf(" \\/  \\/|_|\\____/_|  \\___|\\__,_|\\__\\___/|_|    \n"); 
printf("\n");
  printf(":: Wordlist Creator v1.1 by @thelinuxchoice ::\n");


if (argc <= 1) {
printf("\n[*] Usage: ./wlcreator <password length>\n");
printf("[*] E.g.: ./wlcreator 5\n");
exit(1);

}
  printf("\n[*] Use lowercase letters? [y/n]: ");
  scanf("%s",&chr);
  printf("\n[*] Use CAP letters? [y/n]: ");
  scanf("%s",&cap);
  printf("\n[*] Use Numbers? [y/n]: ");
  scanf("%s",&num);
  printf("\n[*] Use Special Chars? [y/n]: ");
  scanf("%s",&spc);


   if ((!strcmp(chr, "n") && (!strcmp(cap, "n")) && (!strcmp(num, "n")) && (!strcmp(spc, "n"))))
{
   printf("[!] Wordlist not generated. Choose \"y\" for at least one option \n");
   exit(1);
}

  printf("\n[*] Use Pattern? [y/n]: ");
  scanf("%s",&apatt);

   if ((!strcmp(apatt, "y"))) {

    printf("\n1) pattern before string (E.g.: patternSTRING): \n");
    printf("2) pattern after string (E.g.: STRINGpattern): \n");
    printf("\n[*] Choose Pattern Mode: "); 
    scanf("%d",&patt);
    printf("[*] Type pattern (E.g.: admin): ");
    scanf("%s",&makepatt);  
}


  printf("\n[*] Generating WordList...(It can take a long time)\n");



//// y n n n
 if ((!strcmp(chr, "y") && (!strcmp(cap, "n")) && (!strcmp(num, "n")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}


/// n y n n



 if ((!strcmp(chr, "n") && (!strcmp(cap, "y")) && (!strcmp(num, "n")) && (!strcmp(spc, "n")))) {

    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }

////
  int length;
  sscanf(argv[1], "%d", &length); 
  buffer[length]='\0';
  permute0(length - 1);
///  
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
}

/// y y n n
 if ((!strcmp(chr, "y") && (!strcmp(cap, "y")) && (!strcmp(num, "n")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute3(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}
//// y y y n 5

 if ((!strcmp(chr, "y") && (!strcmp(cap, "y")) && (!strcmp(num, "y")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute5(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}

/// y y y n 9

 if ((!strcmp(chr, "y") && (!strcmp(cap, "y")) && (!strcmp(num, "y")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute9(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}

/// n y y n = 10

 if ((!strcmp(chr, "n") && (!strcmp(cap, "y")) && (!strcmp(num, "y")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute10(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}


/// n y y y 12

 if ((!strcmp(chr, "n") && (!strcmp(cap, "y")) && (!strcmp(num, "y")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute12(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}

// ny ny 11

 if ((!strcmp(chr, "n") && (!strcmp(cap, "y")) && (!strcmp(num, "n")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute11(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}
/// n n y n 1

 if ((!strcmp(chr, "n") && (!strcmp(cap, "n")) && (!strcmp(num, "y")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute1(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}

/// n n y y 13

 if ((!strcmp(chr, "n") && (!strcmp(cap, "n")) && (!strcmp(num, "y")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute13(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}
/// n n n y 2

 if ((!strcmp(chr, "n") && (!strcmp(cap, "n")) && (!strcmp(num, "n")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute2(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}

/// y n y n 4

 if ((!strcmp(chr, "y") && (!strcmp(cap, "n")) && (!strcmp(num, "y")) && (!strcmp(spc, "n")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute4(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}

/// y n n y 6

 if ((!strcmp(chr, "y") && (!strcmp(cap, "n")) && (!strcmp(num, "n")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute6(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}

/// y n y y 7

 if ((!strcmp(chr, "y") && (!strcmp(cap, "n")) && (!strcmp(num, "y")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute7(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    
}

/// y y n y 8

 if ((!strcmp(chr, "y") && (!strcmp(cap, "y")) && (!strcmp(num, "n")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute8(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

  
  
}

/// y y y y 14

 if ((!strcmp(chr, "y") && (!strcmp(cap, "y")) && (!strcmp(num, "y")) && (!strcmp(spc, "y")))) {
    remove("wordlist.txt");
    setvbuf(stdout, NULL, _IONBF, 0);
    int out = open("wordlist.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening wordlist.txt"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
/////
    int length;
    sscanf(argv[1], "%d", &length); 
    buffer[length]='\0';
    permute14(length - 1);
///////
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);
    printf("\n[*] Saved: wordlist.txt\n");  
    
}
  if ( patt == 1) {
    FILE *fr, *fw;
    char buffer[10000];
    fr = fopen("wordlist.txt","rt");
    if (fr == NULL) {
        printf("Error opening input file\n");
        return 1;
    }
    fw = fopen("wordlistp1.txt","wt");
    if (fw==NULL) {
        printf("Error opening output file\n");
        fclose (fr);
        return 1;
    }
    while (fgets(buffer, 10000, fr) != NULL) {
        buffer [ strcspn(buffer, "\r\n") ] = 0;   // remove trailing newline etc
        fprintf(fw, "%s%s\n", makepatt,buffer);
    }
    fclose(fw);
    fclose(fr);
    remove("wordlist.txt");
    printf("\n[*] Saved: wordlistp1.txt\n");
   

}
  else if (patt == 2) {

    FILE *fr, *fw;
    char buffer[10000];
    fr = fopen("wordlist.txt","rt");
    if (fr == NULL) {
        printf("Error opening input file\n");
        return 1;
    }
    fw = fopen("wordlistp2.txt","wt");
    if (fw==NULL) {
        printf("Error opening output file\n");
        fclose (fr);
        return 1;
    }
    while (fgets(buffer, 10000, fr) != NULL) {
        buffer [ strcspn(buffer, "\r\n") ] = 0;   // remove trailing newline etc
        fprintf(fw, "%s%s\n", buffer,makepatt);
    }
    fclose(fw);
    fclose(fr);
    remove("wordlist.txt");
    printf("\n[*] Saved: wordlistp2.txt\n");



}
 
   if ((!strcmp(apatt, "n"))) {

  printf("\n[*] Saved: wordlist.txt\n");
}
 

  return 0;
}


