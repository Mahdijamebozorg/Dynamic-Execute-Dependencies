#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>

using namespace std;

string findAddress(string address)
{
   size_t pos = 0;
   for (int i = address.size(); i >= 0; i--)
   {
      if (address[i] == '/')
      {
         pos = i;
         break;
      }
   }
   return address.substr(0, pos + 1);
}

//___________________________________________________________________________________

void copyDependencies(string output, string address)
{
   string temp = output.substr(output.find("/"));
   temp = "cp " + temp.substr(0, temp.find(" "));

   temp += " " + address + "lib/";

   system(temp.c_str());
}

//___________________________________________________________________________________

void readDependencies(string file, string address)
{
   char buffer[512];
   string result;

   FILE *pipe = popen(("ldd " + file).c_str(), "r");
   if (!pipe)
   {
      throw invalid_argument("False command!");
   }

   fgets(buffer, 512, pipe);

   while (!feof(pipe))
   {
      if (fgets(buffer, 512, pipe) != NULL)
         copyDependencies(buffer, address);
   }

   pclose(pipe);
}

//___________________________________________________________________________________

int main(int argc, char const *argv[])
{
   if (argc > 2)
   {
      cout << "Too mush arguments!" << endl;
      return 0;
   }
   else if (argc < 2)
   {
      cout << "Too few arguments!" << endl;
      return 0;
   }
   string file = argv[1];

   string address = findAddress(file);

   struct stat buff;
   if (stat((address + "/lib").c_str(), &buff) != 0)
      if (mkdir((address + "/lib").c_str(), S_IRUSR | S_IWUSR | S_IXUSR) != 0)
         throw runtime_error("Can't make lib file in address, check entered address again!");

   readDependencies(file, address);
   cout<< "All of dependencies coppied to "<<address << "lib" <<endl;
}
