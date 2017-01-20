## Roosha Server

To build and run server you need 
* [Vagrant](https://www.vagrantup.com/downloads.html)
* Maven
* Java 8

First of all, you need to build server.
```
mvn clean package
```

Now, you can run it
```
cd redis/
vagrant up
cd ../target
java -jar roosha-server-jar-with-dependencies.jar
```