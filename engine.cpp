#include "engine.h"
Engine::Engine()
{
    this->noise=false;
}
void Engine::setNoise(bool b){
    this->noise = b;
}

int Engine::size_of_file(FILE *file){
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);
    return size;
}
int Engine::hide(const char * ar1, const char *ar2,const char *ar3){
#ifdef __linux
    FILE* source=fopen(ar2,"r+");
#endif
#ifdef __WIN32
    FILE* source=fopen(ar2,"r+b");
#endif
    if (source == NULL ){
        return 1;
    }
    FILE* target;
#ifdef __linux
    FILE* txt=fopen(ar1,"r");
#endif
#ifdef __WIN32
    FILE* txt=fopen(ar1,"rb");
#endif

    if(txt == NULL ){
        return 1;
    }
    unsigned long size0 = size_of_file(source);
    unsigned long size1 = size_of_file(txt);
    unsigned char a,b;
    unsigned short b0;
    unsigned int i,j,space =55,type = 8,div;
    if(fread(&b0,2,1,source) != 1 )
        return 1;
    switch(b0){
    case 0x4952 :{
            fseek(source,34,SEEK_SET);
            fscanf(source,"%c",&b);
            type = b;
            fseek(source,20,SEEK_SET);
            fread(&b,1,1,source);
            if(b!=1) return 4;
            space=59;
#ifdef __linux__
            target=fopen(strcat((char*)ar3,".wav"),"w+");
#endif
#ifdef __WIN32
            target=fopen(strcat((char*)ar3,".wav"),"w+b");
#endif
            break;
        }
    case 0x4d42 :{
            fseek(source,28,SEEK_SET);
            fread(&b,1,1,source);
            if (b != 0x18) return 5;
            fseek(source,29,SEEK_SET);
            fread(&b,1,1,source);
            if (b != 0) return 4;
            space=55;
#ifdef __linux__
            target=fopen(strcat((char*)ar3,".bmp"),"w+");
#endif
#ifdef __WIN32
            target=fopen(strcat((char*)ar3,".bmp"),"w+b");
#endif

            break;
        }
    default:{
            return 2;
        }
    }
    if(target == NULL ){
        return 1 ;
    }
    div = type/8;
    if(noise){
        unsigned short *a,*b;
        fseek(source,space,SEEK_SET);
        a = (unsigned short*) calloc(2,size0-space);
        b=a;
        fread(a,size0-space,1,source);
        fseek(source,space,SEEK_SET);
        for(unsigned long  i = 0;i< size0 -space;i++){
            a[i]^=rand()%2;
        }
        fwrite(a,size0-space,1,source);
    }
    fseek(source,0,SEEK_SET);
    if (  (size1 * 8 ) < (( size0 - space )/(div)) ) {
        for(i=0;i<(space-1);i++){
            fscanf(source,"%c",&a);
            fprintf(target,"%c",a);
        }
        for (i = 0; i < size1; i++) {
            fscanf(txt,"%c",&a);
            for (j = 0; j < 8; j++) {
                fread(&b0,div,1,source);
                if (a & 0x80) {
                    b0 ^= 1;
                }
                fwrite(&b0,div,1,target);
                a<<=1;
            }
        }
        fread(&b0,div,1,source);
        b0^=2;
        fwrite(&b0,div,1,target);
        for(i=0;i< (size0 - size1*type - space -((div)-1));i++) {
            fscanf(source,"%c",&a);
            fprintf(target,"%c",a);
        }
    }
    else {
        switch(space){
        case 55:
            remove(strcat((char*)ar3,".bmp"));
            break;
        case 59:
            remove(strcat((char*)ar3,".wav"));
            break;
        default:
            break;
        }
        return 3;
    }
    fclose(source);
    fclose(target);
    fclose(txt);
    return 0;
}
int Engine::unhide(const char *ar1, const char *ar2,const char* ar3){
    int i,space = 55,type = 8,div;
#ifdef __linux__
    FILE* source=fopen(ar2,"r");
    FILE* target=fopen(ar1,"r");
    FILE* txt=fopen(strcat((char*)ar3,".txt"),"w+");
#endif
#ifdef __WIN32
    FILE* source=fopen(ar2,"rb");
    FILE* target=fopen(ar1,"rb");
    FILE* txt=fopen(strcat((char*)ar3,".txt"),"w+b");
#endif
    if(source == NULL || target == NULL || txt == NULL){
        return 1;
    }
    unsigned char b=0,c=0;
    unsigned short a0=0,b0=0;
    int size0=size_of_file(source),k=0;
    if(size0 != size_of_file(target)) return 6;
    if(fread(&b0,2,1,source) != 1 )
        return 1;
    switch(b0){
    case 0x4952 : {
            fseek(source,34,SEEK_SET);
            fscanf(source,"%c",&b);
            type = b;
            fseek(source,20,SEEK_SET);
            fread(&b,1,1,source);
            if(b!=1) return 4;
            space=59;
            break;
        }
    case 0x4d42 :{
            fseek(target,28,SEEK_SET);
            fread(&b,1,1,target);
            if (b != 0x18) return 5;
            fseek(source,28,SEEK_SET);
            fread(&b,1,1,source);
            if (b != 0x18) return 5;
            fseek(source,29,SEEK_SET);
            fread(&b,1,1,source);
            if (b != 0) return 4;
            space=55;
            break;
        }
    default:{
            return 2;
        }
    }
    div = type/8;
    fseek(source,space-1,SEEK_SET);
    fseek(target,space-1,SEEK_SET);
    for (i = 0; i < size0-space; i++) {
        fread(&a0,div,1,source);
        fread(&b0,div,1,target);
        if ((a0 ^ b0) == 2 ){
            break;
        }
        c ^= a0 ^ b0 ;
        k++;
        if( k == 8) {
            fprintf(txt,"%c",c);
            c=0;
            k=0;
        }
        c<<=1;
    }
    fclose(source);
    fclose(target);
    fclose(txt);
    return 0;
}
