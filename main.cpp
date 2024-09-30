#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <Windows.h>
#include <tuple>

std::string out_first_name()
{
	std::string first_name = "";
	do
	{
		std::cout << "Enter your first name: ";
		std::cin >> first_name;
	} while (first_name == "");
	return first_name;
}
std::string out_last_name()
{
	std::string last_name = "";
	do
	{
		std::cout << "Enter your last name: ";
		std::cin >> last_name;
	} while (last_name == "");
	return last_name;
}
std::string out_email()
{
	std::string email = "";
	do
	{
		std::cout << "Enter your email: ";
		std::cin >> email;
	} while (email == "");
	return email;
}
std::string out_phoneNumber()
{
	std::string first_number = "+7";
	std::string phoneNumber = "";

	std::cout << "Enter your phone number or leave the field empty: " + first_number;
	std::cin >> phoneNumber;
	return first_number + phoneNumber;
}


struct PersonData
{
public:
	int id;
	std::string first_name;
	std::string last_name;
	std::string email;
	std::string phone_number;
};

class ManagerData
{
public:
	ManagerData()
	{
	}
	
	void CreateClientTable()
	{
		pqxx::transaction tran{ connection};
		std::string query =
			"CREATE TABLE IF NOT EXISTS persondata (id SERIAL PRIMARY KEY, "
			"first_name VARCHAR(60) NOT NULL,"
			"last_name VARCHAR(60) NOT NULL, "
			"email VARCHAR(100) NOT NULL UNIQUE, "
			"phone_number TEXT UNIQUE)";
		tran.exec(query);
		tran.commit();
	}
	int AddClient(std::string& first_name, std::string& last_name, std::string& email, std::string& phone_number)
	{

		pqxx::transaction tran{ connection };
		std::string query = "INSERT INTO persondata(first_name, last_name, email, phone_number)"
			"VALUES('" + tran.esc(first_name) + "', '" + tran.esc(last_name) + "', '" + tran.esc(email) + "', '" + tran.esc(phone_number) + "')";
		tran.exec(query);
		tran.commit();
		std::string query_search_id =
			"SELECT id FROM persondata "
			"WHERE email='" + tran.esc(email) + "'";
		auto result_id = tran.query_value<int>(query_search_id);
		return result_id;
		
	}

	void AddPhoneNumber(int clientId, std::string& phone_number)
	{
		
		
		pqxx::transaction tran{ connection };
		std::string old_phone_number = tran.query_value<std::string>("SELECT phone_number FROM persondata "
			"WHERE id='" + tran.esc(clientId) + "'");
		std::string query = "UPDATE persondata "
			"SET phone_number='" + old_phone_number + " " + tran.esc(phone_number) + "' "
			"WHERE id = '" + std::to_string(clientId) + "'";
		tran.exec(query);
		tran.commit();
		
		
	}

	void UpdateClient(int clientId, std::string& first_name, std::string& last_name, std::string& email, std::string& phone_number)
	{
		
		pqxx::transaction tran{ connection };
		std::string query = "UPDATE persondata "
			"SET first_name = '" + tran.esc(first_name) + "', last_name = '" + tran.esc(last_name) + "', email = '" + tran.esc(email) + "', phone_number = '" + tran.esc(phone_number) + "' "
			"WHERE id = '" + std::to_string(clientId) + "'";
		tran.exec(query);
		tran.commit();
		
	}
	void deletePhoneNumber(int clientId, std::string& phone_number)
	{
		
		pqxx::transaction tran{connection };
		std::string query_search_phone_number = "SELECT phone_number FROM persondata WHERE phone_number LIKE '%" + tran.esc(phone_number) + "%'";
		std::string old_phone_number = tran.query_value<std::string>(query_search_phone_number);
		std::string temp_old_number = old_phone_number;
		size_t index_in = old_phone_number.find(phone_number);
		old_phone_number.erase(index_in, phone_number.length());
		std::string query = "UPDATE persondata "
							"SET phone_number = '" + tran.esc(old_phone_number) + "' "
							"WHERE phone_number LiKE '%" + tran.esc(temp_old_number) + "%' ";
		tran.exec(query);
		tran.commit();
		
		
	}

	void deleteClient(int clientId)
	{
		
		pqxx::transaction tran{ connection };
		std::string query_delete_email = "DELETE FROM persondata "
										"WHERE id = " + std::to_string(clientId);
		tran.exec(query_delete_email);
		tran.commit();
	
	}


	~ManagerData()
	{
		connection.close();
	}
	
private:
	std::string connect_option =
		"host=localhost "
		"port=5432 "
		"dbname=test "
		"user=postgres "
		"password=U12h12b12";
	pqxx::connection connection{ connect_option };
	
	
};
int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	
	try
	{
		PersonData person;
		
		std::cout << "Let's make note in DataBase." << std::endl;
		person.first_name = out_first_name();
		person.last_name = out_last_name();
		person.email = out_email();
		person.phone_number = out_phoneNumber();
		ManagerData managerBD = ManagerData();
		person.id = managerBD.AddClient(person.first_name, person.last_name, person.email, person.phone_number);

	}
	catch (std::exception& e)
	{
		std::cout << "Exception in main: " << e.what() << std::endl;
	}



	
	return 0;
}

//int main()
//{
//	std::cout << "GF" << std::endl;
//}