#include "MongoSchema.hpp"
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>

using namespace bson;
using namespace mongo;
namespace MonAna{
	
	MongoSchema::MongoSchema(){
		m_dbname.clear();
		m_col.clear(); 
		
		// init a list of fields I want to skip checking
		m_skipvector.push_back("_id");
		m_skipvector.push_back("id");
		m_skipvector.push_back("_uid");
		m_skipvector.push_back("_created");
		m_skipvector.push_back("_createduid");
		m_skipvector.push_back("_lastupdated");
		m_skipvector.push_back("_lastupdateuid");
		m_skipvector.push_back("_updated");
		m_skipvector.push_back("_updateuid");
	}
	
	MongoSchema::~MongoSchema(){
		m_dbname.clear();
		m_col.clear(); 
		m_skipvector.clear();
		
	}
	
	std::string MongoSchema::getSchema(mongo::DBClientConnection& conn,std::string dbname, std::string col)
	{
		// process the data and export schema
		m_conn   = &conn; 
		m_dbname = dbname;
		m_col    = col;
		process();
		return m_schema.jsonString();
	}
	
	void MongoSchema::process(){
		//std::cout << "Processing " << m_dbname << "." << m_col << std::endl;
		std::string querystr;
		querystr.clear();
		querystr.append(m_dbname);
		querystr.append(".");
		querystr.append(m_col);
		int recordscount = m_conn->count(querystr);
		
		//std::cout << "count:" <<  recordscount << std::endl;
		
		std::auto_ptr<mongo::DBClientCursor> cursor = m_conn->query(querystr, mongo::Query());
		std::set<std::string> fields;
		while(cursor->more()){
			mongo::BSONObj bo = cursor->next();
			fields.clear();
			int count = bo.getFieldNames(fields);
			//std::cout << "fields count:" << count << std::endl;
			std::set<std::string>::iterator it;
			for(it = fields.begin(); it != fields.end(); it++)
			{
				
				// skip field if it is in skipvector
				if(skipField(*it)){
					continue;
				}
				
				hashmap::const_iterator keyexsit = m_map.find(*it);
				SchemaModel* sm = new SchemaModel();
				if(isBson(*it, bo, sm)){
					// go into the object and check until reach 5th layer
					extractMore(*it, bo);
				}else{
				
					if(keyexsit != m_map.end()){
						sm = &m_map[*it];
						sm->count ++;
					}else{
						sm->count = 1;
						m_map[*it] = *sm; 
					}
				}
			    
			}
			
		}
		BSONObjBuilder bOb;
		BSONArrayBuilder bArr;
		std::tr1::hash<std::string> hashfunc = m_map.hash_function();
		for( hashmap::const_iterator i = m_map.begin(), e = m_map.end() ; i != e ; ++i ) {
			    SchemaModel sm = i->second;
				float percentage = (float)sm.count / (float)recordscount * 100.0;
				std::cout.precision(4);
				BSONObj bo = BSON( "field" << i->first << "percent" << percentage << "datatype" << sm.datatype );
				bArr.append(bo);
		        //std::cout << i->first << " -> "<< "Percent: "<< percentage << " (hash = " << hashfunc( i->first ) << ")" << "\r\n";
		}
		bOb.append(m_col, bArr.arr());
		m_schema = bOb.obj();
	}
	
	std::string MongoSchema::toString() const {
	        return "";
	}
	
	int MongoSchema::skipField(std::string fieldname){
		for(std::vector<std::string>::iterator it = m_skipvector.begin(); it != m_skipvector.end(); ++it) {
		    if(fieldname.compare(*it) == 0){
		    	return 1;
		    }
		}
		return 0;
	}
	
	int MongoSchema::isBson(std::string fieldname, mongo::BSONObj bo, SchemaModel* sm){
		mongo::BSONElement belem = bo.getField(fieldname);
		if(belem.isABSONObj()) {
			sm->datatype = "BSON";
			return 1;
		}else if(belem.isSimpleType()){
			// Simple Type: Number, String, Date, Bool or OID
			if(belem.isNumber() || 
			   belem.type() == mongo::NumberInt || 
			   belem.type() == mongo::NumberLong ||
			   belem.type() == mongo::NumberDouble ){
				sm->datatype = "Number";
			}else if(belem.type() == mongo::String){
				sm->datatype = "String";
			}else{
				sm->datatype = "Other";
			}
			
		}else{
			sm->datatype = "Unknown";
		}		
		return 0;
	}
	
	void MongoSchema::extractMore(std::string fieldname, mongo::BSONObj bo){
		
		// hashmap::const_iterator keyexsit = m_map.find(fieldname);
		
	}
	
}
