#include "../include/Application.h"
#include <iostream>

int main() {
    // Создание и инициализация приложения
    Application app;
    
    if (!app.initialize()) {
        std::cerr << "Не удалось инициализировать приложение" << std::endl;
        return -1;
    }
    
    std::cout << "Программа для формирования навыков пилотирования по приборам запущена" << std::endl;
    std::cout << "Управление:" << std::endl;
    std::cout << "- Джойстик Logitech Extreme 3D Pro (основное управление)" << std::endl;
    std::cout << "- Стрелки клавиатуры: крен и тангаж" << std::endl;
    std::cout << "- Q/E: руль направления" << std::endl;
    std::cout << "- W/S: газ" << std::endl;
    std::cout << "- ESC: выход из программы" << std::endl;
    
    // Запуск главного цикла приложения
    app.run();
    
    std::cout << "Программа завершена" << std::endl;
    
    return 0;
}