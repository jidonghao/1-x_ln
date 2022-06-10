#include <stdio.h>
#include <stdlib.h>
void Fun(int c){
    int d = c+2;
    printf("c=%d\n",d);
}

/***
 * 
 */
void show(){

printf("aaaaaaaaaaaaaa");

}
void tryFun(int a,void(*Fun2)(int b),void(*fun3)()){
    Fun2(a);
    printf("a=%d\n",a);
    fun3();
}




void main(){

    printf("helo world!\n");

tryFun(3,Fun,show);
    system("pause");
}