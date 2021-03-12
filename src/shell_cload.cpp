#include "shell_cload.h"

namespace priscas
{

	void Shell_Cload::Run()
	{
		// Set shell enviornment
		Shell::shEnv.characterize_Env(Shell::args);

		UPString app_brand = branding::APPLICATION_NAME + " " +
				branding::APPLICATION_VERSION + " " +
				branding::APPLICATION_DBG;

		UPString header = app_brand + priscas_io::newLine + UPString("[loader]") +
			priscas_io::newLine;


		Shell::WriteToOutput(header);

		// Check for input script.
		if(Shell::shEnv.get_Option_AsmInput())
		{
			if(!Shell::shEnv.get_Option_AsmInputSpecified())
			{
				fprintf(stdout, "Error: Option -i requires an argument.\n");
				return;
			}
		}

		// Invoke input script is possible.
	
		while(Shell::modeget() == Env::INTERACTIVE)
		{
			Shell::WriteToOutput(">> ");

			const UPString& val = this->ReadFromInput();

			if(val.size() == 0)
			{
				continue;
			}

			else if(val[0] == '.')
			{
				try
				{
					UPString_Vec chopped = chop_string(val);
					Shell::execute_runtime_directive(chopped);
				}
			
				catch(priscas::mt_exception & e)
				{
					WriteToError(e.get_err());
				}
			}
			else
			{
				UPString_Vec args;
				args.push_back(".help");
				Shell::execute_runtime_directive(args);
			}
		}
	}

	// Set up list of runtime directives
	Shell_Cload::Shell_Cload() :
		Shell()
	{
		// Set up jump table for runtime directives
		Shell::directives.insert(directive_pair(".help", priscas::help_loader));
		Shell::directives.insert(directive_pair(".rst", priscas::reset));
	}
}
