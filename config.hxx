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
  template<typename T>
  T config::get_parameter(const std::string& param)
  {

    std::string lowkey = param;
    std::transform(lowkey.begin(), lowkey.end(), lowkey.begin(), ::tolower);

    if(_parameters.find(lowkey) == _parameters.end())
      throw std::logic_error("[config] Accessing unexisting config parameter");

    std::istringstream istr(_parameters[lowkey]);

    T data_value;
    if (!(istr >> data_value))
      throw std::logic_error("config] Convertion to " 
                             + (std::string)typeid(T).name()
                             + " of "+ param +" failed");
    return data_value;
  }

} //!global
