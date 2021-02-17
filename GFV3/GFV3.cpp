#include <iostream>
#include "include.h"
#include "GFW.h"


//project includes
#include "projects/test/test.h"


void run(int argc, char* argv[]) {
	map<string, GFW::Inst*> projects;


	//add proj here
	projects["test"] = new test_proj::test_proj();
	//add proj here


	string projtr;

	cout << "Input the project name of the project you want to run --> ";
	cin >> projtr;
	cout << endl;

	projects[projtr]->ep();

};



int main(int argc, char* argv[])
{
    
	run(argc, argv);

    return 0;
}
