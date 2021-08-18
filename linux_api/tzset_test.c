#include <stdio.h> 
#include <time.h>

int main(int argc, char const *argv[])
{
    time_t td;

    putenv("TZ=PST8PDT");
    tzset();

    // 返回的是1970-01-01 00：00：00：00 到现在的相对时间
    time(&td);

    // localtime是根据不同时区换算出来的绝对时间
    printf("current time=%s\n", asctime(localtime(&td)));

    return 0;
}
