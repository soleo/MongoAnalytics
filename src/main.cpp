
#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp" 
#include "MongoSchema.hpp"

using namespace std;
namespace 
{ 
  const size_t ERROR_IN_COMMAND_LINE = 1; 
  const size_t SUCCESS = 0; 
  const size_t ERROR_UNHANDLED_EXCEPTION = 2; 
  const size_t HELPMSG = 3;
 
} // namespace 

int parse_options(int argc, char** argv, 
				  std::string& action, 
				  std::string& db, 
				  std::string& col, 
				  std::string& expformat) {
	try 
	  { 
	    /** Define and parse the program options 
	     */ 
	    namespace po = boost::program_options; 
	    po::options_description desc("Options"); 
	    desc.add_options() 
	      ("help,h", "Print help messages")
		  ("action,a", po::value<std::string>(&action)->required(), "schema (Required)")
		  ("verbose,v", "Display Verbose Information") 
	      ("dbname,d", po::value<std::string>(&db)->required(), "Database Name (Required)") 
	      ("colname,c", po::value<std::string>(&col)->required(), "Collection Name (Required)")
		  ("export,e", po::value<std::string>(&expformat), "Export Format. Eg:csv,JSON,XML"); 
 
	    po::variables_map vm; 
	    try 
	    { 
	      po::store(po::parse_command_line(argc, argv, desc),  
	                vm); // can throw 
 
	      /** --help option 
	       */ 
	      if ( vm.count("help")  ) 
	      { 
	        std::cout << "MonAna: Command Line Tool for Mongo" << std::endl 
	                  << desc << std::endl; 
	        return HELPMSG; 
	      } 
		  
 
	      po::notify(vm); // throws on error, so do after help in case 
	                      // there are any problems 
	    } 
	    catch(po::error& e) 
	    { 
	      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
	      std::cerr << desc << std::endl; 
	      return ERROR_IN_COMMAND_LINE; 
	    } 
 
	    // application code here // 
 
	  } 
	  catch(std::exception& e) 
	  { 
	    std::cerr << "Unhandled Exception reached the top of main: " 
	              << e.what() << ", application will now exit" << std::endl; 
	    return ERROR_UNHANDLED_EXCEPTION; 
 
	  } 
 
	  return SUCCESS; 
}


int main(int argc, char** argv) {
  // parse options for processing
  std::string db  = "";
  std::string col = "";
  std::string expformat = "";
  std::string action = "";
  size_t status = parse_options(argc, argv, action, db, col, expformat);
  
  
  if(status != SUCCESS){
	  exit(0);
  }
  
  //std::cout << "Connecting to " << db << " use action " << action << endl;
  
  mongo::DBClientConnection c;
  try {
    c.connect("localhost");
    //std::cout << "connected ok" << std::endl;
  } catch( const mongo::DBException &e ) {
    std::cout << "caught " << e.what() << std::endl;
  }

  //query for something
  //std::cout << "count:" << c.count("dev.camps_events") << endl;
  if(action.compare("schema") == 0){
	 MonAna::MongoSchema* ms = new MonAna::MongoSchema();
  	 std::string result = ms->getSchema(c, db, col);
	 std::cout << result << endl;
  }
  
/*
  auto_ptr<mongo::DBClientCursor> cursor = c.query("dev.camps_events",mongo::BSONObj());
  while(cursor->more()){
        std::cout << cursor->next().toString() << endl;
  }
  */
  return EXIT_SUCCESS;
}
