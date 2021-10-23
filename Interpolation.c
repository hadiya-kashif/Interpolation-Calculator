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

bool equalOrUnequal(struct Data **arr, int size)
{
    bool equal = true;
    int xValue1 = 0;
    int xValue2 = 1;
    float difference = arr[xValue2][0].dataValue - arr[xValue1][0].dataValue;
    while (xValue2 < size)
    {
        if (difference != arr[xValue2][0].dataValue - arr[xValue1][0].dataValue)
        {
            equal = false;
            break;
        }
        xValue1++;
        xValue2++;
    }
    return equal;
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
void subtractEqual(struct Data *arr, struct Data **array, int col, int size)
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
void subtractUnequal(struct Data *arr, struct Data **array, int col, int size, int xValue1, int xValue2)
{
    int yValue1 = 0;
    int yValue2 = 1;
    while (yValue2 <= size)
    {
        array[yValue1][col].dataValue = (arr[yValue2].dataValue - arr[yValue1].dataValue) /
                                        (array[xValue2][0].dataValue - array[xValue1][0].dataValue);
        array[yValue1][col].index = yValue1;
        yValue1++;
        yValue2++;
        xValue2++;
        xValue1++;
    }
}
void createTableEqual(struct Data **arr, int rows, int col)
{
    int i = 1;
    for (; i < col; i++)
    {
        struct Data *array = (struct Data *)malloc(sizeof(struct Data) * (rows));
        array = colToRow(arr, i, rows);

        subtractEqual(array, arr, i + 1, rows - i);
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
void createTableUnequal(struct Data **arr, int rows, int col)
{
    int i = 1;
    int xValue1 = 0;
    int xValue2 = 1;
    for (; i < col; i++)
    {
        struct Data *array = (struct Data *)malloc(sizeof(struct Data) * (rows));
        array = colToRow(arr, i, rows);

        subtractUnequal(array, arr, i + 1, rows - i, xValue1, xValue2);
        xValue2++;
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
float *generatePatternForNFDD(float interpolateValue, int size, struct Data **arr)
{
    int i = 0;
    float *xValueArray = (float *)malloc(sizeof(float) * size);
    for (; i < size; i++)
    {
        float current = interpolateValue - arr[i][0].dataValue;
        if (i - 1 >= 0)
        {
            current = current * xValueArray[i - 1];
        }
        xValueArray[i] = current;
    }
    return xValueArray;
}

float nfDividedDifference(struct Data **array, int row, int col, float interpolateValue)
{
    float *arr = generatePatternForNFDD(interpolateValue, row, array);
    float answer = array[0][1].dataValue;
    int i = 2;
    int index = 0;
    for (; i < col; i++)
    {
        answer += (arr[index] * array[0][i].dataValue);
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
    bool h = equalOrUnequal(obj, totalDataSet);
    if (h == true)
    {
        system("clear");
        printf("%s", "Equal Interval\n");
        createTableEqual(obj, totalDataSet, totalDataSet + 1);
        print(obj, totalDataSet);
        float nfAnswer = newtonForward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%f\n", nfAnswer);
        float nbAnswer = newtonBackward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%f\n", nbAnswer);
    }
    else
    {
        system("clear");
        printf("%s", "Unequal Interval\n");
        createTableUnequal(obj, totalDataSet, totalDataSet + 1);
        print(obj, totalDataSet);
        float nfddAnswer = nfDividedDifference(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%f\n", nfddAnswer);
    }

    return 0;
}
