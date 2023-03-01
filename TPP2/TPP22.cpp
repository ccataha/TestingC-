#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

const int PRECISION_ROW = 5;
const int PRECISION_COL = 5;
const double K_MAX_ALFA[] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
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
int metod_MaxAO(double *data, const size_t n, int precision_row, int precision_col, double *KmaxAlfa)
{
    double avr, sko, disp, maxAbs = 0;
    int maxindex = 0;

    calc_stat(data, n, avr, disp, sko);

    for (size_t h1 = 0; h1 < n; h1++)
    {
        double tmp = abs(data[h1] - avr);
        if (tmp > maxAbs) { maxAbs = tmp; maxindex = h1; }
    }

    double tau4 = maxAbs / sko;

    if (KmaxAlfa[precision_row * precision_col] < tau4)
    {
        return maxindex;
    }

    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " filename.txt" << endl;
        return 1;
    }

    // Открытие файла
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Error opening file " << argv[1] << endl;
        return 1;
    }

    // Считывание значений из файла
    vector<double> data;
    double value;
    while (input >> value) {
        data.push_back(value);
    }

    // Закрытие файла
    input.close();

    // Определение выбросов
    vector<int> outliers;
    for (size_t i = 0; i < data.size(); i++) {
        if (metod_MaxAO(&data[0], data.size(), PRECISION_ROW, PRECISION_COL, const_cast<double*>(K_MAX_ALFA)) == i) {
            outliers.push_back(i);
        }
    }

    // Вывод результатов
    cout << "Original data:" << endl;
    for (size_t i = 0; i < data.size(); i++) {
        cout << setw(8) << fixed << setprecision(2) << data[i];
        if ((i + 1) % 10 == 0) {
            cout << endl;
        }
    }
    if (data.size() % 10 != 0) {
        cout << endl;
    }
    cout << "Outliers:" << endl;
    for (size_t i = 0; i < outliers.size(); i++) {
        cout << outliers[i] << " ";
    }
    cout << endl;

    return 0;
}
