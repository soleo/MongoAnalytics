#!/bin/bash
rm -rf mongo-cxx-driver-v2.4
rm -rf mongodb-linux-x86_64-v2.4-latest.tgz
wget http://downloads.mongodb.org/cxx-driver/mongodb-linux-x86_64-v2.4-latest.tgz
tar xzf mongodb-linux-x86_64-v2.4-latest.tgz
cd mongo-cxx-driver-v2.4
sudo rm .sconsign.dblite
scons
sudo scons install
# sudo chmod 755 -R /usr/local/include/mongo/*
# sudo chmod 755 -R /usr/local/lib/libmongo*