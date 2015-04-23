namespace global {

  // =========================================================================
  template<typename T>
  void config_parser::add_parameter(const std::string &key, const T &value)
  {
    std::ostringstream ostr;
    ostr << value;

    //if(_parameters.find(tokens[0]) != _parameters.end())
      //std::cout<<"[config] warning: duplicate configuration parameter detected!"<<std::endl;

    std::string lowkey = key;
    std::transform(lowkey.begin(), lowkey.end(), lowkey.begin(), ::tolower);

    _parameters[lowkey] = ostr.str();
  }

  // =========================================================================
  template<class T, bool force_cast>
  T config_parser::get_parameter(const std::string& param)
  {
    std::string lowkey = param;
    std::transform(lowkey.begin(), lowkey.end(), lowkey.begin(), ::tolower);

    if(_parameters.find(lowkey) == _parameters.end())
      throw std::logic_error("[config] Accessing unexisting config parameter");

    if(!force_cast && std::is_integral<T>::value)
    {
      const std::regex reg_integer("(\\+|-)?[[:digit:]]+");
      if(!std::regex_match(_parameters[lowkey], reg_integer))
        throw std::logic_error("Try to cast floating point data into integral data");
    }
    else if (!force_cast && std::is_floating_point<T>::value)
    {
      //const std::regex reg_float("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
      const std::regex reg_integer("(\\+|-)?[[:digit:]]+");
      if(std::regex_match(_parameters[lowkey], reg_integer))
        throw std::logic_error("Try to cast integral data into floating point data");
    }

    std::istringstream istr(_parameters[lowkey]);
    T data_value;
    if (!(istr >> data_value))
      throw std::logic_error("config] Convertion to " 
                             + (std::string)typeid(T).name()
                             + " of "+ param +" failed");

    return data_value;
  }

  // =========================================================================
  template<typename T, bool force_cast>
  std::vector<T> config_parser::get_parameter_list(const std::string& param)
  {
    std::string lowkey = param;
    std::transform(lowkey.begin(), lowkey.end(), lowkey.begin(), ::tolower);

    if(_parameters.find(lowkey) == _parameters.end())
      throw std::logic_error("[config] Accessing unexisting config parameter");

    const std::string delim=",[]();";
    std::string value=_parameters[lowkey];
    for(unsigned i=0; i < delim.size(); ++i)
      std::replace(value.begin(), value.end(), delim[i], ' ');

    std::stringstream istr(value);
    T data_value;
    std::vector<T> data_list;
    while(istr >> data_value)
    {
       data_list.push_back(data_value);
       if(!force_cast && std::is_integral<T>::value)
       {
         const std::regex reg_integer("(\\+|-)?[[:digit:]]+");
         if(!std::regex_match(value, reg_integer))
           throw std::logic_error("Try to cast floating point data into integral data");
       }
       else if (!force_cast && std::is_floating_point<T>::value)
       {
         //const std::regex reg_float("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
         const std::regex reg_integer("(\\+|-)?[[:digit:]]+");
         if(std::regex_match(value, reg_integer))
           throw std::logic_error("Try to cast integral data into floating point data");
       }
    }

    return data_list;
  }

  // =========================================================================
  config_parser::config_parser()
  {}

  // =========================================================================
  void config_parser::load_file(const std::string& config_file)
  {
    add_parameter("filename", config_file);
    parse_file(config_file);
  }

  // =========================================================================
  template<>
  std::string config_parser::get_parameter(const std::string &param)
  {

    std::string lowkey = param;
    std::transform(lowkey.begin(), lowkey.end(), lowkey.begin(), ::tolower);

    if(_parameters.find(lowkey) == _parameters.end())
      throw std::logic_error("[config] Accessing unexisting config parameter");

    return _parameters[lowkey];
  }

  // =========================================================================
  void config_parser::parse_file(const std::string& filepath)
  {
    if(filepath.empty())
    {
      CONFIG_LOG_WARN("void file path given.");
      return;
    }

    std::ifstream file(filepath.c_str());
    if (!file)
      throw std::logic_error("[config] Failed at opening config file");
    std::string line;
    while(std::getline(file, line))
      parse_line(line);
  }
  // =========================================================================
  void config_parser::parse_line(std::string line)
  {
    // Test ilform lines
    const std::regex regular_regex("[a-zA-Z0-9]+[\t ]*=[\t ]*");
    if(!std::regex_search(line, regular_regex))
      return;

    // Remove comments 
    {
      const auto p = line.find_first_of("#");
      if (p != std::string::npos)
        line.erase(p);
    }

    const std::regex split_regex("[\t ]*=[\t ]*");
    std::vector<std::string> tokens
    { 
      std::sregex_token_iterator(line.begin(), line.end(), split_regex, -1), // begin()
      std::sregex_token_iterator() // end()
    };

    // trim left side of token
    {
      const auto p = tokens[0].find_first_not_of(" \t");
      tokens[0].erase(0, p);
    }

    // trim right side of token value
    {
      const auto p = tokens[1].find_last_not_of(" \t");
      if (p != std::string::npos)
        tokens[1].erase(p+1);
    }

    {
      std::string data;
      std::smatch m;
      std::regex reg_inner_dquote("^\"(.*)\"$");
      std::regex reg_inner_squote("^\'(.*)\'$");
      std::regex reg_inner_nquote("(.*)");
      std::regex_search(tokens[1], m, reg_inner_dquote);
      if(!m.str(1).empty())
        data = m.str(1);
      else
      {
        std::regex_search(tokens[1], m, reg_inner_squote);
        if(!m.str(1).empty())
          data = m.str(1);
        else
        {
          std::regex_search(tokens[1], m, reg_inner_nquote);
          if(!m.str(1).empty())
            data = m.str(1);
        }
      } 
      add_parameter<std::string>(tokens[0], data);
    }
  }

} //!global
