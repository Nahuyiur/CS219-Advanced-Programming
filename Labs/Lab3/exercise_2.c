#include<stdio.h>
union data{
    int n;
    char ch;
    short m;
};
int main(){
    union data a;
    printf("%lu, %lu\n", sizeof(a), sizeof(union data) ); 
    a.n = 0x40;
    printf("%X, %c, %hX\n", a.n, a.ch, a.m);
    a.ch = '9';//ASCII值是0x39
    printf("%X, %c, %hX\n", a.n, a.ch, a.m);
    a.m = 0x2059;//0x59(十进制是89)对应Y
    //低字节在低地址处
    printf("%X, %c, %hX\n", a.n, a.ch, a.m);
    a.n = 0x3E25AD54;
    //0x54对应T,a.m占用两个字节 0x54AD
    printf("%X, %c, %hX\n", a.n, a.ch, a.m);
    return 0;
}