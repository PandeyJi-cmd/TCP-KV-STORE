#include "Database.h"

namespace RCV
{
    void Database::set(const std::string &key, const std::string &value){
        // lock the reading part and the writing part.
        std::unique_lock<std::shared_mutex> lock(raw_mtx);
        Store[key]=value;
    }
    bool Database::get(const std::string&key, std::string &out_value){
        // lock only the writing part
        std::shared_lock<std::shared_mutex> lock(raw_mtx);
        auto it=Store.find(key);
        if(it!=Store.end()){
            out_value=it->second;
            return true;
        }
        return false;
    }
} // RCV