#ifndef DATABASE
#define DATABASE

#include <string>
#include <unordered_map>
#include <shared_mutex>

namespace RCV
{
    class Database
    {
        std::unordered_map<std::string,std::string> Store;
        mutable std::shared_mutex raw_mtx;


        public:
        Database();
        ~Database();

        void set(const std::string &key, const std::string &val);

        bool get(const std::string &key, std::string& value);
    }; // Database

} // namespace

#endif  // DATABASE