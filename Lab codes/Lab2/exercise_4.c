#include <stdio.h>
int main()
{
    char a;
    int b;
    float c;
    printf("Please input a character:");
    scanf("%c",&a);

    printf("Please input an integer:");
    scanf("%d",&b);

    printf("Please input a float:");
    scanf("%f",&c);

    printf("The variables you entered were:\n");

    printf("The character is:%c \n",a);
    printf("The integer is:%d \n",b);
    printf("The float is:%f \n",c);
}