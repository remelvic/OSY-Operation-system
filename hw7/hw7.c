#define STDIN_FILENO    0   /* Standard input.  */
#define STDOUT_FILENO   1   /* Standard output.  */
 
void my_exit(int code){
    asm volatile("int $0x80" : : "a"(1), "b" (code) );
}
 
int my_read(int STDIN, char* buffer, int buffer_size){
    unsigned ret;
    asm volatile("int $0x80" : "=a" (ret) : "a"(3), "b" (STDIN), "c" (buffer), "d" (buffer_size) : "memory");
    return ret;
}
int my_write(int STDOUT, char*buffer, int len){
    unsigned ret;
    asm volatile("int $0x80" : "=a" (ret) : "a"(4), "b" (STDOUT), "c" (buffer), "d" (len) : "memory");
    return ret;
}
 
int isnum(char ch)
{
    return ch >= '0' && ch <= '9';
}
 
int isspc(char ch)
{
    return ch == ' ' || ch == '\n';
}
 
static void print(unsigned num)
{
    char buf[20];
    int count = 0;
    char my_number[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    if (num == 0){
        buf[count++] = '0';
    }
    while(num){
        buf[count++] = my_number[num%16];
        num /= 16;
    }
    char reverse[count];
    for(int i=0; i<count; i++){
        reverse[count-i-1] = buf[i];
    }
    reverse[count] = '\0';
    //sprintf(buf, "0x%x\n", num);
     
     
     
    char start[] = "0x";
    char end[] ="\n";
    int str = my_write(STDOUT_FILENO,start,3);
    if(str == -1){
        my_exit(1);
    }
 
    int ret = my_write(STDOUT_FILENO, reverse, count);
    if (ret == -1)
        my_exit(1);
 
    int res = my_write(STDOUT_FILENO,end,2);
    if(res == -1){
        my_exit(1);
    }
}
 
 
int _start()
{
    char buf[20];
    unsigned num = 0;
    int i;
    int num_digits = 0;
    unsigned chars_in_buffer = 0;
 
    for (;; i++, chars_in_buffer--) {
        if (chars_in_buffer == 0) {
            int ret = my_read(STDIN_FILENO, buf, sizeof(buf));
            if (ret < 0)
                my_exit(1);
            i = 0;
            chars_in_buffer = ret;
        }
        if ( num_digits > 0 && (chars_in_buffer == 0 || !isnum(buf[i]) ) ) {
            print(num);
            num_digits = 0;
            num = 0;
        }
        if ( chars_in_buffer == 0 || (!isspc(buf[i]) && !isnum(buf[i]) ) )
            my_exit(0);
 
        if (isnum(buf[i])) {
            num = num * 10 + buf[i] - '0';
            num_digits++;
        }
    }
}