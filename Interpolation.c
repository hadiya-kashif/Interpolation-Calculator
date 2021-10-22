#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct Data
{
    int index;
    float dataValue;
};
void print(struct Data **array, int size)
{
    int i = 0;
    for (; i < size; i++)
    {
        int j = 0;
        for (; j < size + 1; j++)
        {
            if (array[i][j].dataValue != INT_MIN)
            {
                printf("%f  ", array[i][j].dataValue);
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void setValues(int index, float dataValue, struct Data *arr)
{
    arr->index = index;
    arr->dataValue = dataValue;
}
struct Data *colToRow(struct Data **arr, int col, int row)
{
    struct Data *array = (struct Data *)malloc(sizeof(struct Data) * (row));
    int i = 0;
    for (; i < row; i++)
    {
        array[i] = arr[i][col];
    }
    return array;
}
void subtract(struct Data *arr, struct Data **array, int col, int size)
{
    int yValue1 = 0;
    int yValue2 = 1;
    while (yValue2 <= size)
    {
        array[yValue1][col].dataValue = arr[yValue2].dataValue - arr[yValue1].dataValue;
        array[yValue1][col].index = yValue1;
        yValue1++;
        yValue2++;
    }
}
void createTable(struct Data **arr, int rows, int col)
{
    int i = 1;
    for (; i < col; i++)
    {
        struct Data *array = (struct Data *)malloc(sizeof(struct Data) * (rows));
        array = colToRow(arr, i, rows);

        subtract(array, arr, i + 1, rows - i);
    }
}

void setArrayTo0(struct Data **array, int size)
{
    int i = 0;
    for (; i < size; i++)
    {
        int j = 0;
        for (; j < size + 1; j++)
        {
            array[i][j].dataValue = INT_MIN;
        }
    }
}
float *generatePatternForPNF(float p, int size)
{
    int i = 1;
    float *pValueArray = (float *)malloc(sizeof(float) * size);
    int index = 0;
    for (; i <= size; i++)
    {
        float current = p - i;
        if (index - 1 >= 0)
        {
            current = current * pValueArray[index - 1];
        }
        else
        {
            current = current * p;
        }
        pValueArray[index] = current;
        index++;
    }
    return pValueArray;
}
float *generatePatternForPNB(float p, int size)
{
    int i = 1;
    float *pValueArray = (float *)malloc(sizeof(float) * size);
    int index = 0;
    for (; i <= size; i++)
    {
        float current = p + i;
        if (index - 1 >= 0)
        {
            current = current * pValueArray[index - 1];
        }
        else
        {
            current = current * p;
        }
        pValueArray[index] = current;
        index++;
    }
    return pValueArray;
}
int *factorial(int size)
{
    int i = 3;
    int *array = (int *)malloc(sizeof(int) * size);
    array[0] = 2;
    int index = 1;
    for (; index < size; index++)
    {
        array[index] = array[index - 1] * i;
        i++;
    }
    return array;
}
float newtonForward(struct Data **array, int row, int col, float interpolateValue)
{
    float diff = array[1][0].dataValue - array[0][0].dataValue;
    float p = (interpolateValue - array[0][0].dataValue) / diff;
    float *arr = generatePatternForPNF(p, row - 2);
    int *farr = factorial(row - 2);
    float answer = array[0][1].dataValue + p * array[0][2].dataValue;
    int i = 3;
    int index = 0;
    for (; i < col; i++)
    {
        answer += (arr[index] * array[0][i].dataValue) / farr[index];
        index++;
    }
    return answer;
}
float newtonBackward(struct Data **array, int row, int col, float interpolateValue)
{
    float diff = array[1][0].dataValue - array[0][0].dataValue;
    float p = (interpolateValue - array[row - 1][0].dataValue) / diff;
    float *arr = generatePatternForPNB(p, row - 2);
    int *farr = factorial(row - 2);
    float answer = array[row - 1][1].dataValue + p * array[row - 2][2].dataValue;
    int i = 3;
    int index = 0;
    for (; i < col; i++)
    {
        answer += (arr[index] * array[row - i][i].dataValue) / farr[index];
        index++;
    }
    return answer;
}

int main()
{
    system("clear");
    int totalDataSet;
    printf("%s", "Enter the number of dataset values: ");
    scanf("%d", &totalDataSet);
    struct Data *obj[totalDataSet];
    int i = 0;
    for (; i < totalDataSet; i++)
    {
        obj[i] = (struct Data *)malloc(sizeof(struct Data) * (totalDataSet + 1));
    }
    setArrayTo0(obj, totalDataSet);
    i = 0;
    for (; i < totalDataSet; i++)
    {
        float xValue;
        float yValue;
        printf("%s", "Enter value of x: ");
        scanf("%f", &xValue);
        printf("%s", "Enter value of y: ");
        scanf("%f", &yValue);
        setValues(i, xValue, &obj[i][0]);
        setValues(i, yValue, &obj[i][1]);
    }
    float interpolateValue;
    printf("%s", "Enter the value on which you want to interpolate: ");
    scanf("%f", &interpolateValue);
    print(obj, totalDataSet);
    createTable(obj, totalDataSet, totalDataSet + 1);
    print(obj, totalDataSet);
    float nfAnswer = newtonForward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
    printf("%f\n", nfAnswer);
    float nbAnswer = newtonBackward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
    printf("%f\n", nbAnswer);

    return 0;
}