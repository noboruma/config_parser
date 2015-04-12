namespace global {

  // =========================================================================
  template<typename T>
  void config::add_parameter(const std::string &key, const T &value)
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
  template<typename T, bool force_cast>
  T config::get_parameter(const std::string& param)
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

} //!global
