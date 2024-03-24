#include "include/symtable.hpp"
#include "include/error.hpp"


tableRecord::symRecord(string __name, string __type, int __size, int __lineno, int __column, symbolTable* __symTab)
{
	name = __name;
	type = __type;
	size = __size;
	lineno = __lineno;
	column = __column;
	symTab = __symTab;
}


symbolTable::symTable(string __name, symbolTable* __parentSymtable)
{
	name = __name;
	parentSymtable = __parentSymtable;
}

tableRecord* symbolTable::lookup(string name, int line_no, int column, bool err)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if(indices.size())
	{
		assert(indices.size() == 1);
		record = entries[indices[0]];
		return record;
	}

	if(!parentSymtable) 
	{
		if (err)
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, RESET);
		return NULL;
	}

	record = parentSymtable->lookup(name, line_no, column, err);
	return record;

}

// error must be explicitly mentioned outside
tableRecord* symbolTable::lookup(string name, vector<tableRecord*> &params, int line_no, int column, bool err)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if (indices.size())
	{
		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;
			if (params.size() != table->offset) continue;
			
			int num;
			for (num = 0; num < params.size(); num++)
			{
				if (params[num]->type != ((table->entries)[num])->type)
					break;
			}
			if (num == params.size()) return entry;
		}
	}

	if(!parentSymtable) 
	{
		if (err)
			printErrorMsg(line_no, column, RED, " no previous definition found for \'", name, RESET);
		return NULL;
	}

	record = parentSymtable->lookup(name, params, line_no, column, err);
	return record;	
}

int symbolTable::insert(tableRecord* inputRecord, symbolTable* funcTable)
{
	string name = inputRecord->name;
	string type = inputRecord->type;
	int lineno = inputRecord->lineno;
	int column = inputRecord->column;
	int __size = inputRecord->size;

	vector<int> indices = name_to_indices[name];

	if (!funcTable)
	{
		assert(indices.size() == 0 || indices.size() == 1);
		if (indices.size())
		{
			tableRecord* entry = entries[0];
			printErrorMsg(lineno, column, RED, "redefintion of \'", name, "\'", RESET);
			printErrorMsg(entry->lineno, entry->column, BLUE, " previous definition of \'", name, "\' with type ", entry->type, RESET);
			return -1;
		}
		
	}
	
	else if (indices.size())
	{
		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;
			if(!table) 
			{
				printErrorMsg(lineno, column, RED, name, " redeclared as different kind of symbol", RESET);
				printErrorMsg(entry->lineno, entry->column, BLUE, name, " was declared previously as ", entry->type, RESET);

				return -1;
			}
		}

		for (auto idx: indices)
		{
			tableRecord* entry = entries[idx];
			symbolTable* table = entry->symTab;

			if (funcTable->offset != table->offset) continue;
			
			int num;
			for (num = 0; num < funcTable->offset; num++)
			{
				if (((funcTable->entries)[num])->type != ((table->entries)[num])->type)
					break;
			}

			if (num == funcTable->offset)
			{
				printErrorMsg(lineno, column, RED, "redefintion of \'", name, "\'", RESET);
				printErrorMsg(entry->lineno, entry->column, BLUE, " previous definition of \'", name, "\' with type ", entry->type, RESET);
				return -1;
			}

		}

	}

	tableRecord* record = new tableRecord(name, type, __size, lineno, column, funcTable);
	
	name_to_indices[name].push_back(currentIndex);
	record->index = currentIndex;
	entries[currentIndex] = record;

	// size of the table not updated when function or class entry
	if (!funcTable)
		size += __size;
	else
		childIndices.push_back(currentIndex);
	currentIndex++;

	return 0;
}

int symbolTable::UpdateRecord(tableRecord* newRecord)
{
	vector<int>indices = name_to_indices[newRecord->name];
	if(indices.size() == 0)
	{
		printErrorMsg(newRecord->lineno, newRecord->column, RED, "Use of undeclared variable \'", newRecord->name, "\'", RESET);
		return -1;
	}

	assert(indices.size() == 1);
	int index = indices[0];
	tableRecord* record = entries[index];

	if((newRecord->type).length()) record->type = newRecord->type;
	if(newRecord->size) 
	{
		size += newRecord->size - record->size;
		record->size = newRecord->size;
	}
	record->lineno = newRecord->lineno;
	record->column = newRecord->column;

	return 0;	
}

void tableRecord::generateCSV(ofstream &CSV)
{
	CSV << index << ", ";
	
	if (type.compare("str") == 0)
	{
		for (char c : name)
		{
			if (c == '"')
			{
				CSV << '\\' << '\"';
			}
			else if (c == '\\')
			{
				CSV << '\\' << '\\';
			}
			else
			{
				CSV << c;
			}
		}
	}
	else
		CSV << name;
	
	CSV << ", " << type << ", " << size << ", " << lineno << endl;
	return;
}


void symbolTable::generateCSV(ofstream &CSV)
{
	CSV << "# Table Name: " << name;
	if (parentSymtable)
		CSV << ", Parent Table: " << parentSymtable->name;
	
	CSV << endl;

	int index = 0;

	if (offset)
	{
		CSV << "\n # Incoming Parameters: \n";
		CSV << "index, name, type, size, line no. \n";
		for (; index < offset; index++)
			(entries[index])->generateCSV(CSV);
	}

	CSV << "\n # Local Variables: \n";
	CSV << "index, name, type, size, line no. \n";
	for (; index < currentIndex; index++)
		(entries[index])->generateCSV(CSV);


	for(auto index : childIndices) 
	{
		CSV << "\n\n\n\n\n";
		assert(entries[index]->symTab);
		((entries[index])->symTab)->generateCSV(CSV);
	}

	return;
}