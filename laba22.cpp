#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float (*SumFunction)(float*, int);
typedef void (*FuncChoice)(float, int, float*);

float reduce_sinx(float x) {
    const float pi_over_2=M_PI/2.0f;
    const float two_pi=2.0f*M_PI;

    x = fmodf(x, two_pi);

    if (x>=pi_over_2) {
        x=M_PI-x;
    }
    if (x>=pi_over_2) {
        x-=M_PI;
    }
    if (x>=pi_over_2) {
        x=M_PI-x;
    }

    return x;
}

float reduce_cosx(float x) {
    const float pi_over_2=M_PI/2.0f;
    const float two_pi=2.0f*M_PI;

    x = fmodf(x, two_pi);

    if (x>M_PI) {
        x=two_pi-x;
    }
    if (x>pi_over_2) {
        x=M_PI-x;
    }

    return x;
}
void sinx(float x, int size, float* chleni_ryada) {
    x = reduce_sinx(x);
    chleni_ryada[0] = x;
    for (int i = 1; i < size; i++) {
        chleni_ryada[i] = ((chleni_ryada[i - 1]) * (-x * x)) / ((2 * i + 1) * (2 * i));
    }
}


void cosx(float x, int size, float* chleni_ryada) {
    x=reduce_cosx(x);
    chleni_ryada[0]=1.0;
    for (int i=1; i<size; i++) {
        chleni_ryada[i] = ((chleni_ryada[i - 1]) * (-x * x)) / ((2 * i) * (2 * i - 1));
    }
}

void expx(float x, int size, float* chleni_ryada) {
    chleni_ryada[0] = 1.0;
    for (int i = 1; i < size; i++) {
        chleni_ryada[i] = chleni_ryada[i - 1] * x / i;
    }
}

void lnx(float x, int size, float* chleni_ryada) {
    chleni_ryada[0] = x;
    for (int i = 1; i < size; i++) {
        chleni_ryada[i] = chleni_ryada[i - 1] * (-(i * x)) / (i + 1);
    }
}

float sum_pryam(float* chleni_ryada, int size) {
    float summ = 0.0;
    for (int i = 0; i < size; i++) {
        summ += chleni_ryada[i];
    }
    return summ;
}

float sum_obr(float* chleni_ryada, int size) {
    float summ = 0.0;
    for (int i = size - 1; i >= 0; i--) {
        summ += chleni_ryada[i];
    }
    return summ;
}

float sum_popar(float* chleni_ryada, int size) {
    if (size == 1) {
        return chleni_ryada[0];
    }

    int new_size = (size + 1) / 2;
    float* new_chleni_ryada = (float*)malloc(new_size * sizeof(float));
    if (new_chleni_ryada == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 0;
    }

    for (int i = 0; i < size; i += 2) {
        if (i + 1 < size) {
            new_chleni_ryada[i / 2] = chleni_ryada[i] + chleni_ryada[i + 1];
        } else {
            new_chleni_ryada[i / 2] = chleni_ryada[i];
        }
    }

    float result = sum_popar(new_chleni_ryada, new_size);
    free(new_chleni_ryada);
    return result;
}

float compute_taylor(float x, int size, float* chleni_ryada, SumFunction sumFunction, FuncChoice func) {
    
    func(x, size, chleni_ryada);

    
    return sumFunction(chleni_ryada, size);
}

int main() {
    int choice_Func, choice_MethSum, size;
    float x;

    SumFunction sumFunctions[] = {sum_pryam, sum_obr, sum_popar};

    printf("Выберите функцию для аргумента:\n1 - sin(x)\n2 - cos(x)\n3 - exp(x)\n4 - ln(1+x)\n");
    scanf("%d", &choice_Func);
    printf("Выберите метод суммирования:\n1 - Прямое суммирование\n2 - Обратное суммирование\n3 - Попарное суммирование\n");
    scanf("%d", &choice_MethSum);
    printf("Введите значение аргумента для функции:\n");
    scanf("%f", &x);
    printf("Введите количество элементов для ряда:\n");
    scanf("%d", &size);

    if (choice_Func < 1 || choice_Func > 4 || choice_MethSum < 1 || choice_MethSum > 3) {
        printf("Неверный выбор.\n");
        return 1;
    }

    float teylor_result = 0.0;
    float math_result = 0.0;
    float* chleni_ryada = (float*)malloc(size * sizeof(float));

    if (chleni_ryada == NULL) {
        printf("Ошибка выделения памяти. Похоже вы выбрали недопустимый размер.\n");
        return 1;
    }

    
    switch (choice_Func) {
        case 1:
            math_result = sin(x);
            break;
        case 2:
            math_result = cos(x);
            break;
        case 3:
            math_result = exp(x);
            break;
        case 4:
            if (x <= -1 || x > 1) {
                printf("Ошибка: Ряд Тейлора для ln(1+x) сходится только при |x|<1\n");
                free(chleni_ryada);
                return 1;
            }
            math_result = log(1 + x);
            break;
    }

    
    FuncChoice selectedFunc;
    switch (choice_Func) {
        case 1:
            selectedFunc = sinx;
            break;
        case 2:
            selectedFunc = cosx;
            break;
        case 3:
            selectedFunc = expx;
            break;
        case 4:
            selectedFunc = lnx;
            break;
    }

    
    SumFunction selectedSumFunction = sumFunctions[choice_MethSum - 1];

    
    teylor_result = compute_taylor(x, size, chleni_ryada, selectedSumFunction, selectedFunc);

    
    printf("Члены ряда Тейлора:\n");
    for (int i = 0; i < size; i++) {
        printf("chleni_ryada[%d] = %.15f\n", i, chleni_ryada[i]);
    }

    printf("Результат (Тейлор): %.15f\n", teylor_result);
    printf("Результат (math.h): %.15f\n", math_result);
    printf("Разница: %.15f\n", fabs(teylor_result - math_result));

    free(chleni_ryada);
    return 0;
}