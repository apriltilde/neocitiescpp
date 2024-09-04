#ifndef NEOCITIES_H
#define NEOCITIES_H

#include <string>
#include <vector>

class Neocities {
public:
    Neocities(const std::string& user, const std::string& password);
    bool upload(const std::vector<std::pair<std::string, std::string>>& files);
    bool deleteFiles(const std::vector<std::string>& filenames);
    std::string getInfo(const std::string& sitename);

private:
    std::string user_;
    std::string password_;
};

#endif // NEOCITIES_H
