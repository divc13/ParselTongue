#define RED "\033[1;31m"
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

extern string inputFile;

static const char * error_format_string (int argc)
{
  	switch (argc)
	{
		default: // Avoid compiler warnings.
		case 0: return ("%@: syntax error");
		case 1: return ("%@: syntax error: unexpected %u");
		case 2: return ("%@: syntax error: expected %0e before %u");
		case 3: return ("%@: syntax error: expected %0e or %1e before %u");
		case 4: return ("%@: syntax error: expected %0e or %1e or %2e before %u");
		case 5: return ("%@: syntax error: expected %0e or %1e or %2e or %3e before %u");
		case 6: return ("%@: syntax error: expected %0e or %1e or %2e or %3e or %4e before %u");
		case 7: return ("%@: syntax error: expected %0e or %1e or %2e or %3e or %4e or %5e before %u");
		case 8: return ("%@: syntax error: expected %0e or %1e or %2e or %3e or %4e or %5e etc., before %u");
	}
}

int yyreport_syntax_error (const yypcontext_t *ctx)
{
	
	yyerror("");
	enum { ARGS_MAX = 6 };
	yysymbol_kind_t arg[ARGS_MAX];
	int argsize = yypcontext_expected_tokens (ctx, arg, ARGS_MAX);

	if (argsize < 0) return argsize;
	const int too_many_expected_tokens = argsize == 0 && arg[0] != YYSYMBOL_YYEMPTY;
	if (too_many_expected_tokens) argsize = ARGS_MAX;
	const char *format = error_format_string (1 + argsize + too_many_expected_tokens);

	const YYLTYPE *loc = yypcontext_location (ctx);
	cout << RED ;
	while (*format)
	{
		if (format[0] == '%' && format[1] == '@')
		{
			YY_LOCATION_PRINT (stdout, yylloc);
			format += 2;
		}

	  	if (format[0] == '%' && format[1] == 'u')
		{

			string token = (yysymbol_name(yypcontext_token(ctx)));
			cout << BLUE << TokenNameToString[token] << RESET;
			format += 2;
		}

	  	else if (format[0] == '%' && isdigit ((unsigned char) format[1]) && format[2] == 'e' && (format[1] - '0') < argsize)
		{
			int i = format[1] - '0';
			string token = yysymbol_name(arg[i]);
			cout << BLUE << TokenNameToString[token] << RESET;
			format += 3;
		}

	  	else
		{
			cout << RED;
			fputc (*format, stdout);
			++format;
		}

	}
	cout << endl << RESET;

	exit(0);

}

void yyerror (string s) {

	int column = yylloc.first_column;
	int line = yylloc.first_line;
	int last_col = yylloc.last_column;
	int last_line = yylloc.last_line;
	
	cout << RED << "In " << inputFile << ", found error in line " << line << ", column " << column << RESET << endl;
	if (s.length())
	{
		cout << RED << "In " << inputFile << ", found error in line " << line << ", column " << column << RESET << endl;
		return;
	}
	cout << setw(10) << right << line << left << setw(6) << CYAN <<" | " <<text << RESET << endl;
	cout << setw(17) << right << MAGENTA << " | ";
	
	int i = 0;
	if(last_col != column)
	{
		for (i = 0; i < column - 1; i++)
		cout << " ";
	}

	while(i < last_col - 1) 
	{
		cout << "~";
		i++;
	}

	cout << "^" << endl << RESET;

} 