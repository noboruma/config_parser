#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>
#include <sstream>
#include <fstream>
#include <regex>

#include <unordered_map>
#include <vector>

#include <typeinfo>
#include <stdexcept>

#define CONFIG_LOG_LEVEL 0
#if CONFIG_LOG_LEVEL > 1
  #include <iostream>
  #define CONFIG_LOG_WARN(msg) std::cout<<"[config] Warning: "+msg<<std::endl
#else
  #define CONFIG_LOG_WARN(msg)
#endif

namespace global 
{
  struct config
  {
    config(const std::string& config_file);

    template<typename T>
    T get_parameter(const std::string& param);

    template<typename T>
    void add_parameter(const std::string &key, const T &value);


    protected:
    void parse_line(const std::string& line);
    void parse_file(const std::string& filepath);

    private:
    std::unordered_map<std::string, std::string> _parameters;
  };

  struct config_manager : public config
  {
    static void init(const std::string &file=nullptr)
    {
      singleton = new config_manager(file);
    }

    static config_manager& get()
    {
      if(singleton == nullptr)
        throw std::logic_error("config manager not initialized");
      return *singleton;
    }

    private:
      config_manager(const std::string& file) : config(file) {}
      static config_manager* singleton;
  };
} //!global

#include "config.hxx"

//#undef CONFIG_LOG_LEVEL
//#undef CONFIG_LOG_WARN

#endif
