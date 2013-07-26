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
		m_depth = 4;
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
		//std::set<std::string> fields;
		while(cursor->more()){
			mongo::BSONObj bo = cursor->next();
			
			for( BSONObj::iterator i = bo.begin(); i.more(); ) { 
				BSONElement e = i.next();
				if(skipField(e.fieldName())){
					continue;
				}
				
				if(e.isSimpleType()){
					hashmap::const_iterator keyexsit = m_map.find(e.fieldName());
					SchemaModel* sm = new SchemaModel();
					if(keyexsit != m_map.end()){
							sm = &m_map[e.fieldName()];
							sm->count ++;
							
					}else{
							sm->count = 1;
							sm->datatype = getType(e);
							m_map[e.fieldName()] = *sm; 
					}
				}else if(e.isABSONObj()){
					int depth = 0;
					std::string parent = e.fieldName();
					extractBSON(e.Obj(), depth, parent);
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
	
	std::string MongoSchema::getType(mongo::BSONElement belem){
		
		if(belem.isABSONObj()) {	
			return "BSON";
		}else if(belem.isSimpleType()){
			// Simple Type: Number, String, Date, Bool or OID
			if(belem.isNumber()){
				return "Number";
			}else if(belem.type() == mongo::String){
				return "String";
			}else if(belem.type() == mongo::Date){
				return "Date";
			}else if(belem.type() == mongo::Bool){
				return "Boolean";
			}else{
				return "Other";
			}
			
		}else{
			return "Unknown";
		}		
	}
	
	int MongoSchema::extractBSON(mongo::BSONObj bo, int& depth, std::string parent){

		if(depth >= m_depth){
			return 0;
		}
		depth++;
		
		for( BSONObj::iterator i = bo.begin(); i.more(); ) { 
			BSONElement e = i.next();
			
			if(skipField(e.fieldName())){
					continue;
				}

			std::string fieldname = parent ;
			fieldname.append(".");
			fieldname.append(e.fieldName());
			if(e.isSimpleType()){
					hashmap::const_iterator keyexsit = m_map.find(fieldname);
					SchemaModel* sm = new SchemaModel();
					if(keyexsit != m_map.end()){
							sm = &m_map[fieldname];
							sm->count ++;
							
					}else{
							sm->count = 1;
							sm->datatype = getType(e);
							m_map[fieldname] = *sm; 
					}
			 }else if(e.isABSONObj()){
					
					extractBSON(e.Obj(), depth, fieldname);
			}
				
		}
		
		return 0;		
	}
	
	void MongoSchema::printStringSet(std::set<std::string> set){
		std::set<std::string>::iterator it;
		for(it = set.begin(); it != set.end(); it++)
		{
			std::cout << *it << "\t";
		}
		std::cout << "\r\n";

	}
	
}
