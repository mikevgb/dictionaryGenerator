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

int main()
{
	std::string semi_final[2193];
	std::ofstream out("passdic.txt");
	std::string hash4crack = "55d91e34650fd1a804cd94f5f39b34fb";
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
			int res_count = sizeSemiF;
			for (long int i = 0; i < sizeSemiF; i++)
			{
				semi_final[i] += std::to_string(y);
				semi_final[i] += firstWord[z];
				semi_final[i] += std::to_string(x);
				semi_final[i] += secondWord[a];
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
				std::string cmd("md5 <<< ");
				cmd += semi_final[i];
				std::string res = exec(cmd.c_str());
				res.erase(std::remove(res.begin(), res.end(), '\n'), res.end());
				
				if (res == hash4crack)
				{
					std::cout << "found! = " << semi_final[i] << std::endl;
					exit(0);
				}
				else
				{
					std::cout << "1wordLoop = " << z << "/" << sizeFirst - 1 << 
						" 2wordLoop = " << a << "/" << sizeSecond - 1 << " "  
						<<  res_count-- << std::endl;
					std::cout << semi_final[i] << " " << res << std::endl;
				}
					
				out << semi_final[i] << " " << res << "\n";
				semi_final[i] = ""; //reset string
			}
		}
	}
	std::string cmd2("wc -l passdict.txt");
	std::string res2 = exec(cmd2.c_str());
	std::cout << "Permutations generated = " << std::endl;
	return 0;
}
