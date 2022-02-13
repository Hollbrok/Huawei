int main()
{   
    int array[100];

    for (int i = 0; i < 100; ++i)
        array[i] = i;
    
    int sum = 0;

    for (int i = 0; i < 100; ++i)
        sum += array[i];
    

    return 0;
}