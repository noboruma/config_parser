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
  struct config_parser
  {
    config_parser();
    void load_file(const std::string& config_file);

    template<typename T, bool force_cast=false>
    T get_parameter(const std::string& param);

    template<typename T, bool force_cast=false>
    std::vector<T> get_parameter_list(const std::string& param);

    template<typename T>
    void add_parameter(const std::string &key, const T &value);


    protected:
    void parse_line(std::string line);
    void parse_file(const std::string& filepath);

    private:
    std::unordered_map<std::string, std::string> _parameters;
  };

  struct config_manager : public config_parser
  {
    static void load(const std::string &file=nullptr)
    {
      singleton()->load_file(file);
    }

    static config_manager& get_instance()
    {
      if(singleton == nullptr)
        throw std::logic_error("config manager not initialized");
      return *singleton();
    }

    template<class T, bool force_cast=false>
    static T get(const std::string &param)
    {
      return get_instance().get_parameter<T>(param);
    }

    template<class T, bool force_cast=false>
    static std::vector<T> get_list(const std::string &param)
    {
      return get_instance().get_parameter_list<T>(param);
    }

    static void finish()
    {
      delete singleton();
      singleton() = nullptr;
    }

    private:
      inline static config_manager*& singleton()
      {
        static config_manager* singleton = new config_manager();
        return singleton;
      }

      config_manager() : config_parser() {}
  };

  typedef config_manager config;
} //!global

#include "config.hxx"

//#undef CONFIG_LOG_LEVEL
//#undef CONFIG_LOG_WARN

#endif
