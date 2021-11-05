#define arrSize 100

int main()
{   
    int array[arrSize] = {};

    for (int i = 0; i < arrSize; ++i)
        array[i] = i;
    
    int sum = 0;

    for (int i = 0; i < arrSize; ++i)
        sum += array[i];
    

    return 0;
}