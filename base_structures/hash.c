hashstr(s, max)
char *s;
{
    int res;
    
    for (res=0;*s;s++)
        res += *s;    
    return res%max;
}
