#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

const int PRECISION_ROW = 2;
const int PRECISION_COL = 2;
const double K_MAX_ALFA[] = {
    1.00, 1.00, 1.00, 1.00, 1.00, 
    1.00, 1.00, 1.00, 1.00, 1.00, 
    1.00, 1.00, 1.00, 1.00, 1.00, 
    1.00, 1.00, 1.00, 1.00, 1.00
};

// Функция для вычисления среднего, дисперсии и среднеквадратического отклонения
void calc_stat(double *data, const size_t n, double &avr, double &disp, double &sko)
{
    double sum = 0;

    for (size_t h1 = 0; h1 < n; h1++) // Сумма элементов
    {
        sum = sum + data[h1];
    }
    avr = sum / n; // Среднее арифметическое

    sum = 0;
    for (size_t h1 = 0; h1 < n; h1++) // Сумма квадратов отклонений от среднего
    {
        sum = sum + (data[h1] - avr) * (data[h1] - avr);
    }
    disp = sum / n; // Дисперсия
    sko = sqrt(disp); // Среднеквадратическое отклонение
}

// Функция для вычисления выбросов методом Максимума модуля отклонения
vector<int> metod_MaxAO(double *data, const size_t n, const double threshold)
{
    double avr, sko, disp;

    calc_stat(data, n, avr, disp, sko);

    vector<int> outliers;
    for (size_t h1 = 0; h1 < n; h1++)
    {
        double tmp = abs(data[h1] - avr);
        double tau4 = tmp / sko;
        if (tau4 > threshold) {
            outliers.push_back(h1);
        }
    }

    return outliers;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " filename.txt" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Error opening file " << argv[1] << endl;
        return 1;
    }

    // Считываем значения из файла в вектор data
    vector<double> data;
    double value;
    while (input >> value) {
        data.push_back(value);
    }
    input.close();

    // Вычисляем выбросы и сохраняем их индексы в векторе outliers
    vector<int> outliers = metod_MaxAO(&data[0], data.size(), K_MAX_ALFA[PRECISION_COL]);
    cout << "Method: MaxAO" << endl ;
    // Печатаем значения входных данных
    cout << "Input data: ";
    for (size_t i = 0; i < data.size(); i++) {
        cout << data[i] << " ";
    }
    cout << endl;

    // Печатаем индексы всех выбросов
    if (outliers.empty()) {
        cout << "No outliers detected." << endl;
    } else {
        cout << "Outliers detected at indices: ";
        for (size_t i = 0; i < outliers.size(); i++) {
            cout << outliers[i] << " ";
        }
        cout << endl;
    }

    return 0;
}
