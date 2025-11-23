/*
** EPITECH PROJECT, 2024
** test_net
** File description:
** network_client_info
*/

#include "../include/network.hpp"

ClientInfo::ClientInfo(const std::string &ip, unsigned short port, int id)
{
    _ip = ip;
    _port = port;
    _id = id;
}
    
ClientInfo::~ClientInfo()
{

}

std::string ClientInfo::getIp() const 
{
    return _ip;
}

unsigned short ClientInfo::getPort() const
{
    return _port;
}

std::string ClientInfo::getName() const
{
    return _name;
}

void ClientInfo::setName(std::string name)
{
    _name = name;
}

int ClientInfo::getID() const
{
    return _id;
}
