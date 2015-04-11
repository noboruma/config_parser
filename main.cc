#include <iostream>
#include "config.hh"

int main()
{
  global::config_manager::init("toto.conf");

  std::cout<<global::config_manager::get().
      get_parameter<std::string>("ADDR")<<std::endl;

  std::cout<<global::config_manager::get().
      get_parameter<int>("PORT")<<std::endl;

  std::cout<<global::config_manager::get().
      get_parameter<float>("por")<<std::endl;

  return 0;
}
