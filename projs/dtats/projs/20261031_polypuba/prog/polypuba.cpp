// polypuba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iomanip>
#include <iostream>

#include "configfile.hpp"
#include "lg_cpp_cm_fio.hpp"


static void echo_command_line_parameters(std::ostream& in_output_stream, int argc, char *argv[])
{
   int i;

   in_output_stream << "argc     : " << argc << std::endl;

   for (i = 0; i < argc; i++)
   {
      in_output_stream << "argv[" << std::setw(2) << i << "] : \"" << argv[i] << "\"" << std::endl;
   }

   LgCppCm_FioHlineStream(in_output_stream);
}


static void emit_help_info(std::ostream& in_output_stream, bool in_emit_trailing_hline = false)
{
   size_t i;

   static const char * const help_info[] =
   {
      "polypuba, (C) 2026, David T. Ashley",
      "This program is released under The Unlicense (https://unlicense.org)",
      "-",
      "Usage: polypuba <options> <command> <configfile_path>",
      "-",
      "See: https://github.com/dtashley/DtaPublic"
   };

   for (i = 0; i < (sizeof(help_info)/sizeof(help_info[0])); i++)
   {
      if (strcmp(help_info[i], "-") != 0)
      {
         in_output_stream << help_info[i] << std::endl;
      }
      else
      {
         LgCppCm_FioHlineStream(in_output_stream);
      }
   }

   if (in_emit_trailing_hline)
      LgCppCm_FioHlineStream(in_output_stream);
}


void command_translate(std::ostream& in_output_stream, int argc, char* argv[])
{

}


int main(int argc, char *argv[])
{
   int rc = 0;

   //Execution begins heading.
   LgCppCm_FioHlineStream(std::cout);
   std::cout << "Execution begins.\n";
   LgCppCm_FioHlineStream(std::cout);

   //Echo the command-line parameters.
   echo_command_line_parameters(std::cout, argc, argv);

   if (argc < 3)
   {
      //If there are not at least three parameters, print an error message plus
      //information about the program.
      std::cout << "Error:  Too few command-line parameters.  Exiting program.\n";
      LgCppCm_FioHlineStream(std::cout);
      emit_help_info(std::cout, true);

      rc = 1;  //Error return code.
   }
   else if (strcmp(argv[argc-2], "translate") == 0)
   {
      ConfigFile cfgfile;

      //translate command.
      command_translate(std::cout, argc, argv);
   }
   else if (strcmp(argv[argc-2], "epubtocindexadd") == 0)
   {

   }
   else
   {
      //Unrecognized command.
      std::cout << "Error:  Command must be \"translate\" or \"epubtocindexadd\".  Exiting program.\n";
      LgCppCm_FioHlineStream(std::cout);
      emit_help_info(std::cout, true);

      rc = 1;  //Error return code.
   }

   //Execution ends heading.
   std::cout << "Execution ends.\n";
   LgCppCm_FioHlineStream(std::cout);

   return rc;
}
