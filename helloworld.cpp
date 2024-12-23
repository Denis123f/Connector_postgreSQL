#include <iostream>
#include <ctime>
#include <pqxx/pqxx>

int main() {
    /*g++ -o gnida helloworld.cpp -I/usr/include -I/usr/include/libpqxx-7.9.2 -L/usr/lib/x86_64-linux-gnu -lpqxx -lpq*/
    // Задайте параметры подключения
    constexpr const char* db_host = "localhost";
    constexpr const char* db_user = "misha";
    constexpr const char* db_pass = "denis";
    constexpr const char* db_name = "newT";
    std::string registers, name, password;
    try {
        pqxx::connection conn("dbname=" + std::string(db_name) +
                              " user=" + std::string(db_user) +
                              " password=" + std::string(db_pass) +
                              " host=" + std::string(db_host));
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
            }else{
                std::cout << "ЧЕ БЛЯ\n\n\n\n"; 
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
