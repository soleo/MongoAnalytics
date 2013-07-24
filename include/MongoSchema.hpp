#ifndef _MONGOSCHEMA_HPP_
#define _MONGOSCHEMA_HPP_
#include <string>
#include <vector>
#include "mongo/client/dbclient.h"
#include <tr1/unordered_map>

namespace MonAna{

	struct SchemaModel{
		int         count;
		std::string datatype;
	};
	typedef std::tr1::unordered_map<std::string, SchemaModel> hashmap;
	
	class MongoSchema
	{
		public:
			MongoSchema();
			~MongoSchema();
			std::string getSchema(mongo::DBClientConnection& conn,std::string dbname, std::string col);
			std::string toString() const;
			
		private:
			std::string                m_dbname;
			std::string                m_col;
			std::vector<std::string>   m_skipvector;
		
			mongo::DBClientConnection* m_conn;
			mongo::BSONObj             m_schema;
			hashmap m_map;
			void process();
			int skipField(std::string fieldname);
			int isBson(std::string fieldname, mongo::BSONObj bo, SchemaModel* sm);
			void extractMore(std::string fieldname, mongo::BSONOb bo);
	
	}; // class
	
	

} // namespace

#endif // _MONGOSCHEMA_HPP_
