/*
 *  Copyright (c) 2016 Leo Khotov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  version 3 along with this work.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.github.roosha.server.config;

import com.github.roosha.server.RooshaServerImpl;
import io.grpc.Context;
import io.grpc.Metadata;
import lombok.val;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

@Configuration
@ComponentScan(basePackageClasses = {RooshaServerImpl.class})
public class Config {
    private final int AUTH_REDIS_DB = 0;
    private final int TRANSLATIONS_CACHE_REDIS_DB = 1;

    @Bean
    public Context.Key<Long> authorizedUserIdContextKey() {
        return Context.key("user-id");
    }

    @Bean
    public Metadata.Key<String> authTokenMetadataKey() {
        return Metadata.Key.of("roosha-auth-token", Metadata.ASCII_STRING_MARSHALLER);
    }

    @Bean
    public String yandexDictionaryApiKey() {
        // TODO: load it from file system in runtime.
        return "dict.1.1.20160905T084639Z.02f5a4d75c0b1167.84b38402a3426374a791c5df3e5416e0ef7d9b5c";
    }

    @Bean
    public Integer serverPort() {
        return 1543;
    }

    @Bean(destroyMethod = "destroy")
    public JedisPool authRedisPool() {
        val config = new JedisPoolConfig();
        return new JedisPool(config, "localhost", 6379, 1000, null, AUTH_REDIS_DB);
    }

    @Bean(destroyMethod = "destroy")
    public JedisPool translationsCachePool() {
        val config = new JedisPoolConfig();
        return new JedisPool(config, "localhost", 6379, 1000, null, TRANSLATIONS_CACHE_REDIS_DB);
    }
}
