long factorial(int factNum);

int main()
{
    int factNum = 10;

    long result = factorial(factNum);

    return 0;
} 

long factorial(int factNum)
{
    if (factNum == 1)
        return 1;
        
    return factNum * factorial(factNum - 1);
}
