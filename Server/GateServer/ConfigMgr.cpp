#include "ConfigMgr.h"
// Boost Filesystem
#include <boost/filesystem.hpp>             // boost::filesystem::path, boost::filesystem::current_path

// Boost Property Tree
#include <boost/property_tree/ptree.hpp>   // boost::property_tree::ptree
#include <boost/property_tree/ini_parser.hpp> // read_ini, write_ini
#include<iostream>

ConfigMgr::ConfigMgr()
{
    // 获取当前工作目录  
    boost::filesystem::path current_path = boost::filesystem::current_path();
    // 构建config.ini文件的完整路径  
    boost::filesystem::path config_path = current_path / "config.ini";
    std::cout << "Config path: " << config_path << std::endl;

    // 使用Boost.PropertyTree来读取INI文件  
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(config_path.string(), pt);

    for (const auto& section_pair : pt)
    {
        const std::string& section_name = section_pair.first;
        const boost::property_tree::ptree &section_tree = section_pair.second;

        // 对于每个section，遍历其所有的key-value对  
        std::map<std::string, std::string> section_config;
        for (const auto& key_value_pair : section_tree) {
            const std::string& key = key_value_pair.first;
                                                       //还是一个ptree，调用get_value获得value
            const std::string& value = key_value_pair.second.get_value<std::string>();
            section_config[key] = value;
        }
        SectionInfo sectionInfo(section_config);
        // 将section的key-value对保存到config_map中  
        _config_map[section_name] = sectionInfo;
    }


}
