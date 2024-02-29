
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
	cerr << RED ;
    while (*format)
	{
        if (format[0] == '%' && format[1] == '@')
        {
            YY_LOCATION_PRINT (stderr, yylloc);
            format += 2;
        }

	  	if (format[0] == '%' && format[1] == 'u')
		{

            string token = (yysymbol_name(yypcontext_token(ctx)));
            cerr << BLUE <<name_to_symbol[token] << RESET;
			format += 2;
		}

	  	else if (format[0] == '%' && isdigit ((unsigned char) format[1]) && format[2] == 'e' && (format[1] - '0') < argsize)
		{
			int i = format[1] - '0';
            string token = yysymbol_name(arg[i]);
			cerr <<BLUE << name_to_symbol[token] <<RESET;
			format += 3;
		}

	  	else
		{
            cerr << RED;
			fputc (*format, stderr);
			++format;
		}

	}
	cerr << endl << RESET;

	return 1;

}
void yyerror (string s) {

    int column = yylloc.first_column;
    int line = yylloc.first_line;

	cerr << RED << "in line " << line << ", column " << column << RESET << endl;
	if (s.length())
	{
		cerr << RED << s << RESET << endl;
		return;
	}
    cerr << setw(10) << right << line << left << setw(6) << CYAN <<" | " <<text << RESET << endl;
	cerr << setw(17) << right << MAGENTA << " | ";
    for (int i = 0; i < column - 1; i++)
        cerr << "~";

    cerr << "^" << endl << RESET;

} 