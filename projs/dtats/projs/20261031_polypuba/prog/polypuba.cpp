// polypuba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iomanip>
#include <iostream>

#include "configfile.hpp"
#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_buf_uint8.hpp"
#include "lg_cpp_dd_parse_lex_cust_a.hpp"
#include "lg_cpp_dd_rpt_errlst_cd_a.hpp"


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


void command_translate(std::ostream& in_output_stream, const LgBufUint8& file_buffer, int argc, char* argv[])
{
   LgCppDdParseLexCustA cust_lexer;
      //Custom lexographic analyzer used for lexographically analyzing the configuration file.
   ConfigFile cf;
      //Configuration file.
   bool success = true;
      //true if operations so far are successful.
   bool lex_success = false;
      //Success or failure from the lexographic analyzer.
   bool parse_success = false;
      //Success or failure from parsing.
   std::string err_text;
      //Is this used?
   LgCppDdErrlistCdA lex_err_list;
      //Error list from lexographic analysis.
   LgCppDdErrlistCdA parse_err_list;
      //Error list from parsing.

   in_output_stream << "TRANSLATE command.\n";

   LgCppCm_FioHlineStream(in_output_stream);

   echo_command_line_parameters(in_output_stream, argc, argv);

   cust_lexer.Parse(file_buffer, &lex_success, lex_err_list);

   if (lex_success)
   {
      in_output_stream << "Lexical analysis of configuration file was successful." << std::endl;
      LgCppCm_FioHlineStream(in_output_stream);
      cust_lexer.EmitTokenListToStreamText(std::cout, "Configuration file lexical analysis tokens obtained:");
   }
   else
   {
      in_output_stream << "Lexical analysis of configuration file has failed." << std::endl;
      LgCppCm_FioHlineStream(in_output_stream);
      cust_lexer.EmitTokenListToStreamText(in_output_stream, "Configuration file lexical analysis tokens obtained before failure:");
      lex_err_list.EmitListToStreamText(in_output_stream, "Configuration file lexical analysis warning/error list:");
   }

   if (lex_success)
   {
      parse_success = cf.Parse(file_buffer, cust_lexer, parse_err_list);
   }
}


void command_epubtocindexadd(std::ostream& in_output_stream, const LgBufUint8& file_buffer, int argc, char* argv[])
{
   in_output_stream << "EPUBTOCINDEXADD command.\n";
   LgCppCm_FioHlineStream(in_output_stream);

   echo_command_line_parameters(in_output_stream, argc, argv);
}


int main(int argc, char *argv[])
{
   int rc = 0;

   //Execution begins heading.
   LgCppCm_FioHlineStream(std::cout);
   std::cout << "Execution begins.\n";
   LgCppCm_FioHlineStream(std::cout);

#if 0
   {
      LgBufUint8 test_buf;
      LgCppDdErrlistCdA test_list;

      std::cout << "Number of elements: " << test_list.Nelem() << std::endl;

      test_list.AppendFull(test_buf, "", "", 0, 0, 0, LgCppDdErrlistCdASeverity::Informational, "");

      std::cout << "Number of elements: " << test_list.Nelem() << std::endl;

      return 0;
   }
#endif

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
      LgBufUint8 file_buffer;
      bool fread_success;
      uint32_t fread_err_mask;
      std::string fread_err_desc;

      //Attempt to read from the config file into the file buffer.  If this fails, the program will exit with an error code.
      file_buffer.Fread(argv[argc - 1], fread_success, fread_err_mask, fread_err_desc);

      if (!fread_success)
      {
         std::cout << "Error:  Could not open/read configuration file.  Exiting program.\n";
         LgCppCm_FioHlineStream(std::cout);

         rc = 1;  //Error return code.
      }
      else
      {
         std::cout << "Configuration file read: " << file_buffer.Nelem() << " bytes.\n";
         LgCppCm_FioHlineStream(std::cout);

         //translate command.
         command_translate(std::cout, file_buffer, argc - 3, argv + 2);
      }
   }
   else if (strcmp(argv[argc-2], "epubtocindexadd") == 0)
   {
      ConfigFile cfgfile;
      LgBufUint8 file_buffer;
      bool fread_success;
      uint32_t fread_err_mask;
      std::string fread_err_desc;

      //Attempt to read from the config file into the file buffer.  If this fails, the program will exit with an error code.
      file_buffer.Fread(argv[argc - 1], fread_success, fread_err_mask, fread_err_desc);

      if (!fread_success)
      {
         std::cout << "Error:  Could not open/read configuration file.  Exiting program.\n";
         LgCppCm_FioHlineStream(std::cout);

         rc = 1;  //Error return code.
      }
      else
      {
         std::cout << "Configuration file read: " << file_buffer.Nelem() << " bytes.\n";
         LgCppCm_FioHlineStream(std::cout);

         //translate command.
         command_epubtocindexadd(std::cout, file_buffer, argc - 3, argv + 2);
      }
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

   std::cout << "Exiting with return code " << rc << ".\n";
   LgCppCm_FioHlineStream(std::cout);

   return rc;
}
