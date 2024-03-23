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

tableRecord* symbolTable::lookup(string name)
{
	vector<int>indices = name_to_indices[name];
	tableRecord* record = NULL;
	if(indices.size())
	{
		assert(indices.size() == 1);
		record = entries[indices[0]];
		return record;
	}

	if(!parentSymtable) return NULL;

	record = parentSymtable->lookup(name);
	return record;

}

tableRecord* symbolTable::lookup(string name, vector<tableRecord*> &params)
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

	if(!parentSymtable) return NULL;

	record = parentSymtable->lookup(name, params);
	return record;	
}

int symbolTable::insert(string name, string type, int size, int lineno, int column)
{
	vector<int>indices = name_to_indices[name];
	
	assert(indices.size() == 0 || indices.size() == 1);
	if (indices.size()) return (entries[indices[0]])->lineno;

	tableRecord* record = new tableRecord(name, type, size, lineno, column, NULL);
	
	name_to_indices[name].push_back(currentIndex);
	entries[currentIndex] = record;
	currentIndex++;

	return 0;
}

int symbolTable::insert(string name, string type, int size, int lineno, int column, symbolTable* funcTable)
{
	vector<int>indices = name_to_indices[name];
	
	if (indices.size())
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
				printErrorMsg(lineno, column, RED, "redefintion of `", name, "`", RESET);
				printErrorMsg(entry->lineno, entry->column, BLUE, " previous definition of \'", name, "\' with type ", entry->type, RESET);
				return -1;
			}

		}

	}

	tableRecord* record = new tableRecord(name, type, size, lineno, column, funcTable);
	
	name_to_indices[name].push_back(currentIndex);
	childIndices.push_back(currentIndex);
	entries[currentIndex] = record;
	currentIndex++;

	return 0;
}


void tableRecord::generateCSV(ofstream &CSV)
{
	CSV << index << ", " << name << ", " << type << ", " << size << ", " << lineno << endl;
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
		((entries[index])->symTab)->generateCSV(CSV);
	}

	return;
}