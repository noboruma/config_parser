#include <iostream>
#include "../config.hh"

int main()
{
  global::config_manager::load("toto.conf");

  std::cout<<global::config::get<std::string>("ADDR")<<std::endl;

  std::cout<<global::config::get<int>("port")<<std::endl;

  std::cout<<global::config::get<float>("pi")<<std::endl;

  std::vector<float> l = global::config::get_list<float>("list");
  for(auto& i : l)
    std::cout<<i<<std::endl;

  global::config_manager::finish();

  return 0;
}
