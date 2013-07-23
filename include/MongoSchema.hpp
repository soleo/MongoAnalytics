#ifndef _MONGOSCHEMA_HPP_
#define _MONGOSCHEMA_HPP_
#include <string>
#include "mongo/client/dbclient.h"
namespace MonAna{

	class MongoSchema
	{
		public:
			std::string getSchema(mongo::DBClientConnection& conn,std::string dbname, std::string col);
	
		private:
			std::string m_dbname;
			std::string m_col;
			void process();
	
	}; // class

} // namespace

#endif // _MONGOSCHEMA_HPP_
