#ifndef _CONFIGMGR_H_
#define _CONFIGMGR_H_
#include<map>
#include<string>
struct SectionInfo
{
	SectionInfo() = default;
	explicit SectionInfo(const std::map<std::string, std::string>& data) :_section_data(data) {}
	SectionInfo(const SectionInfo& source) = default;
	SectionInfo& operator=(const SectionInfo& source)
	{
		if (this == &source)
		{
			return *this;
		}
		this->_section_data = source._section_data;
	}
	std::string operator[](const std::string& key)const
	{
		auto it = _section_data.find(key);
		if (it == _section_data.end())
			return "";
		return it->second;
	}
	~SectionInfo() = default;
private:
	std::map<std::string, std::string> _section_data;
};


class ConfigMgr
{
public:
	ConfigMgr();

	ConfigMgr(const ConfigMgr& src) {
		this->_config_map = src._config_map;
	}
	~ConfigMgr() = default;

	SectionInfo operator[](const std::string& section) const
	{
		auto it = _config_map.find(section);
		if (it == _config_map.end())
		{
			return SectionInfo();
		}
		
		return it->second;
	}
	ConfigMgr& operator=(const ConfigMgr& src) {
		if (&src == this) {
			return *this;
		}

		this->_config_map = src._config_map;
	};
	
	

private:
	// ´æ´¢sectionºÍkey-value¶ÔµÄmap  
	std::map<std::string, SectionInfo> _config_map;
};

#endif // !_CONFIGMGR_H_



