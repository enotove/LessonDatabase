#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <Windows.h>
#include <regex>
#include <tuple>

class PersonData
{
public:
	PersonData(pqxx::connection& con)
	{
		try
		{
			pqxx::transaction tran{ con };
			std::string query = "CREATE TABLE IF NOT EXISTS persondata (id SERIAL PRIMARY KEY, "
				"first_name VARCHAR(60) NOT NULL,"
				"last_name VARCHAR(60) NOT NULL, "
				"email VARCHAR(100) NOT NULL UNIQUE, "
				"phone_number TEXT UNIQUE)";
			tran.exec(query);
			tran.commit();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception in constructor of PersonData: " << e.what() << std::endl;
		}
	}
	void AddClient(pqxx::connection& con)
	{
		std::string first_name = out_first_name();
		std::string last_name = out_last_name();
		std::string email = out_email();
		std::string phone_number = out_phoneNumber();
		try
		{
			pqxx::transaction tran{ con };
			std::string query = "INSERT INTO persondata(first_name, last_name, email, phone_number)"
				"VALUES('" + tran.esc(first_name) + "', '" + tran.esc(last_name) + "', '" + tran.esc(email) + "', '" + tran.esc(phone_number) + "')";
			tran.exec(query);
			tran.commit();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception in function of AddClient: " << e.what() << std::endl;
		}
		
	}

	void AddPhoneNumber(pqxx::connection& con)
	{
		
		std::string email = out_email();
		std::string phone_number = out_phoneNumber();
		try
		{
			pqxx::transaction tran{ con };
			std::string old_phone_number = tran.query_value<std::string>("SELECT phone_number FROM persondata "
				"WHERE email='" + tran.esc(email) + "'");
			std::string query = "UPDATE persondata "
				"SET phone_number='" + old_phone_number + " " + tran.esc(phone_number) + "' "
				"WHERE email = '" + tran.esc(email) + "'";
			tran.exec(query);
			tran.commit();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception in function of AddPhoneNumber: " << e.what() << std::endl;
		}
	}

	void UpdateClient(pqxx::connection& con)
	{
		std::cout << "Which client do you want update?" << std::endl;
		std::string old_email = out_email();
		std::cout << "Enter your new personal data:" << std::endl;
		std::string first_name = out_first_name();
		std::string last_name = out_last_name();
		std::string email = out_email();
		std::string phone_number = out_phoneNumber();
		try
		{
			pqxx::transaction tran{ con };
			std::string query = "UPDATE persondata "
				"SET first_name = '" + tran.esc(first_name) + "', last_name = '" + tran.esc(last_name) + "', email = '" + tran.esc(email) + "', phone_number = '" + tran.esc(phone_number) + "' "
				"WHERE email = '" + tran.esc(old_email) + "'";
			tran.exec(query);
			tran.commit();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception in function of UpdateClient: " << e.what() << std::endl;
		}
	}
	void deletePhoneNumber(pqxx::connection& con)
	{
		std::cout << "Which phone number do you want delete?" << std::endl;
		std::string phone_number = out_phoneNumber();
		try
		{
			pqxx::transaction tran{ con };
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
		catch (const std::exception& e)
		{
			std::cout << "Exception in function of deletePhoneNumber: " << e.what() << std::endl;
		}
		
	}

	void deleteClient(pqxx::connection& con)
	{
		std::cout << "Which client do you want delete, please enter email?" << std::endl;
		std::string email = out_email();
		try
		{
			pqxx::transaction tran{ con };
			std::string query_delete_email = "DELETE FROM persondata "
											"WHERE email = '" + tran.esc(email) + "'";
			tran.exec(query_delete_email);
			tran.commit();
		}
		catch (const std::exception& ex)
		{
			std::cout << "Exception in function of deleteClient: " << ex.what() << std::endl;
		}
		

		
	}

	void searchClient(pqxx::connection& con)
	{
		
		bool flag = true;
		int choose;
		std::string query_search;
		do
		{
			
			std::cout << "Select the data you will use to search for a client, enter 1 if by First name, Last Name and Email, 2 by phone: ";
			std::cin >> choose;
			if ((choose == 1) || (choose == 2))
			{
				flag = false;
			}
		} while (flag);
		try {
			pqxx::transaction tran{ con };
			switch(choose)
			{
			case 2:
			{
				std::string phone_number = out_phoneNumber();
				query_search = "SELECT first_name, last_name, email, phone_number FROM persondata "
					"WHERE phone_number LIKE '%" + tran.esc(phone_number) + "%'";

				
			}
			break;
			case 1:
				std::string first_name = out_first_name();
				std::string last_name = out_last_name();
				std::string email = out_email();
				query_search = "SELECT first_name, last_name, email, phone_number FROM persondata "
					"WHERE (first_name = '" + tran.esc(first_name) + "') AND (last_name = '" + tran.esc(last_name) + "') AND (email = '" + tran.esc(email) + "')";
				break;
			}

			auto result = tran.query1<std::string, std::string, std::string, std::string>(query_search);
			std::cout << "Search result: \nFirst name: " + std::get<0>(result) + "\nLast name: " + std::get<1>(result) + "\nEmail: " + std::get<2>(result) + "\Phone number: " + std::get<3>(result) << std::endl;
		}
		catch (const std::exception& ex)
		{
			std::cout << "Exception in function of searchClient: " << ex.what() << std::endl;
		}
		
	}
private:
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
	
	
};
int main()
{
	//setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::string connect_option = "host=localhost "
								"port=5432 "
								"dbname=test "
								"user=postgres "
								"password=U12h12b12 ";
	try
	{
		pqxx::connection con(connect_option);
		PersonData person1(con);
		
		std::cout << "searchClient" << std::endl;
		person1.searchClient(con);

	}
	catch (std::exception& e)
	{
		std::cout << "Exception in main: " << e.what() << std::endl;
	}



	
	return 0;
}