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
                printf("%15f", array[i][j].dataValue);
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }
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
float *generatePatternForP(float p, int size, bool check)
{
    int i = 1;
    float current = 0;
    float *pValueArray = (float *)malloc(sizeof(float) * size);
    int index = 0;
    for (; i <= size; i++)
    {
        if (check == false)
        {
            current = p - i;
        }
        else
        {
            current = p + i;
        }
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
    float *arr = generatePatternForP(p, row - 2, false);
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
    float *arr = generatePatternForP(p, row - 2, true);
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
float *generatePatternForUnequal(float interpolateValue, int size, struct Data **arr, int exclude)
{
    int i = 0;
    float *xValueArray = (float *)malloc(sizeof(float) * size);
    int index = 0;
    for (; i < size; i++)
    {
        if (i != exclude)
        {
            float current = interpolateValue - arr[i][0].dataValue;
            if (index - 1 >= 0)
            {
                current = current * xValueArray[index - 1];
            }
            xValueArray[index] = current;
            index++;
        }
    }
    return xValueArray;
}

float lagrange(struct Data **array, int row, float interpolateValue)
{

    float answer = 0;
    int i = 0;
    for (; i < row; i++)
    {
        float *numerator = generatePatternForUnequal(interpolateValue, row, array, i);
        float *denominator = generatePatternForUnequal(array[i][0].dataValue, row, array, i);
        answer += ((numerator[row - 2] / denominator[row - 2]) * array[i][1].dataValue);
    }
    return answer;
}

float nfDividedDifference(struct Data **array, int row, int col, float interpolateValue)
{
    float *arr = generatePatternForUnequal(interpolateValue, row, array, -1);
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
void printLine()
{
    int i = 0;
    for (; i < 50; i++)
    {
        printf("-");
    }
    printf("\n");
}

void printLineTable()
{
    int i = 0;
    for (; i < 100; i++)
    {
        printf("-");
    }
    printf("\n");
}

void setIndex(struct Data **array, int smallestPValue, int row, int col)
{
    int j = 0;
    for (; j < col; j++)
    {
        int i = 0;
        int currentIndex = smallestPValue;
        for (; i < row; i++)
        {
            array[i][j].index = currentIndex;
            currentIndex++;
        }
    }
}
int findX0Index(struct Data **array, int row, float interpolateValue, float lowerlimit, float upperlimit, float *p)
{
    float diff = array[1][0].dataValue - array[0][0].dataValue;
    int i = 0;
    for (; i < row; i++)
    {
        *p = (interpolateValue - array[i][0].dataValue) / diff;
        if (*p >= lowerlimit && *p <= upperlimit)
        {
            return i;
        }
    }
    return 0;
}

float returnYValue(struct Data **arr, int row, int col, int arrIndex, bool *breakLoop)
{
    int i = 0;
    for (; i < row - col + 1; i++)
    {
        if (arr[i][col].index == arrIndex)
        {
            *breakLoop = false;
            return arr[i][col].dataValue;
        }
    }
    *breakLoop = true;
    return -1;
}

float *generatePatternForGF(float p, int size, bool check)
{
    int i = 0;
    bool alternate = true;
    int minus = 1;
    int plus = 1;
    float *pValueArray = (float *)malloc(sizeof(float) * size);
    int index = 0;
    float current = 0;
    for (; i < size; i++)
    {
        if (alternate == check)
        {
            current = p - minus;
            minus++;
            if (check == false)
            {
                alternate = true;
            }
            else
            {
                alternate = false;
            }
        }
        else
        {
            current = p + plus;
            plus++;
            if (check == false)
            {
                alternate = false;
            }
            else
            {
                alternate = true;
            }
        }
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

float gaussForward(struct Data **array, int row, int col, float interpolateValue)
{
    float p = 0;
    bool breakLoop = false;
    int cindex = -1;
    int count = 0;
    int index = findX0Index(array, row, interpolateValue, 0, 1, &p);
    setIndex(array, -1 * index, row, col);
    float *pArr = generatePatternForGF(p, row - 2, true);
    int *farr = factorial(row - 2);
    float answer = returnYValue(array, row, 1, 0, &breakLoop);
    if (breakLoop == true)
    {
        return 0;
    }
    float temp = p * returnYValue(array, row, 2, 0, &breakLoop);
    if (breakLoop == true)
    {
        return answer;
    }
    answer += temp;
    int i = 3;
    index = 0;
    for (; i < col && breakLoop == false; i++)
    {
        temp = returnYValue(array, row, i, cindex, &breakLoop);
        count++;
        if (count % 2 == 0)
        {
            cindex = cindex - 1;
        }
        if (breakLoop == true)
        {
            return answer;
        }
        answer += temp * pArr[index] / farr[index];
        index++;
    }
    return answer;
}

float gaussBackward(struct Data **array, int row, int col, float interpolateValue)
{
    float p = 0;
    bool breakLoop = false;
    int cindex = -1;
    int count = 1;
    int index = findX0Index(array, row, interpolateValue, -1, 0, &p);
    setIndex(array, -1 * index, row, col);
    float *pArr = generatePatternForGF(p, row - 2, false);
    int *farr = factorial(row - 2);
    float answer = returnYValue(array, row, 1, 0, &breakLoop);
    if (breakLoop == true)
    {
        return 0;
    }
    float temp = p * returnYValue(array, row, 2, -1, &breakLoop);
    if (breakLoop == true)
    {
        return answer;
    }
    answer += temp;
    int i = 3;
    index = 0;
    for (; i < col && breakLoop == false; i++)
    {
        temp = returnYValue(array, row, i, cindex, &breakLoop);
        count++;
        if (count % 2 == 0)
        {
            cindex = cindex - 1;
        }
        if (breakLoop == true)
        {
            return answer;
        }
        answer += temp * pArr[index] / farr[index];
        index++;
    }
    return answer;
}
float sterling(struct Data **array, int row, int col, float interpolateValue)
{
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
        printLineTable();
        createTableEqual(obj, totalDataSet, totalDataSet + 1);
        print(obj, totalDataSet);
        printLineTable();
        printf("\n\n");
        printf("%s", "Equal Interval Formulas\n");
        printLine();
        float nfAnswer = newtonForward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%s %f\n\n", "Newton Forward Difference Formula:  ", nfAnswer);
        printLine();
        float nbAnswer = newtonBackward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%s %f\n\n", "Newton Backward Difference Formula: ", nbAnswer);
        printLine();
        float lagrangeAnswer = lagrange(obj, totalDataSet, interpolateValue);
        printf("%s %f\n\n", "Lagrange Formula:                   ", lagrangeAnswer);
        printLine();
        float gfAnswer = gaussForward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%s %f\n\n", "Gauss Forward Difference Formula:   ", gfAnswer);
        printLine();
        float gbAnswer = gaussBackward(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%s %f\n\n", "Gauss Backward Difference Formula:  ", gbAnswer);
        printLine();
        // float sAnswer=sterling(obj,totalDataSet,totalDataSet+1,interpolateValue);
        // printf("%s %f\n\n","Sterling Formula:                   ",sAnswer);
        // printLine();
    }
    else
    {
        system("clear");
        printLineTable();
        createTableUnequal(obj, totalDataSet, totalDataSet + 1);
        print(obj, totalDataSet);
        printLineTable();
        printf("\n\n");
        printf("%s", "Unequal Interval Formulas\n");
        printLine();
        float nfddAnswer = nfDividedDifference(obj, totalDataSet, totalDataSet + 1, interpolateValue);
        printf("%s %f\n\n", "Newton Divided Difference Formula: ", nfddAnswer);
        printLine();
        float lagrangeAnswer = lagrange(obj, totalDataSet, interpolateValue);
        printf("%s %f\n\n", "Lagrange Formula:                  ", lagrangeAnswer);
        printLine();
    }

    return 0;
}
