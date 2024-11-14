#include <stdio.h>
int main()
{
    char a = 127;
    unsigned char b = 0xff;
    unsigned char c = 0;
    int d = 65;
    a++;
    b++;
    c--;
    printf("a=%d\nb=%d\nc=%d\nd=%c\n",a,b,c,d);
    return 0;
}