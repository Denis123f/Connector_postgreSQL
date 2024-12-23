#include <iostream>
#include <ctime>
#include <pqxx/pqxx>

int main() {
    constexpr const char* db_host = "localhost"; // Локальный хост используется, если на Вашем пк запущена бд
    constexpr const char* db_user = "..."; // Пользователь, что заходит в базу данных
    constexpr const char* db_pass = "..."; // Пароль для входа
    constexpr const char* db_name = "..."; // база данных к которой присоединяемся
    std::string registers, name, password;
    try {
        //Создание коннектора 
        pqxx::connection conn("dbname=" + std::string(db_name) +
                              " user=" + std::string(db_user) +
                              " password=" + std::string(db_pass) +
                              " host=" + std::string(db_host));
        // Проверка подключения
        if(conn.is_open()){
            pqxx::work work(conn);
            std::cout << "Если вы зарегистрированы, то введите имя:\nИли введите register чтобы пройти регистрацию:";
            std::cin >> registers;
            
            if(registers == "register" ) {
                std::cout << "Укажите имя и пароль для регистрациия:";
                std::cin >> name >> password; 
                std::string zapros {"insert into admins values("};
                work.exec0(zapros + '\'' + name + '\'' + ", " + password + ")"); 
                work.commit();
                std::cout << "Ваши данные добавлены и вы сможете авторизоваться."; 
                
            }else
            {
                pqxx::result res = work.exec("select name, login from admins");
                std::cout << "Введите пароль: ";
                std::cin >> password;
                name = registers;
                bool check_reg = 0;
                for(const auto& a: res){
                    if(a[0].as<std::string>() == name && a[1].as<std::string>() == password){
                        check_reg = 1;
                    }
                }
                if(check_reg) std::cout << "Добро пожаловать";
                else std::cout << "Неверно указан логин или пароль";
            }
        }else std::cout << "Данные введены не корректно";
    } catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка подключения к базе данных: " << e.what() << std::endl;
        return 1;
    } catch (std::exception e){
        std::cerr << "Возникла ошибка при работе: " << e.what() << std::endl; 
    }
    return 0;
}
