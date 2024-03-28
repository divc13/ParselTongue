#include "include/symtable.hpp"
#include "include/error.hpp"

map<int, string> ErrorMap;
map<int, string> NoteMap;

void printErrorLine(int line, int column)
{
	cout << setw(10) << right << line << left << setw(6) << CYAN << " | " << lines[line - 1] << RESET;
	cout << setw(17) << right << MAGENTA << " | ";
	
	int i = 0;
	while(i < column - 1) 
	{
		cout << "~";
		i++;
	}

	cout << "^" << endl << RESET;
	cout << endl;
}

void raise_error(int err, TreeNode* root)
{
	if (err < ERR::CLASS_ATTRIBUTE_DECL_SELF)
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << CYAN << root->name << RESET << RED << ":" << RESET;

	else if (err < ERR::EXPECTED_INT)
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << ":" << RESET;

	else
		cout << BLUE << UNDERLINE << inputFile << ":" << root->lineno << ":" << root->column << ":" << RESET << " " << RED << "Error: " << ErrorMap[err] << CYAN << root->type << RESET << RED << ":" << RESET;
	
	cout << endl;

	if ( root->lineno <= 0 )
		return;

	printErrorLine(root->lineno, root->column);
}

void print_note(int note, tableRecord* record)
{

	if ( record->lineno <= 0 )
		return;

	cout << BLUE << UNDERLINE << inputFile << ":" << record->lineno << ":" << record->column << ":" << RESET << " " << BLUE << "Note: " << NoteMap[note] << CYAN << record->name  << RESET << BLUE << " :" << RESET;
	
	cout << endl;
	
	if (note < NOTE::SHOW_TYPE)
		printErrorLine(record->lineno, record->column);
}