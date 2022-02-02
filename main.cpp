//
//  main.cpp
//  T8
//
//  Created by Михаил Фокин on 02.02.2022.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
enum class Choice
{
    READ_FORM_FILE = 1,
    MANAL_ENTRY
};

struct Task {
    double p;
    std::vector<double> ai;
    std::vector<double> gi;
    std::vector<double> ni;
    
    Task(): p(0.0) { }
    
    void getOSNR() {
        const size_t count = ai.size();
        double startP = p;
        double noise = 0;
        for (size_t i = 0; i < count; i++) {
            std::cout << "ai[" << i << "] = " << ai[i] << std::endl;
            std::cout << "gi[" << i << "] = " << gi[i] << std::endl;
            std::cout << "ni[" << i << "] = " << ni[i] << std::endl;
            std::cout << "Разница между затуханием и усилением на " << i << " участке." << std::endl;
            double delta = ai[i] - gi[i];
            std::cout << "ai[" << i << "] - gi[" << i << "] = " << delta << std::endl;
            
            std::cout << "Мощность входного сигнала на " << i << " участке." << std::endl;
            startP = pow(10, std::log10(startP) - delta / 10);
            std::cout << "P[" << i << "] = " << startP << " дБм"<< std::endl;
            
            std::cout << "Мощность шума на " << i << " участке." << std::endl;
            noise += ni[i];
            std::cout << "noise = " << noise << " дБм"<< std::endl;
            std::cout << "Мощность шума на " << i << " участке после затухания и усиления." << std::endl;
            noise = pow(10, std::log10(noise) - delta / 10);
            std::cout << "noise = " << noise << " дБм"<< std::endl;
            std::cout << std::endl;
        }
        std::cout << "Мощность оптического сигнала на выходе Pвых: " << startP << std::endl;
        std::cout << "Мощность накомленного шума Pn.вых: " << noise << std::endl;
        std::cout << "Отношение в логарифмических еденицах (дБ) osnr = Pвых - Pn.вых:" << std::endl;
        double osnr = 10 * std::log10(startP / noise);
        std::cout << "osnr = " << osnr << " дБ"<< std::endl;
    }
};

void printVector(const std::vector<double> &vec) {
    for (const double &elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;
}

void printTable(const Task *task) {
    std::cout << "a: ";
    printVector(task->ai);
    std::cout << "g: ";
    printVector(task->gi);
    std::cout << "n: ";
    printVector(task->ni);
}

Task *readFromFile() {
    
    std::cout << "Введите имя файла:" << std::endl;
    std::string fileName;
    std::cin >> fileName;
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Error open file " << fileName << std::endl;
        return NULL;
    }
    Task *task = new Task();
    std::string temp;
    getline(in, temp);
    while (std::getline(in, temp)) {
        //std::cout << "{" << temp << "}" << std::endl;
        std::istringstream ss(temp);
        double num;
        char buf;
        ss >> num;
        ss >> buf;
        //std::cout << "{" << num << "}" << "[" << buf << "]" << std::endl;
        task->ai.push_back(num);
        ss >> num;
        ss >> buf;
        //std::cout << "{" << num << "}" << "[" << buf << "]" << std::endl;
        task->gi.push_back(num);
        ss >> num;
        ss >> buf;
        //std::cout << "{" << num << "}" << "[" << buf << "]" << std::endl;
        task->ni.push_back(num);
    }
    printTable(task);
    return task;
}

Task *manualEntry() {
    std::cout << "Введите количество пролетов: " << std::endl;
    int count;
    std::cin >> count;
    Task *task = new Task();
    for (int i = 0; i < count; i++) {
        double elem;
        std::cout << "Введите ai[" << i << "]" << std::endl;
        std::cin >> elem;
        task->ai.push_back(elem);
        std::cout << "Введите gi[" << i << "]" << std::endl;
        std::cin >> elem;
        task->gi.push_back(elem);
        std::cout << "Введите ni[" << i << "]" << std::endl;
        std::cin >> elem;
        task->ni.push_back(elem);
    }
    printTable(task);
    return task;
}

int main(int argc, const char *argv[]) {
    std::cout << "Введите мощность сигнала на входе в дБм" << std::endl;
    double p;
    std::cin >> p;
    std::cout << "Выберите действие для чтения ai, gi, ni:" << std::endl;
    std::cout << static_cast<int>(Choice::READ_FORM_FILE) << " Чтение из файла" << std::endl;
    std::cout << static_cast<int>(Choice::MANAL_ENTRY) << " Ручной ввод" << std::endl;
    int input;
    std::cin >> input;
    const auto choice = static_cast<Choice>(input);
    Task *task = NULL;
    switch (choice)
    {
        case Choice::READ_FORM_FILE:
        {
            task = readFromFile();
            break;
        }
        case Choice::MANAL_ENTRY:
        {
            task = manualEntry();
            break;
        }
        default:
        {
            std::cout << "Неверный ввод" << std::endl;
        }
    }
    if (task == NULL)
        exit(-1);
    task->p = p;
    task->getOSNR();
    delete task;
    return 0;
}
