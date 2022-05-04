#ifndef STRING_H
    #define STRING_H



    void strcpy(char *str,char *str2);
    void strcpywl(char *str,char *str2,int tam);
    uint64 strlen(char *str);
    unsigned int findchar(char *str,char c);
    void strclean(char *str);
    int strcmp(const char *cs, const char *ct);
    uint64 asc_int(char *numero);
    void int_asc(int64 num, char *numero);
    int gettam_num(int64 num);


    uint32 sprintf(char *out,char *string,...);



#endif
