#include <iostream>
#include <fstream>

std::string exec(const char* cmd) 
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try 
	{
        while (fgets(buffer, sizeof buffer, pipe) != NULL) 
		{
            result += buffer;
        }
    } 
	catch (...) 
	{
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::string hashCalc(std::string semi_final, std::string hash4crack, int z, int sizeFirst, int a, int sizeSecond)
{
	std::string cmd("md5 <<< ");
	//std::string cmd("shasum <<< ");
	cmd += semi_final;
	std::string res = exec(cmd.c_str());
	res.erase(std::remove(res.begin(), res.end(), '\n'), res.end());
	res.erase(std::remove(res.begin(), res.end(), '-'), res.end());
	res.erase(std::remove(res.begin(), res.end(), ' '), res.end());
			
	if (res == hash4crack)
	{
		std::cout << "found! = " << semi_final << std::endl;
		exit(0);
	}
	else
	{
		std::cout << "1wordLoop = " << z << "/" << sizeFirst - 1 << 
		" 2wordLoop = " << a << "/" << sizeSecond - 1 << " "  
		<< std::endl; 
		std::cout << semi_final << " " << res << std::endl;
	}
	return res;
}

int main()
{
	std::string semi_final;
	std::ofstream out("passdic.txt");
	std::string hash4crack = "55d91e34650fd1a804cd94f5f39b34fb";
	//std::string hash4crack = "cbd4985cff1c0846df04f5a5162bb6f5"; //test
	std::string firstWord[] = {"Monaco", "MonacoTelecom", "Monacotelecom","Iliad", "Minitel","Free", "Online", "Freemobile", "FreeMobile", "42", "KimaVentures", "Kimaventures", "StationF", "Stationf"};
	std::string secondWord[] = {"Josset", "Julia", "John", "Jules", "Delphine", "Bernard", "Elisa", "Michel", "Camille"};

	int sizeSemiF = sizeof(semi_final) / sizeof(semi_final[0]);
	int sizeFirst = sizeof(firstWord) / sizeof(firstWord[0]);
	int sizeSecond = sizeof(secondWord) / sizeof(secondWord[0]);

	for (int z = 0; z < sizeFirst; z++)
	{
		for (int a = 0; a < sizeSecond; a++)
		{
			int x = 0;
			int y = 0;
			for (long int i = 0; i < 2194; i++)
			{
				semi_final = std::to_string(y);
				semi_final += firstWord[z];
				semi_final += std::to_string(x);
				semi_final += secondWord[a];
				if (y == 42)
				{
					y = 0;
					x++;
				}	
				else
					y++;
				if (x == 51)
					x = 0;
				
				//hashcat not working in campus macs :(
				std::string res = hashCalc(semi_final, hash4crack, z, sizeFirst, a, sizeSecond);
		
				out << semi_final << " ";
				out	<< res << "\n"; //comment this to remove hashes
			}
		}
	}
	std::string cmd2("wc -l passdic.txt");
	std::string res2 = exec(cmd2.c_str());
	std::cout << "Permutations generated = " << std::endl;
	return 0;
}
