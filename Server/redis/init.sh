#!/usr/bin/env bash

apt-get update > dev/null
apt-get -y install build-essentials

mkdir /opt/redis
cd /opt/redis
wget -q http://download.redis.io/redis-stable.tar.gz
tar xfz redis-stable.tar.gz
cd redis-stable
make
make install

mkdir /etc/redis
mkdir /var/redis
cp /vagrant/redis.init.d.sh /etc/init.d/redis_6379
cp /vagrant/redis.conf /etc/redis/6379.conf
mkdir /var/redis/6379

update-rc.d redis_6379 defaults

chmod a+x /etc/init.d/redis_6379
/etc/init.d/redis_6379 start